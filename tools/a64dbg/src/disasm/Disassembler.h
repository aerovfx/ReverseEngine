#pragma once

#include <capstone/capstone.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace a64dbg {

// Một lệnh đã giải mã.
struct Instruction {
  std::uint64_t address = 0;
  std::uint8_t bytes[16] = {0};
  std::uint8_t size = 0;
  std::string mnemonic;
  std::string operands;
};

// Bọc Capstone cho AArch64. Stateless — view sẽ tự quản lý cache/giải mã lại.
class Disassembler {
public:
  Disassembler();
  ~Disassembler();

  Disassembler(const Disassembler&) = delete;
  Disassembler& operator=(const Disassembler&) = delete;

  bool ok() const { return m_handle != 0; }

  // Giải mã tối đa `count` lệnh từ `code` (`len` byte) tại `baseAddr`.
  // Trả về số lệnh thực sự giải mã được.
  std::size_t disassemble(std::uint64_t baseAddr, const std::uint8_t* code, std::size_t len,
                          std::size_t count, std::vector<Instruction>& out);

  // Giải mã đúng một lệnh.
  bool disassembleOne(std::uint64_t addr, const std::uint8_t* code, std::size_t len,
                      Instruction& out);

private:
  csh m_handle = 0;
};

// Phiên bản Capstone đang được link, ví dụ "5.0.9".
const char* capstoneVersion();

}  // namespace a64dbg
