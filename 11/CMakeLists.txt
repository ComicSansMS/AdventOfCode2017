add_library(hexgrid STATIC hexgrid.hpp hexgrid.cpp)
target_include_directories(hexgrid PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent11 advent11.cpp)
target_link_libraries(advent11 PUBLIC hexgrid)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(hexgrid_test hexgrid.t.cpp)
    target_link_libraries(hexgrid_test PUBLIC Catch hexgrid)
    add_test(NAME TestAdvent11 COMMAND hexgrid_test)
    enable_coverage(hexgrid)
endif()
