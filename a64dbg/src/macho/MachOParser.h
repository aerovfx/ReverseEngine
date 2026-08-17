#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace a64dbg::macho {

struct Segment {
  std::string name;
  std::uint64_t vmaddr = 0;    // địa chỉ ảo (chưa cộng slide)
  std::uint64_t vmsize = 0;
  std::uint64_t fileoff = 0;
  std::uint64_t filesize = 0;
  std::uint32_t initprot = 0;
  std::uint32_t maxprot = 0;
};

struct Symbol {
  std::string name;
  std::uint64_t address = 0;  // địa chỉ ảo (chưa cộng slide)
};

struct Image {
  bool is64 = false;
  std::uint64_t entryPoint = 0;  // địa chỉ ảo đã cộng slide
  std::vector<Segment> segments;
  std::vector<Symbol> symbols;
};

// Parse Mach-O 64-bit từ buffer. `slide` = độ lệch ASLR (0 khi đọc từ file).
// Trả về true nếu là Mach-O 64-bit hợp lệ.
bool parse(const std::uint8_t* data, std::size_t len, std::uint64_t slide, Image& out);

// Đọc toàn bộ file vào buffer (tiện ích cho test/CLI).
bool readFile(const std::string& path, std::vector<std::uint8_t>& out);

}  // namespace a64dbg::macho
