cmake_minimum_required(VERSION 3.9)

project(advent01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(captcha STATIC captcha.hpp captcha.cpp)
target_include_directories(captcha PUBLIC ${PROJECT_SOURCE_DIR})
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC captcha)

target_compile_options(advent01 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent01 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent01 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

file(COPY input01 DESTINATION ${PROJECT_BINARY_DIR})
file(COPY input02 DESTINATION ${PROJECT_BINARY_DIR})

if(TARGET Catch)
    add_executable(captcha_test captcha.t.cpp)
    target_link_libraries(captcha_test PUBLIC Catch captcha)
    add_test(NAME TestAdvent01 COMMAND captcha_test)
endif()
