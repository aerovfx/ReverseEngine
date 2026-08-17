#pragma once

#include "dbg_sdk.h"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace a64dbg::plugin {

// Nạp plugin (.so/.dylib) qua dlopen và dispatch sự kiện tới callback của plugin.
class PluginHost {
public:
  using MemoryReader = std::function<bool(std::uint64_t, void*, std::size_t)>;
  using MemoryWriter = std::function<bool(std::uint64_t, const void*, std::size_t)>;
  using RegisterReader = std::function<bool(std::uint64_t*, std::size_t)>;

  PluginHost(MemoryReader reader, MemoryWriter writer, RegisterReader regReader);
  ~PluginHost();

  PluginHost(const PluginHost&) = delete;
  PluginHost& operator=(const PluginHost&) = delete;

  bool load(const std::string& path);
  void unloadAll();
  void notifyEvent(const dbg_event& e);
  const std::vector<std::string>& plugins() const { return m_pluginNames; }
  void* symbol(const std::string& name) const;  // dlsym plugin đầu tiên (cho test)

private:
  void registerCallback(void (*cb)(const dbg_event*));

  // Bridge C function pointer → thành viên (qua g_host).
  static void s_log(const char* msg);
  static int s_readMemory(std::uint64_t addr, void* buf, std::size_t len);
  static int s_writeMemory(std::uint64_t addr, const void* buf, std::size_t len);
  static int s_readRegisters(std::uint64_t* regs, std::size_t count);
  static void s_setEventCallback(void (*cb)(const dbg_event*));

  MemoryReader m_reader;
  MemoryWriter m_writer;
  RegisterReader m_regReader;
  dbg_api m_api{};
  std::vector<void*> m_handles;
  std::vector<void (*)()> m_plugstop;
  std::vector<std::string> m_pluginNames;
  std::vector<void (*)(const dbg_event*)> m_callbacks;
};

}  // namespace a64dbg::plugin
