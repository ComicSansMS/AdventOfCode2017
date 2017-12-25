add_library(turing_machine STATIC turing_machine.hpp turing_machine.cpp)
target_include_directories(turing_machine PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent25 advent25.cpp)
target_link_libraries(advent25 PUBLIC turing_machine)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(turing_machine_test turing_machine.t.cpp)
    target_link_libraries(turing_machine_test PUBLIC Catch turing_machine)
    add_test(NAME TestAdvent25 COMMAND turing_machine_test)
    enable_coverage(turing_machine)
endif()
