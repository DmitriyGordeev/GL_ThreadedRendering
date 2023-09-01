add_executable(test_IO test_IO.cpp
        ../../utils/IO.cpp
        ../../core/Logger.cpp)
target_link_libraries(test_IO Catch2::Catch2)