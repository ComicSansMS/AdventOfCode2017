add_library(dancing_programs STATIC dancing_programs.hpp dancing_programs.cpp)
target_include_directories(dancing_programs PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent16 advent16.cpp)
target_link_libraries(advent16 PUBLIC dancing_programs)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(dancing_programs_test dancing_programs.t.cpp)
    target_link_libraries(dancing_programs_test PUBLIC Catch dancing_programs)
    add_test(NAME TestAdvent16 COMMAND dancing_programs_test)
    enable_coverage(dancing_programs)
endif()
