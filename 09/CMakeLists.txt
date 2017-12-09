add_library(garbage_stream STATIC garbage_stream.hpp garbage_stream.cpp)
target_include_directories(garbage_stream PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent09 advent09.cpp)
target_link_libraries(advent09 PUBLIC garbage_stream)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(garbage_stream_test garbage_stream.t.cpp)
    target_link_libraries(garbage_stream_test PUBLIC Catch garbage_stream)
    add_test(NAME TestAdvent09 COMMAND garbage_stream_test)
    enable_coverage(garbage_stream)
endif()
