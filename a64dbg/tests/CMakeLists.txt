# Fixture: tiến trình đích ARM64 để debug.
add_executable(a64dbg_target fixtures/target.c)
set_target_properties(a64dbg_target PROPERTIES MACOSX_BUNDLE FALSE)

add_executable(test_capstone test_capstone.cpp)
target_link_libraries(test_capstone PRIVATE a64dbg_disasm)
a64dbg_enable_warnings(test_capstone)
add_test(NAME capstone_smoke COMMAND test_capstone)

add_executable(test_disasm test_disasm.cpp)
target_link_libraries(test_disasm PRIVATE a64dbg_disasm)
a64dbg_enable_warnings(test_disasm)
add_test(NAME disasm COMMAND test_disasm)

add_executable(test_macho test_macho.cpp)
target_link_libraries(test_macho PRIVATE a64dbg_macho)
target_compile_definitions(test_macho PRIVATE TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_macho)
add_test(NAME macho COMMAND test_macho)

add_executable(test_mach_backend test_mach_backend.cpp)
target_link_libraries(test_mach_backend PRIVATE a64dbg_core)
target_compile_definitions(test_mach_backend PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_mach_backend)

# test gọi task_for_pid → cần ký với entitlement debugger.
add_custom_command(TARGET test_mach_backend POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_mach_backend>"
  COMMENT "sign test_mach_backend với debugger entitlement"
  VERBATIM)

add_test(NAME mach_backend COMMAND test_mach_backend)
set_tests_properties(mach_backend PROPERTIES TIMEOUT 20)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(test_controller test_controller.cpp)
target_link_libraries(test_controller PRIVATE Qt6::Core a64dbg_ui)
target_compile_definitions(test_controller PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_controller)
add_custom_command(TARGET test_controller POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_controller>"
  COMMENT "sign test_controller với debugger entitlement"
  VERBATIM)
add_test(NAME controller COMMAND test_controller)
set_tests_properties(controller PROPERTIES TIMEOUT 20)

add_executable(test_phase3 test_phase3.cpp)
target_link_libraries(test_phase3 PRIVATE a64dbg_core)
target_compile_definitions(test_phase3 PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_phase3)
add_custom_command(TARGET test_phase3 POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_phase3>"
  COMMENT "sign test_phase3 với debugger entitlement"
  VERBATIM)
add_test(NAME phase3 COMMAND test_phase3)
set_tests_properties(phase3 PROPERTIES TIMEOUT 20)

add_executable(test_symbol test_symbol.cpp)
target_link_libraries(test_symbol PRIVATE a64dbg_core a64dbg_symbol)
target_compile_definitions(test_symbol PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_symbol)
add_custom_command(TARGET test_symbol POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_symbol>"
  COMMENT "sign test_symbol với debugger entitlement"
  VERBATIM)
add_test(NAME symbol COMMAND test_symbol)
set_tests_properties(symbol PROPERTIES TIMEOUT 20)

add_executable(test_phase4 test_phase4.cpp)
target_link_libraries(test_phase4 PRIVATE Qt6::Core a64dbg_ui)
target_compile_definitions(test_phase4 PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_phase4)
add_custom_command(TARGET test_phase4 POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_phase4>"
  COMMENT "sign test_phase4 với debugger entitlement"
  VERBATIM)
add_test(NAME phase4 COMMAND test_phase4)
set_tests_properties(phase4 PROPERTIES TIMEOUT 20)

add_executable(test_phase5 test_phase5.cpp)
target_link_libraries(test_phase5 PRIVATE Qt6::Core a64dbg_ui)
target_compile_definitions(test_phase5 PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>")
a64dbg_enable_warnings(test_phase5)
add_custom_command(TARGET test_phase5 POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_phase5>"
  COMMENT "sign test_phase5 với debugger entitlement"
  VERBATIM)
add_test(NAME phase5 COMMAND test_phase5)
set_tests_properties(phase5 PROPERTIES TIMEOUT 20)

add_executable(test_plugin test_plugin.cpp)
target_link_libraries(test_plugin PRIVATE a64dbg_plugin)
target_compile_definitions(test_plugin PRIVATE
  PLUGIN_PATH="$<TARGET_FILE:example_plugin>")
a64dbg_enable_warnings(test_plugin)
add_test(NAME plugin COMMAND test_plugin)
set_tests_properties(plugin PROPERTIES TIMEOUT 20)

add_executable(test_plugin_integration test_plugin_integration.cpp)
target_link_libraries(test_plugin_integration PRIVATE Qt6::Core a64dbg_ui)
target_compile_definitions(test_plugin_integration PRIVATE
  TARGET_PATH="$<TARGET_FILE:a64dbg_target>"
  PLUGIN_PATH="$<TARGET_FILE:example_plugin>")
a64dbg_enable_warnings(test_plugin_integration)
add_custom_command(TARGET test_plugin_integration POST_BUILD
  COMMAND codesign --force --sign - --entitlements
          "${CMAKE_SOURCE_DIR}/entitlements/debugger.entitlements"
          "$<TARGET_FILE:test_plugin_integration>"
  COMMENT "sign test_plugin_integration với debugger entitlement"
  VERBATIM)
add_test(NAME plugin_integration COMMAND test_plugin_integration)
set_tests_properties(plugin_integration PROPERTIES TIMEOUT 20)
