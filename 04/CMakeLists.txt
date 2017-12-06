cmake_minimum_required(VERSION 3.9)

project(advent04)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(passphrase STATIC passphrase.hpp passphrase.cpp)
target_include_directories(passphrase PUBLIC ${PROJECT_SOURCE_DIR})
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC passphrase)

target_compile_options(advent04 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent04 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent04 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

file(COPY input DESTINATION ${PROJECT_BINARY_DIR})

if(TARGET Catch)
    add_executable(passphrase_test passphrase.t.cpp)
    target_link_libraries(passphrase_test PUBLIC Catch passphrase)
    add_test(NAME TestAdvent04 COMMAND passphrase_test)
endif()
