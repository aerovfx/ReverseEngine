cmake_minimum_required(VERSION 3.24)

project(a64dbg VERSION 0.1.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CompilerFlags)
include(FetchCapstone)

option(A64DBG_BUILD_TESTS "Build a64dbg tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default arch/deployment target when not provided by a preset.
if(APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
  set(CMAKE_OSX_ARCHITECTURES arm64)
endif()
if(APPLE AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  set(CMAKE_OSX_DEPLOYMENT_TARGET 14.0)
endif()

add_subdirectory(src/common)
add_subdirectory(src/model)
add_subdirectory(src/disasm)
add_subdirectory(src/macho)
add_subdirectory(src/core)
add_subdirectory(src/ui)
add_subdirectory(src/app)

if(A64DBG_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
