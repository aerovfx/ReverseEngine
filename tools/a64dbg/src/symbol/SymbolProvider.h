#pragma once

#include <mach/mach.h>

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace a64dbg::symbol {

struct ImageInfo {
  std::uint64_t base = 0;  // mach_header* trong bộ nhớ target
  std::string name;
};

struct ResolvedSymbol {
  std::string image;
  std::string symbol;
  std::uint64_t symbolAddr = 0;
  std::uint64_t offset = 0;  // addr - symbolAddr
};

// Symbolication native: đọc danh sách ảnh qua TASK_DYLD_INFO + parse symbol table
// (LC_SYMTAB) từ bộ nhớ target — không phụ thuộc LLDB SB API.
class SymbolProvider {
public:
  using MemoryReader = std::function<bool(std::uint64_t, void*, std::size_t)>;

  SymbolProvider(MemoryReader reader, mach_port_t task);

  bool enumerate();  // đọc danh sách ảnh đã nạp
  const std::vector<ImageInfo>& images() const { return m_images; }

  // Resolve địa chỉ → hàm chứa nó (symbol lớn nhất ≤ addr trong image).
  bool resolve(std::uint64_t addr, ResolvedSymbol& out);

private:
  struct ParsedImage {
    std::string name;
    std::uint64_t base = 0;
    std::uint64_t vmsize = 0;
    std::uint64_t slide = 0;
    std::vector<std::pair<std::uint64_t, std::string>> symbols;  // (runtime addr, name)
  };

  bool readString(std::uint64_t addr, std::string& out);
  bool parseImage(const ImageInfo& img, ParsedImage& out);

  MemoryReader m_reader;
  mach_port_t m_task;
  std::vector<ImageInfo> m_images;
  std::unordered_map<std::uint64_t, ParsedImage> m_cache;  // base -> parsed
};

}  // namespace a64dbg::symbol
