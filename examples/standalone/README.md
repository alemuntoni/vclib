# VCLib Standalone Examples

These are independent CMake projects that demonstrate how to use VCLib as a dependency
in your own projects. Each example is completely self-contained and can be built
separately from the main VCLib repository.

## Purpose

These examples serve two purposes:

1. **Documentation**: They show real-world usage patterns for VCLib's API
2. **Integration Testing**: They verify that VCLib can be consumed via `FetchContent`
   and that the resulting build works correctly

## Building

Each example is a standalone CMake project. To build any example:

```bash
cd examples/standalone/001-basic-mesh-creation
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
./standalone_001_basic_mesh_creation
```

## Examples List

| # | Example | Description |
|---|---------|-------------|
| 001 | `basic-mesh-creation` | Creating basic mesh structures (icosahedron, point cloud, polymesh) |
| 002 | `mesh-io` | Loading and saving meshes in various formats (OBJ, PLY, STL, OFF) |

## Using FetchContent

All examples use CMake's `FetchContent` to download VCLib at configure time:

```cmake
include(FetchContent)

# Set VCLib options BEFORE calling FetchContent_Declare
set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)

FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG        origin/main  # Or a specific tag/branch
)

FetchContent_MakeAvailable(vclib)

add_executable(my_project main.cpp)
target_link_libraries(my_project PRIVATE vclib::vclib)
```

### Configurable Options

You can control which modules are built by setting these variables before
`FetchContent_MakeAvailable()`:

| Option | Default | Description |
|--------|---------|-------------|
| `VCLIB_BUILD_MODULE_EXTERNAL` | OFF | Build external module (Embree, CGAL, etc.) |
| `VCLIB_BUILD_MODULE_RENDER` | OFF | Build render module |
| `VCLIB_BUILD_MODULE_BINDINGS` | OFF | Build Python bindings |
| `VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS` | ON | Exclude examples/tests from build |
| `VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES` | ON | Include 3rdparty IO libraries (for file I/O) |

### Specifying a Specific Version

To use a specific release version instead of the latest main branch:

```cmake
FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG        v0.1.0  # Use a specific tag
)
```

## CI Testing

These examples are automatically built and tested on all supported platforms via GitHub Actions:
- **Ubuntu** (x86_64 and ARM)
- **macOS** (Intel and Apple Silicon)
- **Windows** (x86_64)

See [StandaloneExamples.yml](../../.github/workflows/StandaloneExamples.yml) for the workflow configuration.

## License

These examples are provided under the same license as VCLib (MPL 2.0).
