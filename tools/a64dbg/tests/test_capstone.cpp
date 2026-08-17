#include "Disassembler.h"

#include <cstdio>
#include <cstring>

int main() {
  const char* v = a64dbg::capstoneVersion();
  if (v == nullptr || std::strlen(v) == 0) {
    std::fprintf(stderr, "capstone version rỗng\n");
    return 1;
  }
  std::printf("capstone version: %s\n", v);
  return 0;
}
