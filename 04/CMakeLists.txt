add_library(passphrase STATIC passphrase.hpp passphrase.cpp)
target_include_directories(passphrase PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC passphrase)

target_compile_options(advent04 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent04 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent04 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(passphrase_test passphrase.t.cpp)
    target_link_libraries(passphrase_test PUBLIC Catch passphrase)
    add_test(NAME TestAdvent04 COMMAND passphrase_test)
    enable_coverage(passphrase)
endif()
