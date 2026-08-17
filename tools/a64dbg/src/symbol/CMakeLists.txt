add_library(a64dbg_symbol STATIC
  SymbolProvider.h
  SymbolProvider.cpp
)

target_include_directories(a64dbg_symbol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
a64dbg_enable_warnings(a64dbg_symbol)
