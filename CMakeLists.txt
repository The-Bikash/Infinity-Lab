cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(InfinityLab VERSION 1.0)

# Set C++ standard (adjust if needed)
set(CMAKE_CXX_STANDARD 20)

# Define the include directory for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Define where to find source files (src directory)
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add the library target (static library)
add_library(InfinityLab STATIC ${SOURCES})

# Optional: Set optimization flags (release mode)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Enable compile_commands.json generation for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the test folder as a subdirectory
add_subdirectory(tests)

# Add the example folder as a subdirectory
#add_subdirectory(examples)

# Optional: Specify test settings if using CTest
enable_testing()
