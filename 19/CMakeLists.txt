add_library(tube_traverser STATIC tube_traverser.hpp tube_traverser.cpp)
target_include_directories(tube_traverser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent19 advent19.cpp)
target_link_libraries(advent19 PUBLIC tube_traverser)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(tube_traverser_test tube_traverser.t.cpp)
    target_link_libraries(tube_traverser_test PUBLIC Catch tube_traverser)
    add_test(NAME TestAdvent19 COMMAND tube_traverser_test)
    enable_coverage(tube_traverser)
endif()
