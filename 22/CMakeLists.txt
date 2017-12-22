add_library(virus STATIC virus.hpp virus.cpp)
target_include_directories(virus PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent22 advent22.cpp)
target_link_libraries(advent22 PUBLIC virus)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(virus_test virus.t.cpp)
    target_link_libraries(virus_test PUBLIC Catch virus)
    add_test(NAME TestAdvent22 COMMAND virus_test)
    enable_coverage(virus)
endif()
