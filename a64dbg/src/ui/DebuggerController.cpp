#include "DebuggerController.h"

#include <cstdio>
#include <cstring>

namespace a64dbg {

DebuggerController::DebuggerController(QObject* parent) : QObject(parent) {
  m_plugins = std::make_unique<plugin::PluginHost>(
      [this](std::uint64_t a, void* b, std::size_t n) { return m_backend.readMemory(a, b, n); },
      [this](std::uint64_t a, const void* b, std::size_t n) {
        return m_backend.writeMemory(a, b, n);
      },
      [this](std::uint64_t* r, std::size_t n) { return m_backend.readRegisters(r, n); });

  m_backend.setEventHandler([this](const core::DebugEvent& event) {
    // Chạy trên exception thread của backend → post signal sang main thread.
    m_stopped.store(true);
    const std::uint64_t stopPc = event.pc;

    // Dispatch sự kiện cho plugin.
    dbg_event e{};
    e.type = (event.reason == core::StopReason::Breakpoint) ? DBG_EVENT_BREAKPOINT
                                                            : DBG_EVENT_STOP;
    e.pc = event.pc;
    e.address = event.pc;
    std::snprintf(e.message, sizeof(e.message), "%s", event.message.c_str());
    m_plugins->notifyEvent(e);

    QMetaObject::invokeMethod(
        this,
        [this, stopPc] {
          // Dọn temp breakpoint (step over/out) nếu trúng.
          if (m_tempBpAddr != 0 && stopPc == m_tempBpAddr) {
            m_backend.removeSoftwareBreakpoint(m_tempBpAddr);
            m_tempBpAddr = 0;
          }
          emit stopped();
        },
        Qt::QueuedConnection);
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

void DebuggerController::stepOver() {
  core::RegisterState rs;
  if (!m_backend.readRegisterState(rs)) {
    stepInto();
    return;
  }
  const std::uint64_t pc = rs.r[static_cast<int>(core::Reg::PC)];
  std::vector<Instruction> insns;
  if (!disassembleAt(pc, 1, insns) || insns.empty()) {
    stepInto();  // không disasm được → single step
    return;
  }
  // Gọi (bl/blr) → đặt temp bp ngay sau lệnh; còn lại single-step.
  const bool isCall = (insns[0].mnemonic == "bl" || insns[0].mnemonic == "blr");
  if (isCall) {
    const std::uint64_t next = pc + insns[0].size;
    if (m_backend.setSoftwareBreakpoint(next)) {
      m_tempBpAddr = next;
      m_stopped.store(false);
      m_backend.resume();
      emit running();
      return;
    }
  }
  stepInto();
}

void DebuggerController::stepOut() {
  core::RegisterState rs;
  if (!m_backend.readRegisterState(rs)) {
    runContinue();
    return;
  }
  const std::uint64_t lr = rs.r[static_cast<int>(core::Reg::LR)];
  if (lr == 0 || !m_backend.setSoftwareBreakpoint(lr)) {
    runContinue();
    return;
  }
  m_tempBpAddr = lr;
  m_stopped.store(false);
  m_backend.resume();
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

std::vector<StackFrame> DebuggerController::stackWalk(int maxFrames) {
  std::vector<StackFrame> frames;
  core::RegisterState rs;
  if (!m_backend.readRegisterState(rs)) {
    return frames;
  }
  std::uint64_t fp = rs.r[static_cast<int>(core::Reg::FP)];
  for (int i = 0; i < maxFrames && fp != 0; ++i) {
    std::uint64_t savedFp = 0;
    std::uint64_t savedLr = 0;
    if (!m_backend.readMemory(fp, &savedFp, sizeof(savedFp))) {
      break;
    }
    if (!m_backend.readMemory(fp + 8, &savedLr, sizeof(savedLr))) {
      break;
    }
    frames.push_back(StackFrame{fp, savedLr});
    if (savedFp == 0 || savedFp == fp) {
      break;
    }
    fp = savedFp;
  }
  return frames;
}

std::vector<std::uint64_t> DebuggerController::searchBytes(
    std::uint64_t start, std::uint64_t len, const std::vector<std::uint8_t>& pattern) {
  std::vector<std::uint64_t> hits;
  if (pattern.empty() || len == 0) {
    return hits;
  }
  std::vector<std::uint8_t> buf(static_cast<std::size_t>(len));
  if (!readMemory(start, len, buf)) {
    return hits;
  }
  for (std::size_t i = 0; i + pattern.size() <= buf.size(); ++i) {
    if (std::memcmp(buf.data() + i, pattern.data(), pattern.size()) == 0) {
      hits.push_back(start + static_cast<std::uint64_t>(i));
    }
  }
  return hits;
}

bool DebuggerController::patchBytes(std::uint64_t addr, const std::vector<std::uint8_t>& bytes) {
  if (bytes.empty()) {
    return false;
  }
  return m_backend.writeMemory(addr, bytes.data(), bytes.size());
}

bool DebuggerController::nopFill(std::uint64_t addr, std::size_t count) {
  if (count == 0) {
    return false;
  }
  // AArch64 NOP = 0xD503201F.
  const std::uint32_t nop = 0xD503201F;
  std::vector<std::uint8_t> buf;
  buf.reserve(count * 4);
  for (std::size_t i = 0; i < count; ++i) {
    buf.push_back(static_cast<std::uint8_t>(nop & 0xff));
    buf.push_back(static_cast<std::uint8_t>((nop >> 8) & 0xff));
    buf.push_back(static_cast<std::uint8_t>((nop >> 16) & 0xff));
    buf.push_back(static_cast<std::uint8_t>((nop >> 24) & 0xff));
  }
  return m_backend.writeMemory(addr, buf.data(), buf.size());
}

bool DebuggerController::loadPlugin(const std::string& path) {
  return m_plugins->load(path);
}

void* DebuggerController::pluginSymbol(const std::string& name) const {
  return m_plugins->symbol(name);
}

}  // namespace a64dbg
