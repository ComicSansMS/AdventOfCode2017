add_library(particles STATIC particles.hpp particles.cpp)
target_include_directories(particles PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent20 advent20.cpp)
target_link_libraries(advent20 PUBLIC particles)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(particles_test particles.t.cpp)
    target_link_libraries(particles_test PUBLIC Catch particles)
    add_test(NAME TestAdvent20 COMMAND particles_test)
    enable_coverage(particles)
endif()
