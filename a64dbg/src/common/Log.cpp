#include "Log.h"

#include <cstdio>
#include <ctime>

namespace a64dbg {
namespace {

void log(const char* level, const std::string& msg) {
  std::time_t now = std::time(nullptr);
  char ts[32] = {0};
  std::strftime(ts, sizeof(ts), "%H:%M:%S", std::localtime(&now));
  std::fprintf(stderr, "[%s] %s: %s\n", ts, level, msg.c_str());
}

}  // namespace

void logInfo(const std::string& msg) { log("INFO", msg); }
void logError(const std::string& msg) { log("ERROR", msg); }

}  // namespace a64dbg
