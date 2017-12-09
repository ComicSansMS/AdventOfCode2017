add_library(captcha STATIC captcha.hpp captcha.cpp)
target_include_directories(captcha PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC captcha)

target_compile_options(advent01 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(advent01 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/permissive->)
target_compile_options(advent01 PUBLIC $<$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>:/std:c++latest>)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(captcha_test captcha.t.cpp)
    target_link_libraries(captcha_test PUBLIC Catch captcha)
    add_test(NAME TestAdvent01 COMMAND captcha_test)
    enable_coverage(captcha)
endif()
