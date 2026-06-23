#############################################################################
# VCLib Standalone Example 001: Basic Mesh Creation
#
# This is a standalone CMake project that demonstrates how to use VCLib
# via FetchContent. It creates basic mesh structures (icosahedron, cube).
#
# To build this example independently:
#   mkdir build && cd build
#   cmake .. -DCMAKE_BUILD_TYPE=Release
#   cmake --build . --config Release
#############################################################################

cmake_minimum_required(VERSION 3.24)
project(standalone-001-basic-mesh-creation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable FetchContent to download VCLib at configure time
include(FetchContent)

# Set VCLib options BEFORE calling FetchContent_Declare
set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)
set(VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS ON CACHE STRING "" FORCE)

FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG        origin/main
)

FetchContent_MakeAvailable(vclib)

# Create the standalone executable
add_executable(${PROJECT_NAME} main.cpp)

# Link with VCLib - this will automatically pull in all dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE vclib::vclib)

# Print configuration info
message(STATUS "VCLib Standalone Example 001: Basic Mesh Creation")
message(STATUS "  VCLib source: ${vclib_SOURCE_DIR}")
message(STATUS "  VCLib binary: ${vclib_BINARY_DIR}")
