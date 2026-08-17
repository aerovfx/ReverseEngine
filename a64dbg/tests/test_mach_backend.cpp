#include "MachBackend.h"
#include "RegisterSet.h"

#include <cstdio>
#include <cstring>
#include <string>

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
  const int rc = select(fd + 1, &rfds, nullptr, nullptr, &tv);
  if (rc <= 0) {
    return false;
  }
  char* p = buf;
  while (static_cast<size_t>(p - buf) + 1 < cap) {
    ssize_t n = read(fd, p, 1);
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

int main() {
  setvbuf(stdout, nullptr, _IONBF, 0);  // in ngay để dễ soi chỗ treo
  // 1. Spawn target, redirect stdout qua pipe.
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return 1;
  }
  const char* targetPath = TARGET_PATH;
  char* argv[] = {const_cast<char*>(targetPath), nullptr};
  posix_spawn_file_actions_t fa;
  posix_spawn_file_actions_init(&fa);
  posix_spawn_file_actions_adddup2(&fa, pipefd[1], STDOUT_FILENO);
  posix_spawn_file_actions_addclose(&fa, pipefd[0]);
  pid_t pid = 0;
  const int rc = posix_spawn(&pid, targetPath, &fa, nullptr, argv, environ);
  posix_spawn_file_actions_destroy(&fa);
  close(pipefd[1]);
  if (rc != 0) {
    std::fprintf(stderr, "spawn thất bại\n");
    return 1;
  }

  // 2. Đọc "READY <addr>".
  char line[128] = {0};
  if (!readLineWithTimeout(pipefd[0], line, sizeof(line), 5000)) {
    std::fprintf(stderr, "không nhận được READY\n");
    kill(pid, SIGKILL);
    return 1;
  }
  unsigned long long addr = 0;
  if (std::sscanf(line, "READY %llx", &addr) != 1) {
    std::fprintf(stderr, "dòng READY sai: %s\n", line);
    kill(pid, SIGKILL);
    return 1;
  }
  std::printf("breakpoint addr = 0x%llx\n", addr);

  // 3. Attach + đặt software breakpoint.
  a64dbg::core::MachBackend backend;
  if (!backend.attach(static_cast<std::uint32_t>(pid))) {
    std::fprintf(stderr, "attach thất bại\n");
    kill(pid, SIGKILL);
    return 1;
  }
  backend.suspend();
  std::fprintf(stderr, "[dbg] suspended\n");
  if (!backend.setSoftwareBreakpoint(addr)) {
    std::fprintf(stderr, "set breakpoint thất bại\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  backend.resume();
  std::fprintf(stderr, "[dbg] resumed, waiting stop\n");

  // 4. Chờ breakpoint.
  if (!backend.waitForStop(5000)) {
    std::fprintf(stderr, "không bắt được breakpoint trong 5s\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  const auto& ev = backend.lastEvent();
  std::printf("STOP reason=%d pc=0x%llx msg=%s\n", static_cast<int>(ev.reason),
              static_cast<unsigned long long>(ev.pc), ev.message.c_str());
  if (ev.reason != a64dbg::core::StopReason::Breakpoint) {
    std::fprintf(stderr, "sự kiện không phải breakpoint\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }

  // 5. Đọc register khi đang dừng.
  a64dbg::core::RegisterState rs;
  if (!backend.readRegisterState(rs)) {
    std::fprintf(stderr, "đọc register thất bại\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  const auto pc = rs.r[static_cast<int>(a64dbg::core::Reg::PC)];
  const auto sp = rs.r[static_cast<int>(a64dbg::core::Reg::SP)];
  std::printf("regs pc=0x%llx sp=0x%llx (mong đợi pc ≈ bp+4 = 0x%llx)\n",
              static_cast<unsigned long long>(pc), static_cast<unsigned long long>(sp),
              addr + 4);

  // 6. Gỡ breakpoint, rồi single-step đúng 1 lệnh (hardware SS).
  if (!backend.removeSoftwareBreakpoint(addr)) {
    std::fprintf(stderr, "remove breakpoint thất bại\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  if (!backend.stepInto()) {
    std::fprintf(stderr, "stepInto thất bại\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  if (!backend.waitForStop(5000)) {
    std::fprintf(stderr, "không bắt được single-step\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  {
    const auto& ev2 = backend.lastEvent();
    std::printf("STEP reason=%d pc=0x%llx\n", static_cast<int>(ev2.reason),
                static_cast<unsigned long long>(ev2.pc));
    if (ev2.reason != a64dbg::core::StopReason::SingleStep || ev2.pc != addr + 4) {
      std::fprintf(stderr,
                   "single-step sai: reason=%d pc=0x%llx (mong SingleStep, pc=0x%llx)\n",
                   static_cast<int>(ev2.reason), static_cast<unsigned long long>(ev2.pc),
                   addr + 4);
      backend.detach();
      kill(pid, SIGKILL);
      return 1;
    }
  }

  // 7. Continue → target phải chạy tiếp (in tick).
  backend.resume();
  char tick[64] = {0};
  if (!readLineWithTimeout(pipefd[0], tick, sizeof(tick), 5000)) {
    std::fprintf(stderr, "target không tiếp tục chạy sau step\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  std::printf("sau step nhận: %s\n", tick);

  backend.detach();
  kill(pid, SIGKILL);
  waitpid(pid, nullptr, 0);
  std::printf("TEST PASSED\n");
  return 0;
}
