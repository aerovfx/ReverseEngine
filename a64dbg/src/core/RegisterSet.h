#pragma once

#include <cstdint>

namespace a64dbg::core {

// Chỉ số thanh ghi AArch64 dùng chung giữa core và GUI (khớp bảng Registers).
enum class Reg : int {
  X0 = 0, X1, X2, X3, X4, X5, X6, X7, X8, X9,
  X10, X11, X12, X13, X14, X15, X16, X17, X18, X19,
  X20, X21, X22, X23, X24, X25, X26, X27, X28,
  FP = 29,   // x29
  LR = 30,   // x30
  SP = 31,   // x31
  PC = 32,
  CPSR = 33,
};

inline constexpr int kRegisterCount = 34;

inline constexpr const char* kRegisterNames[kRegisterCount] = {
    "x0",  "x1",  "x2",  "x3",  "x4",  "x5",  "x6",  "x7",  "x8",  "x9",
    "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19",
    "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "fp",
    "lr",  "sp",  "pc",  "cpsr",
};

struct RegisterState {
  std::uint64_t r[kRegisterCount] = {};
};

}  // namespace a64dbg::core
