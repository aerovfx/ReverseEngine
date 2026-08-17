#pragma once

#include "Disassembler.h"
#include "MachBackend.h"
#include "PluginHost.h"
#include "RegisterSet.h"

#include <QObject>

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

namespace a64dbg {

struct StackFrame {
  std::uint64_t fp = 0;
  std::uint64_t returnAddr = 0;
};

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
  void stepOver();
  void stepOut();
  bool setBreakpoint(std::uint64_t addr);
  bool removeBreakpoint(std::uint64_t addr);

  // Đọc state (hợp lệ khi đang dừng).
  bool readRegisters(core::RegisterState& out);
  bool readMemory(std::uint64_t addr, std::size_t len, std::vector<std::uint8_t>& out);
  bool disassembleAt(std::uint64_t addr, int count, std::vector<Instruction>& out);

  // Phase 5: stack walk + tìm kiếm + patch.
  std::vector<StackFrame> stackWalk(int maxFrames = 32);
  std::vector<std::uint64_t> searchBytes(std::uint64_t start, std::uint64_t len,
                                         const std::vector<std::uint8_t>& pattern);
  bool patchBytes(std::uint64_t addr, const std::vector<std::uint8_t>& bytes);
  bool nopFill(std::uint64_t addr, std::size_t count);  // count = số lệnh (4 byte/lệnh)

  // Phase 6: plugin.
  bool loadPlugin(const std::string& path);
  void* pluginSymbol(const std::string& name) const;

signals:
  void stopped();  // target đang dừng, GUI nên đọc state
  void running();  // target đang chạy

private:
  core::MachBackend m_backend;
  Disassembler m_disasm;
  std::unique_ptr<plugin::PluginHost> m_plugins;
  std::atomic<bool> m_stopped{false};
  std::uint64_t m_tempBpAddr = 0;  // temp breakpoint cho step over/out (chỉ main thread)
};

}  // namespace a64dbg
