add_library(a64dbg_plugin STATIC
  PluginHost.h
  PluginHost.cpp
)

target_include_directories(a64dbg_plugin PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(a64dbg_plugin PUBLIC a64dbg_common)
a64dbg_enable_warnings(a64dbg_plugin)
