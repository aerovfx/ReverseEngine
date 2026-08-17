// Phase 5: stack walk + memory search + patch bytes.
#include "DebuggerController.h"

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
  setvbuf(stdout, nullptr, _IONBF, 0);
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
  unsigned long long counterAddr = 0;
  if (std::sscanf(line, "READY %llx %llx", &bpAddr, &counterAddr) < 1) {
    kill(pid, SIGKILL);
    return 1;
  }

  a64dbg::DebuggerController controller;
  if (!controller.attach(static_cast<int>(pid))) {
    kill(pid, SIGKILL);
    return 1;
  }
  std::fprintf(stderr, "[dbg] attached, start search\n");

  // 1) Memory search: đọc 8 byte đầu của bp_here rồi tìm trong vùng quanh bpAddr.
  std::vector<std::uint8_t> code;
  if (!controller.readMemory(bpAddr, 8, code)) {
    std::fprintf(stderr, "đọc code thất bại\n");
    return 1;
  }
  const auto hits = controller.searchBytes(bpAddr, 0x2000, code);
  bool foundBp = false;
  for (const auto h : hits) {
    if (h == bpAddr) {
      foundBp = true;
    }
  }
  std::printf("search: %zu hit, bpAddr=%d\n", hits.size(), foundBp ? 1 : 0);
  std::fprintf(stderr, "[dbg] search done\n");
  if (!foundBp) {
    std::fprintf(stderr, "search không tìm thấy bpAddr\n");
    return 1;
  }

  // 2) Patch bytes: ghi vào g_counter rồi đọc lại.
  std::vector<std::uint8_t> orig;
  if (!controller.readMemory(counterAddr, 4, orig)) {
    std::fprintf(stderr, "đọc g_counter thất bại\n");
    return 1;
  }
  const std::vector<std::uint8_t> marker = {0x41, 0x42, 0x43, 0x44};
  if (!controller.patchBytes(counterAddr, marker)) {
    std::fprintf(stderr, "patch thất bại\n");
    return 1;
  }
  std::vector<std::uint8_t> back;
  controller.readMemory(counterAddr, 4, back);
  if (back != marker) {
    std::fprintf(stderr, "patch đọc lại sai\n");
    return 1;
  }
  controller.patchBytes(counterAddr, orig);  // khôi phục
  std::printf("patch: OK\n");
  std::fprintf(stderr, "[dbg] patch done, set bp + run\n");

  // 3) Stack walk: đặt bp, dừng, rồi walk.
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

  controller.setBreakpoint(bpAddr);
  controller.runContinue();
  std::fprintf(stderr, "[dbg] runContinue, waiting stop\n");
  if (!waitStop(5000)) {
    std::fprintf(stderr, "không dừng ở breakpoint\n");
    return 1;
  }
  const auto frames = controller.stackWalk(16);
  std::printf("stack: %zu frame\n", frames.size());
  for (size_t i = 0; i < frames.size() && i < 4; ++i) {
    std::printf("  fp=0x%llx ret=0x%llx\n",
                static_cast<unsigned long long>(frames[i].fp),
                static_cast<unsigned long long>(frames[i].returnAddr));
  }
  if (frames.size() < 2) {
    std::fprintf(stderr, "stack walk < 2 frame\n");
    return 1;
  }
  std::fprintf(stderr, "[dbg] stack done, detach\n");

  controller.detach();
  kill(pid, SIGKILL);
  waitpid(pid, nullptr, 0);
  std::printf("TEST PASSED\n");
  return 0;
}
