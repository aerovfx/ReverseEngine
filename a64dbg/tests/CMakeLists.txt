add_executable(test_capstone test_capstone.cpp)
target_link_libraries(test_capstone PRIVATE a64dbg_disasm)
a64dbg_enable_warnings(test_capstone)

add_test(NAME capstone_smoke COMMAND test_capstone)
