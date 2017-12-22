add_library(fractal STATIC fractal.hpp fractal.cpp)
target_include_directories(fractal PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent21 advent21.cpp)
target_link_libraries(advent21 PUBLIC fractal)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(fractal_test fractal.t.cpp)
    target_link_libraries(fractal_test PUBLIC Catch fractal)
    add_test(NAME TestAdvent21 COMMAND fractal_test)
    enable_coverage(fractal)
endif()
