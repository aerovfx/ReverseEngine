find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)

add_library(a64dbg_ui STATIC
  DebuggerController.h
  DebuggerController.cpp
  DisasmView.h
  DisasmView.cpp
  DumpView.h
  DumpView.cpp
)

target_include_directories(a64dbg_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(a64dbg_ui PUBLIC Qt6::Widgets a64dbg_core a64dbg_disasm)
a64dbg_enable_warnings(a64dbg_ui)
