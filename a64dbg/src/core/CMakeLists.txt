# Phase 0: chỉ khai báo contract IDebugBackend (header-only).
# Phase 1: thêm MachBackend.cpp triển khai bằng Mach API.
add_library(a64dbg_core INTERFACE)
target_include_directories(a64dbg_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
