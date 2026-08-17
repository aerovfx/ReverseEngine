add_library(a64dbg_model STATIC
  Session.h
  Session.cpp
)

target_include_directories(a64dbg_model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
a64dbg_enable_warnings(a64dbg_model)
