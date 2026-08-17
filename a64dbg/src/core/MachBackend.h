#pragma once

#include "IDebugBackend.h"
#include "RegisterSet.h"

#include <mach/arm/thread_status.h>
#include <mach/exception_types.h>
#include <mach/mach.h>

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace a64dbg::core {

using EventHandler = std::function<void(const DebugEvent&)>;

// Backend Mach API — Phase 1. Attach/launch, memory, register,
// software breakpoint (BRK patch), bắt exception qua Mach exception port.
// stepOver/stepOut defer sang Phase 3/4 (LLDB-assisted); stepInto dùng hardware SS.
class MachBackend : public IDebugBackend {
public:
  MachBackend();
  ~MachBackend() override;

  void setEventHandler(EventHandler handler) { m_handler = std::move(handler); }

  // IDebugBackend
  bool attach(std::uint32_t pid) override;
  bool detach() override;
  bool resume() override;
  bool suspend() override;
  bool stepInto() override;
  bool stepOver() override { return false; }  // Phase 3/4
  bool stepOut() override { return false; }   // Phase 3/4
  bool setSoftwareBreakpoint(std::uint64_t addr) override;
  bool removeSoftwareBreakpoint(std::uint64_t addr) override;
  bool readMemory(std::uint64_t addr, void* buf, std::size_t len) override;
  bool writeMemory(std::uint64_t addr, const void* buf, std::size_t len) override;
  bool readRegisters(std::uint64_t* regs, std::size_t count) override;
  bool writeRegisters(const std::uint64_t* regs, std::size_t count) override;

  // Tiện ích Phase 1 (ngoài contract).
  bool launch(const std::vector<std::string>& args);
  bool isAttached() const { return m_task != MACH_PORT_NULL; }
  bool readRegisterState(RegisterState& out);
  bool writeRegisterState(const RegisterState& in);
  bool waitForStop(int timeoutMs);
  const DebugEvent& lastEvent() const { return m_lastEvent; }

  // Gọi từ catch_mach_exception_* (phải extern "C" nên đi qua hàm thành viên này).
  kern_return_t handleException(exception_type_t exception, mach_exception_data_t code,
                                mach_msg_type_number_t codeCnt,
                                thread_state_t old_state, mach_msg_type_number_t old_stateCnt,
                                thread_state_t new_state, mach_msg_type_number_t* new_stateCnt);

private:
  void exceptionLoop();
  bool setupExceptionPort();
  void teardownExceptionPort();
  bool getThreadState(arm_thread_state64_t& out);
  bool writeCode(std::uint64_t addr, const void* buf, std::size_t len);
  void buildEvent(exception_type_t exception, mach_exception_data_t code,
                  mach_msg_type_number_t codeCnt);
  bool enableSingleStep(bool enable);

  task_t m_task = MACH_PORT_NULL;
  thread_t m_primaryThread = MACH_PORT_NULL;
  mach_port_t m_exceptionPort = MACH_PORT_NULL;
  std::thread m_exceptionThread;

  std::atomic<bool> m_stop{false};
  std::atomic<bool> m_stopped{false};
  std::atomic<bool> m_resumeRequested{false};
  std::mutex m_mtx;
  std::condition_variable m_cv;

  arm_thread_state64_t m_threadState{};
  DebugEvent m_lastEvent;
  std::uint64_t m_lastBreakpointAddr = 0;
  EventHandler m_handler;

  std::unordered_map<std::uint64_t, std::uint32_t> m_swBps;  // addr -> lệnh gốc
};

}  // namespace a64dbg::core
