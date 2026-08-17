// Phase 4: symbolication native — resolve địa chỉ hàm bp_here → "_bp_here".
#include "MachBackend.h"
#include "SymbolProvider.h"

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

int main() {
  setvbuf(stdout, nullptr, _IONBF, 0);

  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return 1;
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
    std::fprintf(stderr, "spawn thất bại\n");
    return 1;
  }

  char line[128] = {0};
  if (!readLineWithTimeout(pipefd[0], line, sizeof(line), 5000)) {
    std::fprintf(stderr, "không nhận READY\n");
    kill(pid, SIGKILL);
    return 1;
  }
  unsigned long long bpAddr = 0;
  if (std::sscanf(line, "READY %llx", &bpAddr) != 1) {
    std::fprintf(stderr, "dòng READY sai\n");
    kill(pid, SIGKILL);
    return 1;
  }

  a64dbg::core::MachBackend backend;
  if (!backend.attach(static_cast<std::uint32_t>(pid))) {
    std::fprintf(stderr, "attach thất bại\n");
    kill(pid, SIGKILL);
    return 1;
  }

  a64dbg::symbol::SymbolProvider provider(
      [&](std::uint64_t a, void* b, std::size_t n) { return backend.readMemory(a, b, n); },
      backend.taskPort());
  if (!provider.enumerate()) {
    std::fprintf(stderr, "enumerate ảnh thất bại\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  std::printf("images = %zu\n", provider.images().size());

  a64dbg::symbol::ResolvedSymbol out;
  if (!provider.resolve(bpAddr, out)) {
    std::fprintf(stderr, "resolve thất bại\n");
    backend.detach();
    kill(pid, SIGKILL);
    return 1;
  }
  std::printf("resolve 0x%llx -> %s!%s+0x%llx\n",
              static_cast<unsigned long long>(bpAddr), out.image.c_str(), out.symbol.c_str(),
              static_cast<unsigned long long>(out.offset));

  backend.detach();
  kill(pid, SIGKILL);
  waitpid(pid, nullptr, 0);

  if (out.symbol != "_bp_here" || out.offset != 0) {
    std::fprintf(stderr, "resolve sai: %s+0x%llx (mong _bp_here+0x0)\n", out.symbol.c_str(),
                 static_cast<unsigned long long>(out.offset));
    return 1;
  }
  std::printf("TEST PASSED\n");
  return 0;
}
