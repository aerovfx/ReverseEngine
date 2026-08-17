#include "SymbolProvider.h"

#include <mach-o/dyld_images.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach/task_info.h>

#include <algorithm>
#include <cstdio>
#include <cstring>

namespace a64dbg::symbol {

SymbolProvider::SymbolProvider(MemoryReader reader, mach_port_t task)
    : m_reader(std::move(reader)), m_task(task) {}

bool SymbolProvider::enumerate() {
  m_images.clear();
  struct task_dyld_info info;
  mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
  if (task_info(m_task, TASK_DYLD_INFO, reinterpret_cast<task_info_t>(&info), &count) !=
      KERN_SUCCESS) {
    return false;
  }
  // Header dyld_all_image_infos: version(u32) + infoArrayCount(u32) + infoArray(u64).
  struct {
    std::uint32_t version;
    std::uint32_t count;
    std::uint64_t array;
  } hdr{};
  if (!m_reader(info.all_image_info_addr, &hdr, sizeof(hdr))) {
    return false;
  }
  for (std::uint32_t i = 0; i < hdr.count && i < 4096; ++i) {
    struct dyld_image_info img {};
    if (!m_reader(hdr.array + static_cast<std::uint64_t>(i) * sizeof(img), &img, sizeof(img))) {
      continue;
    }
    ImageInfo ii;
    ii.base = reinterpret_cast<std::uint64_t>(img.imageLoadAddress);
    if (ii.base == 0) {
      continue;
    }
    if (!readString(reinterpret_cast<std::uint64_t>(img.imageFilePath), ii.name)) {
      continue;
    }
    m_images.push_back(std::move(ii));
  }
  return !m_images.empty();
}

bool SymbolProvider::readString(std::uint64_t addr, std::string& out) {
  out.clear();
  char buf[256];
  for (int chunk = 0; chunk < 16; ++chunk) {
    if (!m_reader(addr + static_cast<std::uint64_t>(chunk) * sizeof(buf), buf, sizeof(buf))) {
      return !out.empty();
    }
    const char* nul = static_cast<const char*>(std::memchr(buf, 0, sizeof(buf)));
    if (nul != nullptr) {
      out.append(buf, static_cast<std::size_t>(nul - buf));
      return true;
    }
    out.append(buf, sizeof(buf));
  }
  return !out.empty();
}

namespace {

// Ánh xạ file offset → địa chỉ bộ nhớ qua danh sách segment.
std::uint64_t fileOffToMem(const std::vector<segment_command_64>& segs, std::uint64_t fileoff) {
  for (const auto& s : segs) {
    if (fileoff >= s.fileoff && fileoff < s.fileoff + s.filesize) {
      return s.vmaddr + (fileoff - s.fileoff);
    }
  }
  return UINT64_MAX;
}

}  // namespace

bool SymbolProvider::parseImage(const ImageInfo& img, ParsedImage& out) {
  mach_header_64 hdr{};
  if (!m_reader(img.base, &hdr, sizeof(hdr))) {
    return false;
  }
  if (hdr.magic != MH_MAGIC_64) {
    return false;
  }

  std::vector<std::uint8_t> cmds(hdr.sizeofcmds);
  if (hdr.sizeofcmds > 0 &&
      !m_reader(img.base + sizeof(mach_header_64), cmds.data(), cmds.size())) {
    return false;
  }

  std::vector<segment_command_64> segs;
  const symtab_command* symtab = nullptr;

  std::size_t off = 0;
  for (std::uint32_t i = 0; i < hdr.ncmds && off + sizeof(load_command) <= cmds.size(); ++i) {
    const auto* lc = reinterpret_cast<const load_command*>(cmds.data() + off);
    if (lc->cmdsize < sizeof(load_command) || off + lc->cmdsize > cmds.size()) {
      break;
    }
    switch (lc->cmd) {
      case LC_SEGMENT_64: {
        const auto* sc = reinterpret_cast<const segment_command_64*>(cmds.data() + off);
        segs.push_back(*sc);
        break;
      }
      case LC_SYMTAB:
        symtab = reinterpret_cast<const symtab_command*>(cmds.data() + off);
        break;
      default:
        break;
    }
    off += lc->cmdsize;
  }

  if (segs.empty()) {
    return false;
  }

  // Bỏ segment unmapped (vd __PAGEZERO có maxprot=0); slide = base - vmaddr của __TEXT.
  std::uint64_t minVmaddr = UINT64_MAX;
  std::uint64_t maxEnd = 0;
  for (const auto& s : segs) {
    if (s.maxprot == 0) {
      continue;
    }
    minVmaddr = std::min(minVmaddr, s.vmaddr);
    maxEnd = std::max(maxEnd, s.vmaddr + s.vmsize);
  }
  if (minVmaddr == UINT64_MAX) {
    return false;
  }

  out.name = img.name;
  out.base = img.base;
  out.slide = img.base - minVmaddr;
  out.vmsize = maxEnd - minVmaddr;

  if (symtab == nullptr || symtab->nsyms == 0) {
    return true;  // không có symbol table (vd một số dylib stripped)
  }

  // fileOffToMem trả vmaddr CHƯA trượt ASLR → phải cộng slide để có địa chỉ runtime.
  const std::uint64_t symVm = fileOffToMem(segs, symtab->symoff);
  const std::uint64_t strVm = fileOffToMem(segs, symtab->stroff);
  if (symVm == UINT64_MAX || strVm == UINT64_MAX) {
    return false;
  }
  const std::uint64_t symAddr = out.slide + symVm;
  const std::uint64_t strAddr = out.slide + strVm;

  std::vector<nlist_64> nlist(symtab->nsyms);
  if (!m_reader(symAddr, nlist.data(), nlist.size() * sizeof(nlist_64))) {
    return false;
  }
  std::string strtab(symtab->strsize, '\0');
  if (symtab->strsize > 0 && !m_reader(strAddr, strtab.data(), symtab->strsize)) {
    return false;
  }

  for (const auto& nl : nlist) {
    if ((nl.n_type & N_STAB) != 0) {
      continue;
    }
    if ((nl.n_type & N_TYPE) != N_SECT || nl.n_value == 0) {
      continue;
    }
    if (nl.n_un.n_strx == 0 || nl.n_un.n_strx >= symtab->strsize) {
      continue;
    }
    const std::string name = strtab.c_str() + nl.n_un.n_strx;
    const std::uint64_t runtime = out.slide + nl.n_value;
    out.symbols.emplace_back(runtime, name);
  }
  std::sort(out.symbols.begin(), out.symbols.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
  return true;
}

bool SymbolProvider::resolve(std::uint64_t addr, ResolvedSymbol& out) {
  for (const auto& img : m_images) {
    auto it = m_cache.find(img.base);
    if (it == m_cache.end()) {
      ParsedImage parsed;
      if (!parseImage(img, parsed)) {
        continue;
      }
      it = m_cache.emplace(img.base, std::move(parsed)).first;
    }
    const ParsedImage& p = it->second;
    if (addr < p.base || addr >= p.base + p.vmsize) {
      continue;
    }
    // Tìm symbol lớn nhất ≤ addr.
    const std::string* name = nullptr;
    std::uint64_t symAddr = 0;
    for (const auto& [a, n] : p.symbols) {
      if (a <= addr) {
        name = &n;
        symAddr = a;
      } else {
        break;
      }
    }
    out.image = p.name;
    out.symbol = name ? *name : "<unknown>";
    out.symbolAddr = symAddr;
    out.offset = addr - symAddr;
    return true;
  }
  return false;
}

}  // namespace a64dbg::symbol
