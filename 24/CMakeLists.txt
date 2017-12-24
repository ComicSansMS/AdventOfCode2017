add_library(bridge_builder STATIC bridge_builder.hpp bridge_builder.cpp)
target_include_directories(bridge_builder PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent24 advent24.cpp)
target_link_libraries(advent24 PUBLIC bridge_builder)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(bridge_builder_test bridge_builder.t.cpp)
    target_link_libraries(bridge_builder_test PUBLIC Catch bridge_builder)
    add_test(NAME TestAdvent24 COMMAND bridge_builder_test)
    enable_coverage(bridge_builder)
endif()
