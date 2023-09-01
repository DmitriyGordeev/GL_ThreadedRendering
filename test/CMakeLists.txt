add_subdirectory(utils/)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main Catch2::Catch2)