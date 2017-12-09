add_library(passphrase STATIC passphrase.hpp passphrase.cpp)
target_include_directories(passphrase PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC passphrase)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(passphrase_test passphrase.t.cpp)
    target_link_libraries(passphrase_test PUBLIC Catch passphrase)
    add_test(NAME TestAdvent04 COMMAND passphrase_test)
    enable_coverage(passphrase)
endif()
