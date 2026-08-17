#include "Disassembler.h"

#include <capstone/capstone.h>

#include <cstdio>

namespace a64dbg {

const char* capstoneVersion() {
  static char buf[32] = {0};
  if (buf[0] == '\0') {
    // Dùng macro phiên bản release (CS_VERSION_EXTRA là số patch, vd 9 → "5.0.9").
    // cs_version() chỉ trả về API version 5.0, không phải version release đầy đủ.
    std::snprintf(buf, sizeof(buf), "%d.%d.%d", CS_VERSION_MAJOR, CS_VERSION_MINOR,
                  CS_VERSION_EXTRA);
  }
  return buf;
}

}  // namespace a64dbg
