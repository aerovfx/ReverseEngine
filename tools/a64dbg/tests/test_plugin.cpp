// Phase 6: PluginHost nạp plugin qua dlopen, dispatch sự kiện, plugin đếm callback.
#include "PluginHost.h"
#include "dbg_sdk.h"

#include <cstdio>

int main() {
  setvbuf(stdout, nullptr, _IONBF, 0);

  a64dbg::plugin::PluginHost host(
      [](std::uint64_t, void*, std::size_t) { return true; },   // reader stub
      [](std::uint64_t, const void*, std::size_t) { return true; },  // writer stub
      [](std::uint64_t*, std::size_t) { return true; });        // reg reader stub

  if (!host.load(PLUGIN_PATH)) {
    std::fprintf(stderr, "load plugin thất bại\n");
    return 1;
  }
  std::printf("loaded: %s\n", host.plugins().empty() ? "?" : host.plugins()[0].c_str());

  dbg_event e{};
  e.type = DBG_EVENT_BREAKPOINT;
  e.pc = 0x1234;
  host.notifyEvent(e);
  host.notifyEvent(e);
  host.notifyEvent(e);

  using GetCount = int (*)();
  auto getCount = reinterpret_cast<GetCount>(host.symbol("get_event_count"));
  if (getCount == nullptr) {
    std::fprintf(stderr, "thiếu symbol get_event_count\n");
    return 1;
  }
  const int count = getCount();
  std::printf("event count = %d\n", count);
  if (count != 3) {
    std::fprintf(stderr, "callback sai: %d (mong 3)\n", count);
    return 1;
  }

  host.unloadAll();
  std::printf("TEST PASSED\n");
  return 0;
}
