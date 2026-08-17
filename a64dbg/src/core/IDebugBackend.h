#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace a64dbg::core {

enum class StopReason { None, Entry, Breakpoint, SingleStep, Exception, Exited };

// Sự kiện chuẩn hóa từ backend đẩy lên GUI (qua hàng đợi thread-safe).
struct DebugEvent {
  StopReason reason = StopReason::None;
  std::uint64_t pc = 0;
  std::uint32_t threadId = 0;
  std::string message;
};

// Contract chung cho mọi backend (MachBackend — Phase 1, LLDBBackend — Phase 4).
// GUI chỉ được nói chuyện qua interface này, không được chạm header Mach/LLDB.
class IDebugBackend {
public:
  virtual ~IDebugBackend() = default;

  virtual bool attach(std::uint32_t pid) = 0;
  virtual bool detach() = 0;

  virtual bool resume() = 0;
  virtual bool suspend() = 0;

  virtual bool stepInto() = 0;
  virtual bool stepOver() = 0;
  virtual bool stepOut() = 0;

  virtual bool setSoftwareBreakpoint(std::uint64_t addr) = 0;
  virtual bool removeSoftwareBreakpoint(std::uint64_t addr) = 0;

  virtual bool readMemory(std::uint64_t addr, void* buf, std::size_t len) = 0;
  virtual bool writeMemory(std::uint64_t addr, const void* buf, std::size_t len) = 0;

  // Mảng register tổng quát; MachBackend ánh xạ sang ARM_THREAD_STATE64.
  virtual bool readRegisters(std::uint64_t* regs, std::size_t count) = 0;
  virtual bool writeRegisters(const std::uint64_t* regs, std::size_t count) = 0;
};

}  // namespace a64dbg::core
