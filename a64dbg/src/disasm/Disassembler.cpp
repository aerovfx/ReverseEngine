#include "Disassembler.h"

#include <cstdio>
#include <cstring>

namespace a64dbg {

const char* capstoneVersion() {
  static char buf[32] = {0};
  if (buf[0] == '\0') {
    // Dùng macro phiên bản release (CS_VERSION_EXTRA là số patch, vd 9 → "5.0.9").
    // cs_version() chỉ trả về API version 5.0, không phải version release đầy đủ.
    std::snprintf(buf, sizeof(buf), "%d.%d.%d", CS_VERSION_MAJOR, CS_VERSION_MINOR,
                  CS_VERSION_EXTRA);
  }
  return buf;
}

Disassembler::Disassembler() {
  if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &m_handle) != CS_ERR_OK) {
    m_handle = 0;
  }
}

Disassembler::~Disassembler() {
  if (m_handle != 0) {
    cs_close(&m_handle);
  }
}

std::size_t Disassembler::disassemble(std::uint64_t baseAddr, const std::uint8_t* code,
                                      std::size_t len, std::size_t count,
                                      std::vector<Instruction>& out) {
  out.clear();
  if (m_handle == 0 || code == nullptr || len == 0 || count == 0) {
    return 0;
  }
  cs_insn* insn = nullptr;
  const std::size_t n = cs_disasm(m_handle, code, len, baseAddr, count, &insn);
  if (n == 0 || insn == nullptr) {
    if (insn != nullptr) {
      cs_free(insn, count);
    }
    return 0;
  }
  out.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    Instruction inst;
    inst.address = insn[i].address;
    inst.size = static_cast<std::uint8_t>(insn[i].size);
    if (inst.size > sizeof(inst.bytes)) {
      inst.size = sizeof(inst.bytes);
    }
    std::memcpy(inst.bytes, insn[i].bytes, inst.size);
    inst.mnemonic = insn[i].mnemonic ? insn[i].mnemonic : "";
    inst.operands = insn[i].op_str ? insn[i].op_str : "";
    out.push_back(std::move(inst));
  }
  cs_free(insn, n);
  return n;
}

bool Disassembler::disassembleOne(std::uint64_t addr, const std::uint8_t* code, std::size_t len,
                                  Instruction& out) {
  std::vector<Instruction> tmp;
  if (disassemble(addr, code, len, 1, tmp) == 0) {
    return false;
  }
  out = std::move(tmp[0]);
  return true;
}

}  // namespace a64dbg
