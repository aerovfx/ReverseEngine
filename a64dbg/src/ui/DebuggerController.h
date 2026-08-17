#pragma once

#include "Disassembler.h"
#include "MachBackend.h"
#include "RegisterSet.h"

#include <QObject>

#include <atomic>
#include <cstdint>
#include <vector>

namespace a64dbg {

// Cầu nối giữa MachBackend (chạy trên thread riêng) và GUI (main thread).
// Backend báo stop qua EventHandler → controller phát signal stopped() trên main thread.
class DebuggerController : public QObject {
  Q_OBJECT

public:
  explicit DebuggerController(QObject* parent = nullptr);
  ~DebuggerController() override;

  bool attach(int pid);
  bool detach();
  bool isAttached() const;
  bool isStopped() const { return m_stopped.load(); }

  void runContinue();
  void stepInto();
  bool setBreakpoint(std::uint64_t addr);
  bool removeBreakpoint(std::uint64_t addr);

  // Đọc state (hợp lệ khi đang dừng).
  bool readRegisters(core::RegisterState& out);
  bool readMemory(std::uint64_t addr, std::size_t len, std::vector<std::uint8_t>& out);
  bool disassembleAt(std::uint64_t addr, int count, std::vector<Instruction>& out);

signals:
  void stopped();  // target đang dừng, GUI nên đọc state
  void running();  // target đang chạy

private:
  core::MachBackend m_backend;
  Disassembler m_disasm;
  std::atomic<bool> m_stopped{false};
};

}  // namespace a64dbg
