cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# == project ==

project(
  "ChessEngine"
  VERSION 0.1.0
  DESCRIPTION "Chess Engine"
  LANGUAGES CXX
)

# == modules ==

include(InsourceGuard)
include(CompilerOptions)
include(CTest)
include(DoctestIntegration)

include_directories(include/doctest)
# == project modules ==

add_subdirectory(engine)
add_subdirectory(example)
add_subdirectory(test)
