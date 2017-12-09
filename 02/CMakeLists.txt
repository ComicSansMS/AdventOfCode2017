add_library(spreadsheeet STATIC spreadsheeet.hpp spreadsheeet.cpp)
target_include_directories(spreadsheeet PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent02 advent02.cpp)
target_link_libraries(advent02 PUBLIC spreadsheeet)

target_compile_options(advent02 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent02 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent02 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

file(COPY input DESTINATION ${PROJECT_BINARY_DIR})

if(TARGET Catch)
    add_executable(spreadsheeet_test spreadsheeet.t.cpp)
    target_link_libraries(spreadsheeet_test PUBLIC Catch spreadsheeet)
    add_test(NAME TestAdvent02 COMMAND spreadsheeet_test)
    enable_coverage(spreadsheeet)
endif()
