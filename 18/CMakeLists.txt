add_library(duet_vm STATIC duet_vm.hpp duet_vm.cpp)
target_include_directories(duet_vm PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent18 advent18.cpp)
target_link_libraries(advent18 PUBLIC duet_vm)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(duet_vm_test duet_vm.t.cpp)
    target_link_libraries(duet_vm_test PUBLIC Catch duet_vm)
    add_test(NAME TestAdvent18 COMMAND duet_vm_test)
    enable_coverage(duet_vm)
endif()
