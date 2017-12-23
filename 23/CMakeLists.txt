add_library(coprocessor STATIC coprocessor.hpp coprocessor.cpp)
target_include_directories(coprocessor PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent23 advent23.cpp)
target_link_libraries(advent23 PUBLIC coprocessor)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(coprocessor_test coprocessor.t.cpp)
    target_link_libraries(coprocessor_test PUBLIC Catch coprocessor)
    add_test(NAME TestAdvent23 COMMAND coprocessor_test)
    enable_coverage(coprocessor)
endif()
