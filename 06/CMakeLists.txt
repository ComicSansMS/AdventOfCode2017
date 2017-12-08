cmake_minimum_required(VERSION 3.9)

project(advent06)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(memory_banks STATIC memory_banks.hpp memory_banks.cpp)
target_include_directories(memory_banks PUBLIC ${PROJECT_SOURCE_DIR})
add_executable(advent06 advent06.cpp)
target_link_libraries(advent06 PUBLIC memory_banks)

target_compile_options(advent06 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent06 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent06 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

file(COPY input DESTINATION ${PROJECT_BINARY_DIR})

if(TARGET Catch)
    add_executable(memory_banks_test memory_banks.t.cpp)
    target_link_libraries(memory_banks_test PUBLIC Catch memory_banks)
    add_test(NAME TestAdvent06 COMMAND memory_banks_test)
endif()
