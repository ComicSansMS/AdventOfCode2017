cmake_minimum_required(VERSION 3.9)

project(advent03)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(spiralsquare STATIC spiralsquare.hpp spiralsquare.cpp)
target_include_directories(spiralsquare PUBLIC ${PROJECT_SOURCE_DIR})
add_executable(advent03 advent03.cpp)
target_link_libraries(advent03 PUBLIC spiralsquare)

target_compile_options(advent03 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent03 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent03 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

if(TARGET Catch)
    add_executable(spiralsquare_test spiralsquare.t.cpp)
    target_link_libraries(spiralsquare_test PUBLIC Catch spiralsquare)
    add_test(NAME TestAdvent03 COMMAND spiralsquare_test)
endif()
