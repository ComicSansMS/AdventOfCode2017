add_library(program_tree STATIC program_tree.hpp program_tree.cpp)
target_include_directories(program_tree PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent07 advent07.cpp)
target_link_libraries(advent07 PUBLIC program_tree)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(program_tree_test program_tree.t.cpp)
    target_link_libraries(program_tree_test PUBLIC Catch program_tree)
    add_test(NAME TestAdvent07 COMMAND program_tree_test)
    enable_coverage(program_tree)
endif()
