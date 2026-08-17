#include "Session.h"

namespace a64dbg {

void Session::setAttached(std::uint32_t pid) {
  m_attached = true;
  m_pid = pid;
}

void Session::clear() {
  m_attached = false;
  m_pid = 0;
  m_name.clear();
}

}  // namespace a64dbg
