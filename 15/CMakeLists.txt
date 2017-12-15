add_library(dueling_generators STATIC dueling_generators.hpp dueling_generators.cpp)
target_include_directories(dueling_generators PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent15 advent15.cpp)
target_link_libraries(advent15 PUBLIC dueling_generators)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(dueling_generators_test dueling_generators.t.cpp)
    target_link_libraries(dueling_generators_test PUBLIC Catch dueling_generators)
    add_test(NAME TestAdvent15 COMMAND dueling_generators_test)
    enable_coverage(dueling_generators)
endif()
