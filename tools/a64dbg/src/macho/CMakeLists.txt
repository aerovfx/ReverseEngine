add_library(a64dbg_macho STATIC
  MachOParser.h
  MachOParser.cpp
)

target_include_directories(a64dbg_macho PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
a64dbg_enable_warnings(a64dbg_macho)
