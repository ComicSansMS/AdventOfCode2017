add_library(firewall STATIC firewall.hpp firewall.cpp)
target_include_directories(firewall PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent13 advent13.cpp)
target_link_libraries(advent13 PUBLIC firewall)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(firewall_test firewall.t.cpp)
    target_link_libraries(firewall_test PUBLIC Catch firewall)
    add_test(NAME TestAdvent13 COMMAND firewall_test)
    enable_coverage(firewall)
endif()
