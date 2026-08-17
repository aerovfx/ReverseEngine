// Integration headless: kiểm tra cầu nối DebuggerController (Qt signal) ↔ MachBackend.
// Dùng QCoreApplication (không cần GUI) để chạy event loop.
#include "DebuggerController.h"
#include "Disassembler.h"

#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <signal.h>
#include <spawn.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>

extern char** environ;

namespace {

bool readLineWithTimeout(int fd, char* buf, size_t cap, int timeoutMs) {
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  timeval tv{timeoutMs / 1000, (timeoutMs % 1000) * 1000};
  if (select(fd + 1, &rfds, nullptr, nullptr, &tv) <= 0) {
    return false;
  }
  char* p = buf;
  while (static_cast<size_t>(p - buf) + 1 < cap) {
    const ssize_t n = read(fd, p, 1);
    if (n <= 0) {
      break;
    }
    if (*p == '\n') {
      *p = '\0';
      return true;
    }
    ++p;
  }
  *p = '\0';
  return p != buf;
}

}  // namespace

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  // Spawn target, đọc địa chỉ bp_here.
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return 1;
  }
  const char* targetPath = TARGET_PATH;
  char* argvArr[] = {const_cast<char*>(targetPath), nullptr};
  posix_spawn_file_actions_t fa;
  posix_spawn_file_actions_init(&fa);
  posix_spawn_file_actions_adddup2(&fa, pipefd[1], STDOUT_FILENO);
  posix_spawn_file_actions_addclose(&fa, pipefd[0]);
  pid_t pid = 0;
  const int rc = posix_spawn(&pid, targetPath, &fa, nullptr, argvArr, environ);
  posix_spawn_file_actions_destroy(&fa);
  close(pipefd[1]);
  if (rc != 0) {
    std::fprintf(stderr, "spawn thất bại\n");
    return 1;
  }

  char line[128] = {0};
  if (!readLineWithTimeout(pipefd[0], line, sizeof(line), 5000)) {
    std::fprintf(stderr, "không nhận READY\n");
    kill(pid, SIGKILL);
    return 1;
  }
  unsigned long long addr = 0;
  if (std::sscanf(line, "READY %llx", &addr) != 1) {
    std::fprintf(stderr, "dòng READY sai\n");
    kill(pid, SIGKILL);
    return 1;
  }

  a64dbg::DebuggerController controller;

  if (!controller.attach(static_cast<int>(pid))) {
    std::fprintf(stderr, "attach thất bại\n");
    kill(pid, SIGKILL);
    return 1;
  }
  if (!controller.setBreakpoint(addr)) {
    std::fprintf(stderr, "set breakpoint thất bại\n");
    kill(pid, SIGKILL);
    return 1;
  }

  // Chờ signal stopped() bất đồng bộ khi breakpoint hit.
  bool bpHit = false;
  QEventLoop loop;
  QObject::connect(&controller, &a64dbg::DebuggerController::stopped, [&] {
    bpHit = true;
    loop.quit();
  });
  QTimer::singleShot(5000, &loop, &QEventLoop::quit);

  controller.runContinue();
  loop.exec();

  if (!bpHit) {
    std::fprintf(stderr, "không nhận stopped() trong 5s\n");
    kill(pid, SIGKILL);
    return 1;
  }

  // Đọc register: PC phải = địa chỉ breakpoint.
  a64dbg::core::RegisterState rs;
  if (!controller.readRegisters(rs)) {
    std::fprintf(stderr, "đọc register thất bại\n");
    kill(pid, SIGKILL);
    return 1;
  }
  const std::uint64_t pc = rs.r[static_cast<int>(a64dbg::core::Reg::PC)];
  std::printf("stopped, pc=0x%llx (mong 0x%llx)\n",
              static_cast<unsigned long long>(pc), addr);
  if (pc != addr) {
    std::fprintf(stderr, "PC sai\n");
    kill(pid, SIGKILL);
    return 1;
  }

  // Disassemble tại PC (cầu nối disasm qua controller).
  std::vector<a64dbg::Instruction> insns;
  if (!controller.disassembleAt(pc, 5, insns) || insns.empty()) {
    std::fprintf(stderr, "disassemble thất bại\n");
    kill(pid, SIGKILL);
    return 1;
  }
  std::printf("disasm[0]: %s %s\n", insns[0].mnemonic.c_str(), insns[0].operands.c_str());

  // Dọn dẹp.
  controller.removeBreakpoint(addr);
  controller.runContinue();
  controller.detach();
  kill(pid, SIGKILL);
  waitpid(pid, nullptr, 0);
  std::printf("TEST PASSED\n");
  return 0;
}
