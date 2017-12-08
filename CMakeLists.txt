cmake_minimum_required(VERSION 3.9)

project(AdventOfCode2017 CXX)

find_path(CATCH_INCLUDE_DIR NAMES catch.hpp PATHS ${PROJECT_SOURCE_DIR}/external/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
target_compile_definitions(Catch INTERFACE $<$<CXX_COMPILER_ID:MSVC>:_SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING>)

enable_testing()

add_subdirectory(01)
add_subdirectory(02)
add_subdirectory(03)
add_subdirectory(04)
add_subdirectory(05)
add_subdirectory(06)
add_subdirectory(07)
