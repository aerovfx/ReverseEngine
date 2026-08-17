add_library(a64dbg_common STATIC
  Log.h
  Log.cpp
)

# Sinh Version.h từ phiên bản project.
configure_file(Version.h.in ${CMAKE_CURRENT_BINARY_DIR}/Version.h @ONLY)

target_include_directories(a64dbg_common PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
a64dbg_enable_warnings(a64dbg_common)
