add_library(knot_hash STATIC knot_hash.hpp knot_hash.cpp)
target_include_directories(knot_hash PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent10 advent10.cpp)
target_link_libraries(advent10 PUBLIC knot_hash)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(knot_hash_test knot_hash.t.cpp)
    target_link_libraries(knot_hash_test PUBLIC Catch knot_hash)
    add_test(NAME TestAdvent10 COMMAND knot_hash_test)
    enable_coverage(knot_hash)
endif()
