find_package(Qt6 REQUIRED COMPONENTS Qml)
set(CMAKE_AUTOMOC ON)

add_library(a64dbg_script STATIC
  ScriptEngine.h
  ScriptEngine.cpp
)

target_include_directories(a64dbg_script PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(a64dbg_script PUBLIC Qt6::Qml)
a64dbg_enable_warnings(a64dbg_script)
