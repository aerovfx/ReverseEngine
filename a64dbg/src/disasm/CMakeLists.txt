add_library(a64dbg_disasm STATIC
  Disassembler.h
  Disassembler.cpp
)

target_link_libraries(a64dbg_disasm PUBLIC ${A64DBG_CAPSTONE_TARGET})
target_include_directories(a64dbg_disasm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
a64dbg_enable_warnings(a64dbg_disasm)
