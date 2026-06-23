#############################################################################
# VCLib Standalone Example 002: Mesh I/O
#
# This is a standalone CMake project that demonstrates how to load and save
# meshes using VCLib via FetchContent.
#
# To build this example independently:
#   mkdir build && cd build
#   cmake .. -DCMAKE_BUILD_TYPE=Release
#   cmake --build . --config Release
#
# Note: This example requires 3rdparty IO libraries (enabled by default).
#############################################################################

cmake_minimum_required(VERSION 3.24)
project(standalone-002-mesh-io LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Enable IO libraries for loading/saving mesh files
set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)
set(VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS ON CACHE STRING "" FORCE)
# Keep 3rdparty IO libraries enabled (default is ON)

FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG        origin/main
)

FetchContent_MakeAvailable(vclib)

# Define ASSETS_PATH macro pointing to the repository's assets directory
set(ASSETS_PATH "${vclib_SOURCE_DIR}/assets")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vclib::vclib)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_PATH="${ASSETS_PATH}")

message(STATUS "VCLib Standalone Example 002: Mesh I/O")
message(STATUS "  VCLib source: ${vclib_SOURCE_DIR}")
message(STATUS "  Assets path: ${ASSETS_PATH}")
