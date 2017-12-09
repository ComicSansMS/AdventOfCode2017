add_library(jumper STATIC jumper.hpp jumper.cpp)
target_include_directories(jumper PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent05 advent05.cpp)
target_link_libraries(advent05 PUBLIC jumper)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(jumper_test jumper.t.cpp)
    target_link_libraries(jumper_test PUBLIC Catch jumper)
    add_test(NAME TestAdvent05 COMMAND jumper_test)
    enable_coverage(jumper)
endif()
