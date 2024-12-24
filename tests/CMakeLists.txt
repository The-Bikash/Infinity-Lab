# tests/CMakeLists.txt
enable_testing()

# Define a test executable
add_executable(test_infinitylab test.cpp)

# Link the test executable to the InfinityLab library
target_link_libraries(test_infinitylab InfinityLab)

# Register the test with CTest
add_test(NAME InfinityLabTest COMMAND test_infinitylab)
