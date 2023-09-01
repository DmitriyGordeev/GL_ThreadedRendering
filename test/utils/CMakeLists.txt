add_executable(test_IO test_IO.cpp
        ../../utils/IO.cpp
        ../../utils/Logger.cpp)
target_link_libraries(test_IO Catch2::Catch2)

add_executable(test_Logger test_Logger.cpp
        ../../utils/Logger.cpp)
target_link_libraries(test_Logger Catch2::Catch2)