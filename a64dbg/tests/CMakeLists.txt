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
