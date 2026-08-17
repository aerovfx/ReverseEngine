#include "Disassembler.h"

#include <cstdio>
#include <cstring>
#include <vector>

int main() {
  a64dbg::Disassembler d;
  if (!d.ok()) {
    std::fprintf(stderr, "disassembler init thất bại\n");
    return 1;
  }

  // mov x0, #42 ; ret  (little-endian, AArch64)
  const std::uint8_t code[] = {0x40, 0x05, 0x80, 0xD2, 0xC0, 0x03, 0x5F, 0xD6};
  std::vector<a64dbg::Instruction> insns;
  const std::size_t n = d.disassemble(0x1000, code, sizeof(code), 16, insns);

  if (n != 2) {
    std::fprintf(stderr, "mong 2 lệnh, nhận %zu\n", n);
    return 1;
  }
  if (insns[0].size != 4 || insns[1].size != 4) {
    std::fprintf(stderr, "size lệnh sai\n");
    return 1;
  }
  const bool firstIsMov =
      insns[0].mnemonic == "mov" || insns[0].mnemonic == "movz";
  if (!firstIsMov || insns[1].mnemonic != "ret") {
    std::fprintf(stderr, "mnemonic sai: [0]=%s [1]=%s\n", insns[0].mnemonic.c_str(),
                 insns[1].mnemonic.c_str());
    return 1;
  }
  if (insns[0].operands.find("x0") == std::string::npos) {
    std::fprintf(stderr, "operand sai: %s\n", insns[0].operands.c_str());
    return 1;
  }

  std::printf("disasm: 0x%llx: %s %s ; %s\n",
              static_cast<unsigned long long>(insns[0].address), insns[0].mnemonic.c_str(),
              insns[0].operands.c_str(), insns[1].mnemonic.c_str());
  std::printf("TEST PASSED\n");
  return 0;
}
