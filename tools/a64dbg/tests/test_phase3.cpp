// Phase 3: breakpoint dance (continue giữ bp) + hardware breakpoint (bvr/bcr).
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

struct Target {
  pid_t pid = 0;
  int pipefd = -1;
  std::uint64_t addr = 0;
  std::uint64_t counterAddr = 0;
};

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

bool spawnTarget(Target& t) {
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return false;
  }
  const char* path = TARGET_PATH;
  char* argv[] = {const_cast<char*>(path), nullptr};
  posix_spawn_file_actions_t fa;
  posix_spawn_file_actions_init(&fa);
  posix_spawn_file_actions_adddup2(&fa, pipefd[1], STDOUT_FILENO);
  posix_spawn_file_actions_addclose(&fa, pipefd[0]);
  pid_t pid = 0;
  const int rc = posix_spawn(&pid, path, &fa, nullptr, argv, environ);
  posix_spawn_file_actions_destroy(&fa);
  close(pipefd[1]);
  if (rc != 0) {
    return false;
  }
  char line[128] = {0};
  if (!readLineWithTimeout(pipefd[0], line, sizeof(line), 5000)) {
    kill(pid, SIGKILL);
    return false;
  }
  unsigned long long addr = 0;
  unsigned long long counterAddr = 0;
  if (std::sscanf(line, "READY %llx %llx", &addr, &counterAddr) < 1) {
    kill(pid, SIGKILL);
    return false;
  }
  t.pid = pid;
  t.pipefd = pipefd[0];
  t.addr = addr;
  t.counterAddr = counterAddr;
  return true;
}

void cleanup(Target& t, a64dbg::core::MachBackend& b) {
  b.detach();
  kill(t.pid, SIGKILL);
  waitpid(t.pid, nullptr, 0);
  close(t.pipefd);
}

bool testBreakpointDance() {
  Target t;
  if (!spawnTarget(t)) {
    std::fprintf(stderr, "spawn thất bại\n");
    return false;
  }
  a64dbg::core::MachBackend backend;
  if (!backend.attach(static_cast<std::uint32_t>(t.pid))) {
    return false;
  }
  backend.suspend();
  backend.setSoftwareBreakpoint(t.addr);
  backend.resume();

  if (!backend.waitForStop(5000) ||
      backend.lastEvent().reason != a64dbg::core::StopReason::Breakpoint) {
    std::fprintf(stderr, "dance: hit #1 thất bại\n");
    cleanup(t, backend);
    return false;
  }
  std::printf("dance: hit #1 pc=0x%llx\n",
              static_cast<unsigned long long>(backend.lastEvent().pc));

  // Continue GIỮ breakpoint → dance phải tự khôi phục + re-patch rồi hit lại.
  backend.resume();
  if (!backend.waitForStop(5000) ||
      backend.lastEvent().reason != a64dbg::core::StopReason::Breakpoint) {
    std::fprintf(stderr, "dance: hit #2 thất bại\n");
    cleanup(t, backend);
    return false;
  }
  std::printf("dance: hit #2 pc=0x%llx\n",
              static_cast<unsigned long long>(backend.lastEvent().pc));

  backend.removeSoftwareBreakpoint(t.addr);
  backend.resume();
  cleanup(t, backend);
  return true;
}

bool testHardwareBreakpoint() {
  Target t;
  if (!spawnTarget(t)) {
    std::fprintf(stderr, "spawn thất bại\n");
    return false;
  }
  a64dbg::core::MachBackend backend;
  if (!backend.attach(static_cast<std::uint32_t>(t.pid))) {
    return false;
  }
  backend.suspend();
  if (!backend.setHardwareBreakpoint(t.addr)) {
    std::fprintf(stderr, "set hardware breakpoint thất bại\n");
    cleanup(t, backend);
    return false;
  }
  backend.resume();

  if (!backend.waitForStop(5000)) {
    std::fprintf(stderr, "hw bp: không dừng\n");
    cleanup(t, backend);
    return false;
  }
  if (backend.lastEvent().reason != a64dbg::core::StopReason::HardwareBreakpoint) {
    std::fprintf(stderr, "hw bp: reason=%d (mong HardwareBreakpoint=%d)\n",
                 static_cast<int>(backend.lastEvent().reason),
                 static_cast<int>(a64dbg::core::StopReason::HardwareBreakpoint));
    cleanup(t, backend);
    return false;
  }
  if (backend.lastEvent().pc != t.addr) {
    std::fprintf(stderr, "hw bp: pc=0x%llx (mong 0x%llx)\n",
                 static_cast<unsigned long long>(backend.lastEvent().pc),
                 static_cast<unsigned long long>(t.addr));
    cleanup(t, backend);
    return false;
  }
  std::printf("hw bp hit pc=0x%llx\n",
              static_cast<unsigned long long>(backend.lastEvent().pc));

  backend.removeHardwareBreakpoint(t.addr);
  backend.resume();
  cleanup(t, backend);
  return true;
}

bool testWatchpoint() {
  Target t;
  if (!spawnTarget(t)) {
    std::fprintf(stderr, "spawn thất bại\n");
    return false;
  }
  a64dbg::core::MachBackend backend;
  if (!backend.attach(static_cast<std::uint32_t>(t.pid))) {
    return false;
  }
  backend.suspend();
  if (!backend.setWatchpoint(t.counterAddr, 4)) {
    std::fprintf(stderr, "set watchpoint thất bại\n");
    cleanup(t, backend);
    return false;
  }
  backend.resume();

  if (!backend.waitForStop(5000)) {
    std::fprintf(stderr, "watchpoint: không dừng\n");
    cleanup(t, backend);
    return false;
  }
  if (backend.lastEvent().reason != a64dbg::core::StopReason::Watchpoint) {
    std::fprintf(stderr, "watchpoint: reason=%d (mong Watchpoint=%d)\n",
                 static_cast<int>(backend.lastEvent().reason),
                 static_cast<int>(a64dbg::core::StopReason::Watchpoint));
    cleanup(t, backend);
    return false;
  }
  std::printf("watchpoint hit pc=0x%llx\n",
              static_cast<unsigned long long>(backend.lastEvent().pc));

  backend.removeWatchpoint(t.counterAddr);
  backend.resume();
  cleanup(t, backend);
  return true;
}

}  // namespace

int main() {
  setvbuf(stdout, nullptr, _IONBF, 0);
  if (!testBreakpointDance()) {
    return 1;
  }
  if (!testHardwareBreakpoint()) {
    return 1;
  }
  if (!testWatchpoint()) {
    return 1;
  }
  std::printf("TEST PASSED\n");
  return 0;
}
