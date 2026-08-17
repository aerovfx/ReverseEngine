#include "MachOParser.h"

#include <cstdio>
#include <string>
#include <vector>

int main() {
  std::vector<std::uint8_t> buf;
  if (!a64dbg::macho::readFile(TARGET_PATH, buf)) {
    std::fprintf(stderr, "đọc file thất bại\n");
    return 1;
  }

  a64dbg::macho::Image img;
  if (!a64dbg::macho::parse(buf.data(), buf.size(), 0, img)) {
    std::fprintf(stderr, "parse thất bại\n");
    return 1;
  }
  if (!img.is64) {
    std::fprintf(stderr, "không phải Mach-O 64-bit\n");
    return 1;
  }

  bool hasText = false;
  bool hasBp = false;
  bool hasMain = false;
  for (const auto& s : img.segments) {
    if (s.name == "__TEXT") {
      hasText = true;
    }
  }
  for (const auto& s : img.symbols) {
    if (s.name == "_bp_here") {
      hasBp = true;
    }
    if (s.name == "_main") {
      hasMain = true;
    }
  }

  if (!hasText || !hasBp || !hasMain) {
    std::fprintf(stderr, "thiếu: __TEXT=%d _bp_here=%d _main=%d\n", hasText, hasBp, hasMain);
    return 1;
  }

  std::printf("segments=%zu symbols=%zu entry=0x%llx\n", img.segments.size(),
              img.symbols.size(), static_cast<unsigned long long>(img.entryPoint));
  std::printf("TEST PASSED\n");
  return 0;
}
