// Phase 4: stepOut qua controller (temp breakpoint tại LR).
#include "DebuggerController.h"
#include "Disassembler.h"

#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>

#include <cstdio>
#include <cstring>

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

  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return 1;
  }
  const char* path = TARGET_PATH;
  char* argvArr[] = {const_cast<char*>(path), nullptr};
  posix_spawn_file_actions_t fa;
  posix_spawn_file_actions_init(&fa);
  posix_spawn_file_actions_adddup2(&fa, pipefd[1], STDOUT_FILENO);
  posix_spawn_file_actions_addclose(&fa, pipefd[0]);
  pid_t pid = 0;
  const int rc = posix_spawn(&pid, path, &fa, nullptr, argvArr, environ);
  posix_spawn_file_actions_destroy(&fa);
  close(pipefd[1]);
  if (rc != 0) {
    return 1;
  }

  char line[128] = {0};
  if (!readLineWithTimeout(pipefd[0], line, sizeof(line), 5000)) {
    kill(pid, SIGKILL);
    return 1;
  }
  unsigned long long bpAddr = 0;
  if (std::sscanf(line, "READY %llx", &bpAddr) != 1) {
    kill(pid, SIGKILL);
    return 1;
  }

  a64dbg::DebuggerController controller;

  // helper: chờ stopped() với timeout.
  auto waitStop = [&](int ms) {
    bool got = false;
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(&controller, &a64dbg::DebuggerController::stopped, [&] {
      got = true;
      loop.quit();
    });
    timer.start(ms);
    loop.exec();
    return got;
  };

  if (!controller.attach(static_cast<int>(pid))) {
    kill(pid, SIGKILL);
    return 1;
  }
  controller.setBreakpoint(bpAddr);
  controller.runContinue();
  if (!waitStop(5000)) {
    std::fprintf(stderr, "không dừng ở breakpoint\n");
    return 1;
  }

  a64dbg::core::RegisterState rs;
  controller.readRegisters(rs);
  const std::uint64_t pc1 = rs.r[static_cast<int>(a64dbg::core::Reg::PC)];
  if (pc1 != bpAddr) {
    std::fprintf(stderr, "PC sau breakpoint sai: 0x%llx\n",
                 static_cast<unsigned long long>(pc1));
    return 1;
  }

  // Step out: phải chạy hết bp_here và dừng ở địa chỉ trả về (LR).
  controller.stepOut();
  if (!waitStop(5000)) {
    std::fprintf(stderr, "step out không dừng\n");
    return 1;
  }
  controller.readRegisters(rs);
  const std::uint64_t pc2 = rs.r[static_cast<int>(a64dbg::core::Reg::PC)];
  std::printf("step out: 0x%llx -> 0x%llx\n", static_cast<unsigned long long>(pc1),
              static_cast<unsigned long long>(pc2));
  if (pc2 == pc1) {
    std::fprintf(stderr, "step out không rời khỏi bp_here\n");
    return 1;
  }

  controller.detach();
  kill(pid, SIGKILL);
  waitpid(pid, nullptr, 0);
  std::printf("TEST PASSED\n");
  return 0;
}
