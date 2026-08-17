add_library(a64dbg_core STATIC
  IDebugBackend.h
  RegisterSet.h
  MachBackend.h
  MachBackend.cpp
  # MIG-generated exception server (vendor từ mach_exc.defs, Xcode SDK).
  mig/mach_exc.h
  mig/mach_excServer.c
)

target_include_directories(a64dbg_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(a64dbg_core PUBLIC a64dbg_common)
a64dbg_enable_warnings(a64dbg_core)
