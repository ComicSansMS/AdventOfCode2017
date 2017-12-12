add_library(pipe_network STATIC pipe_network.hpp pipe_network.cpp)
target_include_directories(pipe_network PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent12 advent12.cpp)
target_link_libraries(advent12 PUBLIC pipe_network)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(pipe_network_test pipe_network.t.cpp)
    target_link_libraries(pipe_network_test PUBLIC Catch pipe_network)
    add_test(NAME TestAdvent12 COMMAND pipe_network_test)
    enable_coverage(pipe_network)
endif()
