#include "PluginHost.h"

#include "Log.h"

#include <dlfcn.h>

namespace a64dbg::plugin {

namespace {

PluginHost* g_host = nullptr;

}  // namespace

PluginHost::PluginHost(MemoryReader reader, MemoryWriter writer, RegisterReader regReader)
    : m_reader(std::move(reader)), m_writer(std::move(writer)), m_regReader(std::move(regReader)) {
  g_host = this;
  m_api.log = &PluginHost::s_log;
  m_api.read_memory = &PluginHost::s_readMemory;
  m_api.write_memory = &PluginHost::s_writeMemory;
  m_api.read_registers = &PluginHost::s_readRegisters;
  m_api.set_event_callback = &PluginHost::s_setEventCallback;
}

PluginHost::~PluginHost() {
  unloadAll();
  if (g_host == this) {
    g_host = nullptr;
  }
}

bool PluginHost::load(const std::string& path) {
  void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
  if (handle == nullptr) {
    logError("dlopen thất bại: " + std::string(dlerror()));
    return false;
  }
  using PlugInit = void (*)(dbg_plugin_info*, const dbg_api*);
  auto pluginit = reinterpret_cast<PlugInit>(dlsym(handle, "pluginit"));
  if (pluginit == nullptr) {
    dlclose(handle);
    logError("plugin thiếu symbol pluginit");
    return false;
  }

  dbg_plugin_info info{};
  pluginit(&info, &m_api);

  m_handles.push_back(handle);
  m_pluginNames.push_back(info.name ? info.name : path);
  auto stop = reinterpret_cast<void (*)()>(dlsym(handle, "plugstop"));
  m_plugstop.push_back(stop);
  logInfo("plugin loaded: " + std::string(info.name ? info.name : path));
  return true;
}

void PluginHost::unloadAll() {
  for (std::size_t i = m_handles.size(); i > 0; --i) {
    const std::size_t idx = i - 1;
    if (m_plugstop[idx] != nullptr) {
      m_plugstop[idx]();
    }
    dlclose(m_handles[idx]);
  }
  m_handles.clear();
  m_plugstop.clear();
  m_pluginNames.clear();
  m_callbacks.clear();
}

void PluginHost::notifyEvent(const dbg_event& e) {
  for (auto cb : m_callbacks) {
    if (cb != nullptr) {
      cb(&e);
    }
  }
}

void* PluginHost::symbol(const std::string& name) const {
  if (m_handles.empty()) {
    return nullptr;
  }
  return dlsym(m_handles.front(), name.c_str());
}

void PluginHost::registerCallback(void (*cb)(const dbg_event*)) {
  if (cb != nullptr) {
    m_callbacks.push_back(cb);
  }
}

void PluginHost::s_log(const char* msg) {
  if (msg != nullptr) {
    a64dbg::logInfo(msg);
  }
}

int PluginHost::s_readMemory(std::uint64_t addr, void* buf, std::size_t len) {
  return (g_host != nullptr && g_host->m_reader && g_host->m_reader(addr, buf, len)) ? 1 : 0;
}

int PluginHost::s_writeMemory(std::uint64_t addr, const void* buf, std::size_t len) {
  return (g_host != nullptr && g_host->m_writer && g_host->m_writer(addr, buf, len)) ? 1 : 0;
}

int PluginHost::s_readRegisters(std::uint64_t* regs, std::size_t count) {
  return (g_host != nullptr && g_host->m_regReader && g_host->m_regReader(regs, count)) ? 1 : 0;
}

void PluginHost::s_setEventCallback(void (*cb)(const dbg_event*)) {
  if (g_host != nullptr) {
    g_host->registerCallback(cb);
  }
}

}  // namespace a64dbg::plugin
