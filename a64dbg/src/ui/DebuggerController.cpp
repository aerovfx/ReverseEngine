#include "DebuggerController.h"

namespace a64dbg {

DebuggerController::DebuggerController(QObject* parent) : QObject(parent) {
  m_backend.setEventHandler([this](const core::DebugEvent& /*event*/) {
    // Chạy trên exception thread của backend → post signal sang main thread.
    m_stopped.store(true);
    QMetaObject::invokeMethod(this, [this] { emit stopped(); }, Qt::QueuedConnection);
  });
}

DebuggerController::~DebuggerController() {
  m_backend.detach();
}

bool DebuggerController::attach(int pid) {
  if (!m_backend.attach(static_cast<std::uint32_t>(pid))) {
    return false;
  }
  m_backend.suspend();  // dừng target để GUI đọc state ban đầu
  m_stopped.store(true);
  emit stopped();
  return true;
}

bool DebuggerController::detach() {
  const bool ok = m_backend.detach();
  m_stopped.store(false);
  return ok;
}

void DebuggerController::runContinue() {
  m_stopped.store(false);
  m_backend.resume();
  emit running();
}

void DebuggerController::stepInto() {
  m_stopped.store(false);
  m_backend.stepInto();
  emit running();
}

bool DebuggerController::setBreakpoint(std::uint64_t addr) {
  return m_backend.setSoftwareBreakpoint(addr);
}

bool DebuggerController::removeBreakpoint(std::uint64_t addr) {
  return m_backend.removeSoftwareBreakpoint(addr);
}

bool DebuggerController::readRegisters(core::RegisterState& out) {
  return m_backend.readRegisterState(out);
}

bool DebuggerController::readMemory(std::uint64_t addr, std::size_t len,
                                    std::vector<std::uint8_t>& out) {
  out.resize(len);
  if (!m_backend.readMemory(addr, out.data(), len)) {
    out.clear();
    return false;
  }
  return true;
}

bool DebuggerController::disassembleAt(std::uint64_t addr, int count,
                                       std::vector<Instruction>& out) {
  // Đọc đủ byte (tối đa 4 byte/lệnh ARM64) rồi giải mã.
  std::vector<std::uint8_t> code;
  if (!readMemory(addr, static_cast<std::size_t>(count) * 4, code)) {
    return false;
  }
  return m_disasm.disassemble(addr, code.data(), code.size(), static_cast<std::size_t>(count),
                              out) > 0;
}

}  // namespace a64dbg
