#pragma once

#include <cstdint>
#include <string>

namespace a64dbg {

// Trạng thái phiên debug hiện tại (được mở rộng ở Phase 1 khi attach thật).
class Session {
public:
  void setAttached(std::uint32_t pid);
  void clear();

  bool isAttached() const { return m_attached; }
  std::uint32_t pid() const { return m_pid; }
  const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

private:
  bool m_attached = false;
  std::uint32_t m_pid = 0;
  std::string m_name;
};

}  // namespace a64dbg
