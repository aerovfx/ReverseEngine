#include "MachBackend.h"

#include "Log.h"
#include "mig/mach_exc.h"

#include <mach/arm/exception.h>
#include <mach/arm/thread_status.h>
#include <mach/exception_types.h>
#include <mach/mach_vm.h>
#include <mach/task.h>
#include <mach/thread_act.h>

#include <spawn.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <cstring>

extern char** environ;

namespace a64dbg::core {

// MIG server demux (được định nghĩa trong mach_excServer.c).
extern "C" boolean_t mach_exc_server(mach_msg_header_t* InHeadP, mach_msg_header_t* OutHeadP);

namespace {

// Các catch_* là callback mà mach_exc_server gọi; cần một con trỏ toàn cục
// vì chúng là free function (C linkage), không có this.
MachBackend* g_backend = nullptr;

// AArch64: BRK #imm16 = 0xD4200000 | (imm16 << 5). Dùng BRK #0.
constexpr std::uint32_t kBrkInstruction = 0xD4200000u;
constexpr int kPageSize = 0x1000;

kern_return_t defaultReply(thread_state_t new_state, mach_msg_type_number_t* new_stateCnt,
                           thread_state_t old_state, mach_msg_type_number_t old_stateCnt) {
  std::memcpy(new_state, old_state, old_stateCnt * sizeof(natural_t));
  *new_stateCnt = old_stateCnt;
  return KERN_SUCCESS;
}

}  // namespace

extern "C" kern_return_t catch_mach_exception_raise(
    mach_port_t /*exception_port*/, mach_port_t /*thread*/, mach_port_t /*task*/,
    exception_type_t /*exception*/, mach_exception_data_t /*code*/,
    mach_msg_type_number_t /*codeCnt*/) {
  return KERN_SUCCESS;
}

extern "C" kern_return_t catch_mach_exception_raise_state(
    mach_port_t /*exception_port*/, exception_type_t /*exception*/,
    const mach_exception_data_t /*code*/, mach_msg_type_number_t /*codeCnt*/,
    int* /*flavor*/, const thread_state_t old_state, mach_msg_type_number_t old_stateCnt,
    thread_state_t new_state, mach_msg_type_number_t* new_stateCnt) {
  return defaultReply(new_state, new_stateCnt, (thread_state_t)old_state, old_stateCnt);
}

extern "C" kern_return_t catch_mach_exception_raise_state_identity(
    mach_port_t /*exception_port*/, mach_port_t /*thread*/, mach_port_t /*task*/,
    exception_type_t exception, mach_exception_data_t code, mach_msg_type_number_t codeCnt,
    int* /*flavor*/, thread_state_t old_state, mach_msg_type_number_t old_stateCnt,
    thread_state_t new_state, mach_msg_type_number_t* new_stateCnt) {
  if (g_backend != nullptr) {
    return g_backend->handleException(exception, code, codeCnt, old_state, old_stateCnt,
                                      new_state, new_stateCnt);
  }
  return defaultReply(new_state, new_stateCnt, old_state, old_stateCnt);
}

// ---------------------------------------------------------------------------

MachBackend::MachBackend() { g_backend = this; }

MachBackend::~MachBackend() {
  detach();
  if (g_backend == this) {
    g_backend = nullptr;
  }
}

bool MachBackend::launch(const std::vector<std::string>& args) {
  if (args.empty()) {
    return false;
  }
  std::vector<char*> argv;
  for (const auto& a : args) {
    argv.push_back(const_cast<char*>(a.c_str()));
  }
  argv.push_back(nullptr);

  posix_spawnattr_t attr;
  posix_spawnattr_init(&attr);
  posix_spawnattr_setflags(&attr, POSIX_SPAWN_START_SUSPENDED);
  pid_t pid = 0;
  const int rc = posix_spawn(&pid, argv[0], nullptr, &attr, argv.data(), environ);
  posix_spawnattr_destroy(&attr);
  if (rc != 0) {
    return false;
  }
  return attach(static_cast<std::uint32_t>(pid));
}

bool MachBackend::attach(std::uint32_t pid) {
  if (m_task != MACH_PORT_NULL) {
    detach();
  }
  if (task_for_pid(mach_task_self(), pid, &m_task) != KERN_SUCCESS) {
    m_task = MACH_PORT_NULL;
    logError("task_for_pid thất bại (cần entitlement com.apple.security.cs.debugger)");
    return false;
  }

  // Lấy thread chính để đọc state khi target đang chạy.
  thread_act_array_t threads = nullptr;
  mach_msg_type_number_t count = 0;
  if (task_threads(m_task, &threads, &count) == KERN_SUCCESS && count > 0) {
    m_primaryThread = threads[0];
    for (mach_msg_type_number_t i = 1; i < count; ++i) {
      mach_port_deallocate(mach_task_self(), threads[i]);
    }
    vm_deallocate(mach_task_self(), reinterpret_cast<vm_address_t>(threads),
                  count * sizeof(thread_t));
  }

  if (!setupExceptionPort()) {
    detach();
    return false;
  }
  m_stop = false;
  m_exceptionThread = std::thread(&MachBackend::exceptionLoop, this);
  logInfo("attached pid=" + std::to_string(pid));
  return true;
}

bool MachBackend::detach() {
  if (m_task == MACH_PORT_NULL) {
    return true;
  }

  // Nếu đang dừng (target đã suspend), khôi phục mọi software breakpoint + tắt
  // hw bp/watchpoint TRƯỚC khi resume, để target không re-trigger sau khi reply.
  if (m_stopped.load()) {
    for (const auto& [addr, orig] : m_swBps) {
      writeCode(addr, &orig, sizeof(orig));
    }
    m_swBps.clear();
    if (m_primaryThread != MACH_PORT_NULL) {
      arm_debug_state64_t ds{};
      mach_msg_type_number_t count = ARM_DEBUG_STATE64_COUNT;
      if (thread_get_state(m_primaryThread, ARM_DEBUG_STATE64,
                           reinterpret_cast<thread_state_t>(&ds), &count) == KERN_SUCCESS) {
        for (int i = 0; i < 16; ++i) {
          ds.__bcr[i] = 0;
          ds.__wcr[i] = 0;
        }
        thread_set_state(m_primaryThread, ARM_DEBUG_STATE64,
                         reinterpret_cast<thread_state_t>(&ds), count);
      }
    }
    m_hwBps.clear();
  }

  m_stop = true;
  {
    std::lock_guard<std::mutex> lk(m_mtx);
    m_resumeRequested = true;
  }
  m_cv.notify_all();
  if (m_exceptionThread.joinable()) {
    m_exceptionThread.join();
  }
  teardownExceptionPort();
  if (m_primaryThread != MACH_PORT_NULL) {
    mach_port_deallocate(mach_task_self(), m_primaryThread);
    m_primaryThread = MACH_PORT_NULL;
  }
  mach_port_deallocate(mach_task_self(), m_task);
  m_task = MACH_PORT_NULL;
  m_swBps.clear();
  m_stopped = false;
  m_resumeRequested = false;
  return true;
}

bool MachBackend::suspend() {
  if (m_task == MACH_PORT_NULL) {
    return false;
  }
  return task_suspend(m_task) == KERN_SUCCESS;
}

bool MachBackend::resume() {
  if (m_task == MACH_PORT_NULL) {
    return false;
  }
  {
    std::unique_lock<std::mutex> lk(m_mtx);
    if (m_stopped.load()) {
      // Đang dừng ở software breakpoint: lùi PC về địa chỉ bp để lệnh gốc được chạy.
      if (m_lastEvent.reason == StopReason::Breakpoint && m_lastBreakpointAddr != 0) {
        const std::uint64_t bpAddr = m_lastBreakpointAddr;
        m_threadState.__pc = bpAddr;
        if (m_swBps.count(bpAddr)) {
          // Breakpoint dance: user giữ bp → khôi phục lệnh gốc, single-step 1 lệnh,
          // rồi re-patch BRK ở handleException (transparent với user).
          const std::uint32_t orig = m_swBps[bpAddr];
          writeCode(bpAddr, &orig, sizeof(orig));
          enableSingleStep(true);
          m_pendingBpAddr.store(bpAddr);
        }
      }
      m_resumeRequested = true;
      m_cv.notify_all();
      // Chờ stop hiện tại được clear để waitForStop() sau không trả về sự kiện cũ.
      m_cv.wait(lk, [&] { return !m_stopped.load(); });
      return true;
    }
  }
  return task_resume(m_task) == KERN_SUCCESS;
}

bool MachBackend::stepInto() {
  if (!enableSingleStep(true)) {
    return false;
  }
  return resume();
}

bool MachBackend::enableSingleStep(bool enable) {
  arm_debug_state64_t ds{};
  mach_msg_type_number_t count = ARM_DEBUG_STATE64_COUNT;
  if (thread_get_state(m_primaryThread, ARM_DEBUG_STATE64, reinterpret_cast<thread_state_t>(&ds),
                       &count) != KERN_SUCCESS) {
    return false;
  }
  // __mdscr_el1 bit 0 = SS (hardware single step).
  if (enable) {
    ds.__mdscr_el1 |= 1ull;
  } else {
    ds.__mdscr_el1 &= ~1ull;
  }
  return thread_set_state(m_primaryThread, ARM_DEBUG_STATE64,
                          reinterpret_cast<thread_state_t>(&ds), count) == KERN_SUCCESS;
}

bool MachBackend::setSoftwareBreakpoint(std::uint64_t addr) {
  if (m_swBps.count(addr)) {
    return true;
  }
  std::uint32_t orig = 0;
  if (!readMemory(addr, &orig, sizeof(orig))) {
    return false;
  }
  const std::uint32_t brk = kBrkInstruction;
  if (!writeCode(addr, &brk, sizeof(brk))) {
    return false;
  }
  m_swBps[addr] = orig;
  return true;
}

bool MachBackend::removeSoftwareBreakpoint(std::uint64_t addr) {
  const auto it = m_swBps.find(addr);
  if (it == m_swBps.end()) {
    return false;
  }
  if (!writeCode(addr, &it->second, sizeof(it->second))) {
    return false;
  }
  m_swBps.erase(it);
  return true;
}

bool MachBackend::setHardwareBreakpoint(std::uint64_t addr) {
  if (m_hwBps.count(addr)) {
    return true;
  }
  if (m_task == MACH_PORT_NULL || m_primaryThread == MACH_PORT_NULL) {
    return false;
  }
  arm_debug_state64_t ds{};
  mach_msg_type_number_t count = ARM_DEBUG_STATE64_COUNT;
  if (thread_get_state(m_primaryThread, ARM_DEBUG_STATE64,
                       reinterpret_cast<thread_state_t>(&ds), &count) != KERN_SUCCESS) {
    return false;
  }
  int slot = -1;
  for (int i = 0; i < 16; ++i) {
    if ((ds.__bcr[i] & 1u) == 0) {  // slot chưa bật
      slot = i;
      break;
    }
  }
  if (slot < 0) {
    return false;  // hết 16 slot
  }
  ds.__bvr[slot] = addr;
  // BCR: E=1, BT=0b00 (unlinked instruction address match), BAS=0xF (4 byte).
  ds.__bcr[slot] = 0x1E1u;
  if (thread_set_state(m_primaryThread, ARM_DEBUG_STATE64,
                       reinterpret_cast<thread_state_t>(&ds), count) != KERN_SUCCESS) {
    return false;
  }
  {
    std::lock_guard<std::mutex> lk(m_hwBpsMtx);
    m_hwBps.insert(addr);
  }
  return true;
}

bool MachBackend::removeHardwareBreakpoint(std::uint64_t addr) {
  if (!m_hwBps.count(addr)) {
    return false;
  }
  if (m_primaryThread == MACH_PORT_NULL) {
    return false;
  }
  arm_debug_state64_t ds{};
  mach_msg_type_number_t count = ARM_DEBUG_STATE64_COUNT;
  if (thread_get_state(m_primaryThread, ARM_DEBUG_STATE64,
                       reinterpret_cast<thread_state_t>(&ds), &count) != KERN_SUCCESS) {
    return false;
  }
  for (int i = 0; i < 16; ++i) {
    if (ds.__bvr[i] == addr) {
      ds.__bcr[i] = 0;  // tắt slot
    }
  }
  if (thread_set_state(m_primaryThread, ARM_DEBUG_STATE64,
                       reinterpret_cast<thread_state_t>(&ds), count) != KERN_SUCCESS) {
    return false;
  }
  {
    std::lock_guard<std::mutex> lk(m_hwBpsMtx);
    m_hwBps.erase(addr);
  }
  return true;
}

bool MachBackend::setWatchpoint(std::uint64_t addr, std::size_t size) {
  if (m_task == MACH_PORT_NULL || m_primaryThread == MACH_PORT_NULL) {
    return false;
  }
  arm_debug_state64_t ds{};
  mach_msg_type_number_t count = ARM_DEBUG_STATE64_COUNT;
  if (thread_get_state(m_primaryThread, ARM_DEBUG_STATE64,
                       reinterpret_cast<thread_state_t>(&ds), &count) != KERN_SUCCESS) {
    return false;
  }
  int slot = -1;
  for (int i = 0; i < 16; ++i) {
    if ((ds.__wcr[i] & 1u) == 0) {
      slot = i;
      break;
    }
  }
  if (slot < 0) {
    return false;
  }
  ds.__wvr[slot] = addr;
  // WCR: E=1, PAC=0b10 (EL0), LSC=0b11 (load+store), BAS=(1<<size)-1.
  const std::uint64_t bas = (size >= 8) ? 0xFF : ((1u << size) - 1);
  ds.__wcr[slot] = (bas << 5) | (0b11 << 3) | (0b10 << 1) | 1;
  return thread_set_state(m_primaryThread, ARM_DEBUG_STATE64,
                          reinterpret_cast<thread_state_t>(&ds), count) == KERN_SUCCESS;
}

bool MachBackend::removeWatchpoint(std::uint64_t addr) {
  if (m_primaryThread == MACH_PORT_NULL) {
    return false;
  }
  arm_debug_state64_t ds{};
  mach_msg_type_number_t count = ARM_DEBUG_STATE64_COUNT;
  if (thread_get_state(m_primaryThread, ARM_DEBUG_STATE64,
                       reinterpret_cast<thread_state_t>(&ds), &count) != KERN_SUCCESS) {
    return false;
  }
  bool found = false;
  for (int i = 0; i < 16; ++i) {
    if (ds.__wvr[i] == addr) {
      ds.__wcr[i] = 0;
      found = true;
    }
  }
  if (!found) {
    return false;
  }
  return thread_set_state(m_primaryThread, ARM_DEBUG_STATE64,
                          reinterpret_cast<thread_state_t>(&ds), count) == KERN_SUCCESS;
}

bool MachBackend::readMemory(std::uint64_t addr, void* buf, std::size_t len) {
  if (m_task == MACH_PORT_NULL) {
    return false;
  }
  mach_vm_size_t outSize = 0;
  const kern_return_t kr = mach_vm_read_overwrite(
      m_task, addr, len, reinterpret_cast<mach_vm_address_t>(buf), &outSize);
  return kr == KERN_SUCCESS && outSize == len;
}

bool MachBackend::writeMemory(std::uint64_t addr, const void* buf, std::size_t len) {
  return writeCode(addr, buf, len);
}

bool MachBackend::writeCode(std::uint64_t addr, const void* buf, std::size_t len) {
  if (m_task == MACH_PORT_NULL) {
    return false;
  }
  // Patch code R-X: làm trang ghi được tạm (COW) → ghi → khôi phục execute.
  // W^X cấm một trang vừa write vừa execute, nên phải tách 2 bước; target
  // phải đang bị suspend để khoảng mất-execute không bị chạy vào.
  const mach_vm_address_t pageStart = addr & ~(static_cast<mach_vm_address_t>(kPageSize - 1));
  const mach_vm_size_t span =
      ((addr + len + kPageSize - 1) & ~(static_cast<mach_vm_address_t>(kPageSize - 1))) -
      pageStart;
  mach_vm_protect(m_task, pageStart, span, FALSE, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
  const kern_return_t kr =
      mach_vm_write(m_task, addr, reinterpret_cast<vm_offset_t>(buf), static_cast<mach_msg_type_number_t>(len));
  mach_vm_protect(m_task, pageStart, span, FALSE, VM_PROT_READ | VM_PROT_EXECUTE);
  return kr == KERN_SUCCESS;
}

bool MachBackend::readRegisters(std::uint64_t* regs, std::size_t count) {
  RegisterState rs;
  if (!readRegisterState(rs)) {
    return false;
  }
  const std::size_t n = count < static_cast<std::size_t>(kRegisterCount) ? count : kRegisterCount;
  for (std::size_t i = 0; i < n; ++i) {
    regs[i] = rs.r[i];
  }
  return true;
}

bool MachBackend::writeRegisters(const std::uint64_t* regs, std::size_t count) {
  RegisterState rs;
  if (!readRegisterState(rs)) {
    return false;
  }
  const std::size_t n = count < static_cast<std::size_t>(kRegisterCount) ? count : kRegisterCount;
  for (std::size_t i = 0; i < n; ++i) {
    rs.r[i] = regs[i];
  }
  return writeRegisterState(rs);
}

bool MachBackend::readRegisterState(RegisterState& out) {
  arm_thread_state64_t st{};
  if (!getThreadState(st)) {
    return false;
  }
  for (int i = 0; i < 29; ++i) {
    out.r[i] = st.__x[i];
  }
  out.r[static_cast<int>(Reg::FP)] = st.__fp;
  out.r[static_cast<int>(Reg::LR)] = st.__lr;
  out.r[static_cast<int>(Reg::SP)] = st.__sp;
  out.r[static_cast<int>(Reg::PC)] = st.__pc;
  out.r[static_cast<int>(Reg::CPSR)] = st.__cpsr;
  return true;
}

bool MachBackend::writeRegisterState(const RegisterState& in) {
  arm_thread_state64_t st{};
  if (!getThreadState(st)) {
    return false;
  }
  for (int i = 0; i < 29; ++i) {
    st.__x[i] = in.r[i];
  }
  st.__fp = in.r[static_cast<int>(Reg::FP)];
  st.__lr = in.r[static_cast<int>(Reg::LR)];
  st.__sp = in.r[static_cast<int>(Reg::SP)];
  st.__pc = in.r[static_cast<int>(Reg::PC)];
  st.__cpsr = static_cast<std::uint32_t>(in.r[static_cast<int>(Reg::CPSR)]);

  if (m_stopped.load()) {
    std::lock_guard<std::mutex> lk(m_mtx);
    m_threadState = st;
    return true;
  }
  mach_msg_type_number_t count = ARM_THREAD_STATE64_COUNT;
  return thread_set_state(m_primaryThread, ARM_THREAD_STATE64,
                          reinterpret_cast<thread_state_t>(&st), count) == KERN_SUCCESS;
}

bool MachBackend::getThreadState(arm_thread_state64_t& out) {
  if (m_stopped.load()) {
    std::lock_guard<std::mutex> lk(m_mtx);
    out = m_threadState;
    return true;
  }
  if (m_primaryThread == MACH_PORT_NULL) {
    return false;
  }
  mach_msg_type_number_t count = ARM_THREAD_STATE64_COUNT;
  return thread_get_state(m_primaryThread, ARM_THREAD_STATE64,
                          reinterpret_cast<thread_state_t>(&out), &count) == KERN_SUCCESS;
}

bool MachBackend::waitForStop(int timeoutMs) {
  std::unique_lock<std::mutex> lk(m_mtx);
  if (m_stopped.load()) {
    return true;
  }
  return m_cv.wait_for(lk, std::chrono::milliseconds(timeoutMs),
                       [&] { return m_stopped.load(); });
}

bool MachBackend::setupExceptionPort() {
  if (mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &m_exceptionPort) !=
      KERN_SUCCESS) {
    return false;
  }
  if (mach_port_insert_right(mach_task_self(), m_exceptionPort, m_exceptionPort,
                             MACH_MSG_TYPE_MAKE_SEND) != KERN_SUCCESS) {
    return false;
  }
  const exception_mask_t mask = EXC_MASK_BREAKPOINT | EXC_MASK_BAD_ACCESS | EXC_MASK_ARITHMETIC |
                                EXC_MASK_SOFTWARE | EXC_MASK_BAD_INSTRUCTION | EXC_MASK_CRASH;
  const kern_return_t kr =
      task_set_exception_ports(m_task, mask, m_exceptionPort,
                               EXCEPTION_STATE_IDENTITY | MACH_EXCEPTION_CODES, ARM_THREAD_STATE64);
  return kr == KERN_SUCCESS;
}

void MachBackend::teardownExceptionPort() {
  if (m_exceptionPort != MACH_PORT_NULL) {
    mach_port_deallocate(mach_task_self(), m_exceptionPort);
    m_exceptionPort = MACH_PORT_NULL;
  }
}

void MachBackend::buildEvent(exception_type_t exception, mach_exception_data_t code,
                             mach_msg_type_number_t codeCnt) {
  m_lastEvent = DebugEvent{};
  m_lastEvent.pc = m_threadState.__pc;
  m_lastBreakpointAddr = 0;
  switch (exception) {
    case EXC_BREAKPOINT:
      if (codeCnt >= 1 && code[0] == EXC_ARM_BREAKPOINT) {
        // code[1] = 0 → single-step; code[1] != 0 → BRK mềm HOẶC hw breakpoint
        // (trên macOS ARM64 cả hai đều gửi EXC_ARM_BREAKPOINT). Phân biệt bằng bảng hw bp.
        if (codeCnt >= 2 && code[1] == 0) {
          m_lastEvent.reason = StopReason::SingleStep;
          m_lastEvent.message = "single step";
        } else {
          bool isHw = false;
          {
            std::lock_guard<std::mutex> lk(m_hwBpsMtx);
            isHw = m_hwBps.count(m_threadState.__pc) != 0;
          }
          if (isHw) {
            m_lastEvent.reason = StopReason::HardwareBreakpoint;
            m_lastEvent.message = "hardware breakpoint";
          } else {
            m_lastEvent.reason = StopReason::Breakpoint;
            m_lastEvent.message = "software breakpoint (BRK)";
            // ARM64 báo PC TẠI lệnh BRK, nên địa chỉ bp = pc.
            m_lastBreakpointAddr = m_threadState.__pc;
          }
        }
      } else if (codeCnt >= 1 && code[0] == EXC_ARM_DA_DEBUG) {
        // Watchpoint: EXC_BREAKPOINT + code[0]=EXC_ARM_DA_DEBUG(0x102), code[1]=địa chỉ watch.
        m_lastEvent.reason = StopReason::Watchpoint;
        m_lastEvent.message = "watchpoint";
      } else {
        m_lastEvent.reason = StopReason::SingleStep;
        m_lastEvent.message = "debug exception";
      }
      break;
    case EXC_BAD_ACCESS:
      m_lastEvent.reason = StopReason::Exception;
      m_lastEvent.message = "EXC_BAD_ACCESS";
      break;
    case EXC_BAD_INSTRUCTION:
      m_lastEvent.reason = StopReason::Exception;
      m_lastEvent.message = "EXC_BAD_INSTRUCTION";
      break;
    case EXC_ARITHMETIC:
      m_lastEvent.reason = StopReason::Exception;
      m_lastEvent.message = "EXC_ARITHMETIC";
      break;
    default:
      m_lastEvent.reason = StopReason::Exception;
      m_lastEvent.message = "exception " + std::to_string(exception);
      break;
  }
}

kern_return_t MachBackend::handleException(exception_type_t exception, mach_exception_data_t code,
                                           mach_msg_type_number_t codeCnt,
                                           thread_state_t old_state,
                                           mach_msg_type_number_t old_stateCnt,
                                           thread_state_t new_state,
                                           mach_msg_type_number_t* new_stateCnt) {
  // Lưu state của thread lỗi (ARM_THREAD_STATE64) để GUI đọc.
  if (old_stateCnt >= ARM_THREAD_STATE64_COUNT) {
    std::memcpy(&m_threadState, old_state, sizeof(arm_thread_state64_t));
  }
  buildEvent(exception, code, codeCnt);

  // Breakpoint dance: single-step sau khi phục hồi lệnh gốc → re-patch + tự continue
  // (không dừng cho user, không gọi handler).
  if (m_lastEvent.reason == StopReason::SingleStep && m_pendingBpAddr.load() != 0) {
    const std::uint64_t bpAddr = m_pendingBpAddr.load();
    task_suspend(m_task);
    const std::uint32_t brk = kBrkInstruction;
    writeCode(bpAddr, &brk, sizeof(brk));
    m_pendingBpAddr.store(0);
    enableSingleStep(false);
    task_resume(m_task);
    std::memcpy(new_state, &m_threadState, sizeof(arm_thread_state64_t));
    *new_stateCnt = ARM_THREAD_STATE64_COUNT;
    return KERN_SUCCESS;
  }

  // Sau single-step phải tắt SS, nếu không mọi continue/step sau đều bị step tiếp.
  if (m_lastEvent.reason == StopReason::SingleStep) {
    enableSingleStep(false);
  }

  // Stop-the-world: dừng các thread khác.
  task_suspend(m_task);

  m_resumeRequested = false;
  m_stopped = true;
  m_cv.notify_all();
  if (m_handler) {
    m_handler(m_lastEvent);
  }

  // Chờ lệnh resume từ GUI/test.
  {
    std::unique_lock<std::mutex> lk(m_mtx);
    m_cv.wait(lk, [&] { return m_resumeRequested.load() || m_stop.load(); });
    m_resumeRequested = false;
    m_stopped = false;
    m_cv.notify_all();  // đánh thức resume() đang chờ stop được clear
    // PC đã có thể bị resume() chỉnh lại (lùi về địa chỉ bp).
    std::memcpy(new_state, &m_threadState, sizeof(arm_thread_state64_t));
    *new_stateCnt = ARM_THREAD_STATE64_COUNT;
  }

  // Thả các thread còn lại; thread lỗi được thả khi reply gửi đi.
  task_resume(m_task);
  return KERN_SUCCESS;
}

void MachBackend::exceptionLoop() {
  union ExceptionMessage {
    mach_msg_header_t header;
    char padding[16384];
  } recv, reply;

  while (!m_stop.load()) {
    const mach_msg_return_t rc =
        mach_msg(&recv.header, MACH_RCV_MSG | MACH_RCV_LARGE | MACH_RCV_TIMEOUT, 0, sizeof(recv),
                 m_exceptionPort, 100, MACH_PORT_NULL);
    if (rc != MACH_MSG_SUCCESS) {
      continue;  // timeout hoặc lỗi — vòng lại kiểm tra m_stop.
    }
    if (mach_exc_server(&recv.header, &reply.header)) {
      mach_msg(&reply.header, MACH_SEND_MSG, reply.header.msgh_size, 0, MACH_PORT_NULL, 0,
               MACH_PORT_NULL);
    }
  }
}

}  // namespace a64dbg::core
