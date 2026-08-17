#pragma once

namespace a64dbg {

// Phiên bản Capstone đang được link, ví dụ "5.0.9".
// (Phase 2 sẽ thay bằng lớp Disassembler đầy đủ: cs_open ARM64 + instruction cache.)
const char* capstoneVersion();

}  // namespace a64dbg
