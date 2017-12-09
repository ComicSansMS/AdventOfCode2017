add_library(register_vm STATIC register_vm.hpp register_vm.cpp)
target_include_directories(register_vm PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent08 advent08.cpp)
target_link_libraries(advent08 PUBLIC register_vm)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(register_vm_test register_vm.t.cpp)
    target_link_libraries(register_vm_test PUBLIC Catch register_vm)
    add_test(NAME TestAdvent07 COMMAND register_vm_test)
    enable_coverage(register_vm)
endif()
