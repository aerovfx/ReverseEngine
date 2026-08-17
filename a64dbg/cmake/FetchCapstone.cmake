# Kéo Capstone (disassembly engine) về build tĩnh, ghim tag 5.0.9.
# Build "diet": chỉ bật kiến trúc a64dbg cần (ARM64 ngay, X86 để dành cho Rosetta/tương lai).

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
# KHÔNG bật diet: diet mode làm SStream không điền mnemonic/op_str (chuỗi rỗng).
set(CAPSTONE_BUILD_DIET OFF CACHE BOOL "" FORCE)
set(CAPSTONE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CAPSTONE_BUILD_CSTOOL OFF CACHE BOOL "" FORCE)
set(CAPSTONE_BUILD_CSTEST OFF CACHE BOOL "" FORCE)
set(CAPSTONE_ARCHITECTURE_DEFAULT OFF CACHE BOOL "" FORCE)
set(CAPSTONE_INSTALL OFF CACHE BOOL "" FORCE)
set(CAPSTONE_ARM64_SUPPORT ON CACHE BOOL "" FORCE)
set(CAPSTONE_X86_SUPPORT ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  capstone
  GIT_REPOSITORY https://github.com/capstone-engine/capstone.git
  GIT_TAG 5.0.9
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(capstone)

if(NOT TARGET capstone_static)
  message(FATAL_ERROR "Capstone không tạo được target 'capstone_static'")
endif()

set(A64DBG_CAPSTONE_TARGET capstone_static)
message(STATUS "a64dbg: dùng Capstone target ${A64DBG_CAPSTONE_TARGET}")
