#include "MachOParser.h"

#include <mach-o/loader.h>
#include <mach-o/nlist.h>

#include <cstring>
#include <fstream>

namespace a64dbg::macho {

namespace {

// Đọc an toàn một struct từ buffer ở offset, trả về con trỏ (nullptr nếu vượt biên).
template <typename T>
const T* at(const std::uint8_t* data, std::size_t len, std::size_t off) {
  if (off + sizeof(T) > len) {
    return nullptr;
  }
  return reinterpret_cast<const T*>(data + off);
}

// Địa chỉ ảo từ file offset (ánh xạ qua segment chứa nó).
std::uint64_t fileOffToVmAddr(const Image& img, std::uint64_t fileoff) {
  for (const auto& seg : img.segments) {
    if (fileoff >= seg.fileoff && fileoff < seg.fileoff + seg.filesize) {
      return seg.vmaddr + (fileoff - seg.fileoff);
    }
  }
  return 0;
}

}  // namespace

bool parse(const std::uint8_t* data, std::size_t len, std::uint64_t slide, Image& out) {
  out = Image{};
  if (data == nullptr || len < sizeof(mach_header_64)) {
    return false;
  }
  const auto* hdr = reinterpret_cast<const mach_header_64*>(data);
  if (hdr->magic != MH_MAGIC_64) {
    return false;
  }
  out.is64 = true;

  std::size_t off = sizeof(mach_header_64);
  const std::uint32_t ncmds = hdr->ncmds;
  const std::size_t cmdsEnd = off + hdr->sizeofcmds;

  const symtab_command* symtab = nullptr;
  std::uint64_t mainEntryOff = UINT64_MAX;

  for (std::uint32_t i = 0; i < ncmds && off + sizeof(load_command) <= cmdsEnd; ++i) {
    const auto* lc = reinterpret_cast<const load_command*>(data + off);
    if (lc->cmdsize < sizeof(load_command) || off + lc->cmdsize > cmdsEnd) {
      break;
    }
    switch (lc->cmd) {
      case LC_SEGMENT_64: {
        const auto* sc = at<segment_command_64>(data, len, off);
        if (sc == nullptr) {
          break;
        }
        Segment seg;
        seg.name.assign(sc->segname, strnlen(sc->segname, 16));
        seg.vmaddr = sc->vmaddr;
        seg.vmsize = sc->vmsize;
        seg.fileoff = sc->fileoff;
        seg.filesize = sc->filesize;
        seg.initprot = sc->initprot;
        seg.maxprot = sc->maxprot;
        out.segments.push_back(std::move(seg));
        break;
      }
      case LC_SYMTAB: {
        symtab = at<symtab_command>(data, len, off);
        break;
      }
      case LC_MAIN: {
        const auto* ep = at<entry_point_command>(data, len, off);
        if (ep != nullptr) {
          mainEntryOff = ep->entryoff;
        }
        break;
      }
      default:
        break;
    }
    off += lc->cmdsize;
  }

  // Symbol table.
  if (symtab != nullptr) {
    const auto* nlistBase = at<nlist_64>(data, len, symtab->symoff);
    const char* strtab =
        (symtab->stroff + symtab->strsize <= len)
            ? reinterpret_cast<const char*>(data + symtab->stroff)
            : nullptr;
    if (nlistBase != nullptr && strtab != nullptr) {
      for (std::uint32_t i = 0; i < symtab->nsyms; ++i) {
        const nlist_64* nl = at<nlist_64>(data, len, symtab->symoff + i * sizeof(nlist_64));
        if (nl == nullptr) {
          break;
        }
        // Bỏ symbol debug (N_STAB), undefined (import), và symbol không tên/không địa chỉ.
        if ((nl->n_type & N_STAB) != 0) {
          continue;
        }
        if ((nl->n_type & N_TYPE) != N_SECT || nl->n_value == 0) {
          continue;
        }
        if (nl->n_un.n_strx == 0 || nl->n_un.n_strx >= symtab->strsize) {
          continue;
        }
        Symbol sym;
        sym.name = strtab + nl->n_un.n_strx;
        sym.address = slide + nl->n_value;
        out.symbols.push_back(std::move(sym));
      }
    }
  }

  // Entry point: ánh xạ entryoff (file offset) sang địa chỉ ảo qua segment.
  if (mainEntryOff != UINT64_MAX) {
    out.entryPoint = slide + fileOffToVmAddr(out, mainEntryOff);
  }

  return true;
}

bool readFile(const std::string& path, std::vector<std::uint8_t>& out) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    return false;
  }
  f.seekg(0, std::ios::end);
  const std::streamoff size = f.tellg();
  if (size <= 0) {
    return false;
  }
  f.seekg(0, std::ios::beg);
  out.resize(static_cast<std::size_t>(size));
  f.read(reinterpret_cast<char*>(out.data()), size);
  return f.good() || f.eof();
}

}  // namespace a64dbg::macho
