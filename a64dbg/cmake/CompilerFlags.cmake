# Áp cờ cảnh báo lên target CỦA MÌNH — không dùng add_compile_options global,
# để dependency vendored (Capstone...) không chịu -Werror của a64dbg.

function(a64dbg_enable_warnings target)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
  endif()
endfunction()
