# VCLib Release Workflow - Analysis & Roadmap

## Repository Analysis Summary

### Current State

**Project**: VCLib (Visual Computing Library) - C++20 multi-platform Mesh and Geometry Processing library  
**Current Version**: 0.0.1  
**License**: Mozilla Public License 2.0  
**CMake Minimum Version**: 3.24  

---

## 1. Current CI/CD Infrastructure

### Existing Workflows (`.github/workflows/`)

| Workflow | Trigger | Platforms | Description |
|----------|---------|-----------|-------------|
| `Core_BuildAndRunTests.yml` | push, PR | Ubuntu, ARM Ubuntu, macOS (Intel), macOS (Apple Silicon), Windows | Builds core module + runs tests |
| `External_BuildAndRunTests.yml` | push, PR | Same 5 platforms | Builds external module (Embree, CGAL, etc.) + runs tests |
| `Render_BuildAndRunTests.yml` | push, PR | Same 5 platforms × 2 backends (bgfx, opengl2) | Builds render module + attempts tests (currently commented out) |
| `Bindings_BuildAndTestWheel.yml` | push, PR, workflow_dispatch | Same 5 platforms | Builds Python wheel + runs Python examples |
| `BuildDocs.yml` | workflow_dispatch only | Ubuntu 24.04 | Builds Doxygen docs + deploys to personal GitHub Pages |
| `RunClangFormat.yml` | Schedule (daily) + workflow_dispatch | Ubuntu latest | Runs clang-format linter daily |

### CMake Presets (`CMakePresets.json`)

The project has well-organized presets:
- **Module-specific**: `vclib-core-debug/release`, `vclib-external-debug/release`, `vclib-render-bgfx/opengl2-debug/release`
- **CI presets**: `vclib-core-ci`, `vclib-external-ci`, `vclib-render-*-ci` (with tests included)
- **All-modules**: `vclib-all-modules-debug/release`, `vclib-all-modules-eat-debug/release`
- **Python wheel**: `vclib-python-wheel`

---

## 2. What's Already Working

### ✅ CMake FetchContent Integration
- The library is designed to work with `FetchContent` (documented in installation docs)
- All 3rdparty dependencies are fetched automatically via `FetchContent` or submodule mechanism
- Examples exist that demonstrate library usage

### ✅ Module Structure
- **Core**: Header-only library with mesh data structures and algorithms
- **External**: Algorithms using external libraries (Embree, CGAL, libigl, VCG, PoissonRecon)
- **Render**: Rendering library with BGFX and OpenGL2 backends, GLFW and Qt window managers
- **Bindings**: Python bindings via pybind11

### ✅ Build System
- CMake presets for all configurations
- Cross-platform support (Linux, macOS, Windows)
- ARM support (Ubuntu ARM, macOS Apple Silicon)
- Ccache integration in CI
- Python wheel building via `setup.py`

### ✅ Version Management
- Single `VERSION` file as source of truth
- [`update_version.py`](scripts/update_version.py:1) script for version updates
- Auto-generated `version.h` from template
- Version propagated to CMake and Python

### ✅ Testing
- Core tests with Catch2 framework
- Tests organized by module (core, external, render)
- Header-only test mode available

---

## 3. What's Missing (Gaps Analysis)

### 🔴 Critical Gaps for Release Workflow

#### 3.1 `find_package` Support - **TOTALLY ABSENT**
- **Current state**: No `*-Config.cmake`, `*ConfigVersion.cmake`, or `cmake_install.cmake` files are generated
- **Impact**: Users cannot use `find_package(vclib)` after installation
- **Only option**: `FetchContent` or `add_subdirectory`
- **Required files**:
  - `vclibConfig.cmake` - Main config file
  - `vclibConfigVersion.cmake` - Version compatibility
  - Export targets in install manifest

#### 3.2 CMake Install Export Configuration
- **Current state**: Modules have basic `install()` directives but no export set
- **Missing**:
  - `install(EXPORT ...)` commands
  - Export target properties (include directories, link dependencies)
  - Version compatibility macros

#### 3.3 Standalone CMake Examples
- **Current state**: Examples depend on the vclib build tree structure
- **Required**: Independent example projects that:
  - Use `FetchContent` to pull vclib
  - Use `find_package(vclib)` to find installed vclib
  - Can be built/installed completely independently

#### 3.4 Release Artifacts Generation
- **Current state**: CI builds and tests but produces no release artifacts
- **Required artifacts per platform/architecture**:
  - Archive with compiled library (core-only and full variants)
  - Python wheels for each "viva" Python version
  - Documentation zip
  - (Optional) Source tarball

### 🟡 Important Gaps

#### 3.5 CMake Package Configuration Files in 3rdparty
- **Current state**: `3rdparty/CMakeLists.txt` sets `CMAKE_MODULE_PATH` but only for local find scripts
- **Impact**: No `.cmake` config files are installed with the library

#### 3.6 Render Tests Disabled
- **Current state**: Test execution is commented out in `Render_BuildAndRunTests.yml:54-58`
- **Reason**: Likely requires display/GUI infrastructure not available in CI

#### 3.7 Python Wheel Dependencies
- **Current state**: `setup.py` hardcodes `pyqt6==6.8` as a dependency
- **Impact**: Forces PyQt6 even if users only need core functionality

#### 3.8 No Changelog / Release Notes
- **Current state**: No `CHANGELOG.md` or similar file
- **Standard practice**: Track changes between releases

#### 3.9 No Signed Tags Workflow
- **Current state**: Version script suggests tagging but no GPG signing workflow
- **Best practice**: Sign release tags for authenticity

---

## 4. Proposed Release Workflow Architecture

### 4.1 New Workflow: `CreateRelease.yml`

```
Trigger: Manual (workflow_dispatch)
Input: library_version (string, required)

Jobs:
├── build-and-test-all          # Run all existing CI builds/tests
├── generate-release-artifacts  # Create release packages
├── build-python-wheels         # Build wheels for each Python version
├── build-docs                  # Generate documentation package
└── create-github-release       # Create GitHub Release with artifacts
```

### 4.2 Release Artifacts Structure

```
vclib-v0.1.0/
├── core/
│   ├── linux-x86_64/
│   │   ├── vclib-core-0.1.0-core-only.tar.gz
│   │   └── vclib-core-0.1.0-full.tar.gz
│   ├── linux-aarch64/
│   │   ├── vclib-core-0.1.0-core-only.tar.gz
│   │   └── vclib-core-0.1.0-full.tar.gz
│   ├── macos-x86_64/
│   │   ├── vclib-core-0.1.0-core-only.tar.gz
│   │   └── vclib-core-0.1.0-full.tar.gz
│   ├── macos-arm64/
│   │   ├── vclib-core-0.1.0-core-only.tar.gz
│   │   └── vclib-core-0.1.0-full.tar.gz
│   └── windows-x86_64/
│       ├── vclib-core-0.1.0-core-only.zip
│       └── vclib-core-0.1.0-full.zip
├── python-wheels/
│   ├── vclib-0.1.0-cp310-cp310-linux_x86_64.whl
│   ├── vclib-0.1.0-cp311-cp311-linux_x86_64.whl
│   ├── vclib-0.1.0-cp312-cp312-linux_x86_64.whl
│   └── ... (one per OS/arch/Python version combo)
├── documentation/
│   └── vclib-docs-0.1.0.zip
└── development-sources/
    └── vclib-0.1.0-source.tar.gz
```

---

## 5. Ordered Implementation Roadmap

### Phase 1: Foundation - CMake Install & find_package Support

#### Task 1.1: Create CMake Export Configuration Files
- [ ] Create `cmake/vclibConfig.cmake.in` template
- [ ] Create `cmake/vclibConfigVersion.cmake.in` template
- [ ] Add `install(EXPORT ...)` to each module's CMakeLists.txt
- [ ] Configure export set with proper target properties

#### Task 1.2: Test find_package with In-Tree Examples
- [ ] Create test examples in `examples/standalone/find_package/`
- [ ] These examples should use `find_package(vclib REQUIRED)` 
- [ ] Add CI workflow to test these examples after installation

#### Task 1.3: Create Standalone FetchContent Examples
- [ ] Create examples in `examples/standalone/fetch_content/`
- [ ] These examples should use `FetchContent_Declare(vclib ...)`
- [ ] Test that they configure and build independently

#### Task 1.4: Add Standalone Examples CI Workflow
- [ ] Create `.github/workflows/StandaloneExamples.yml`
- [ ] Test both `find_package` and `FetchContent` approaches
- [ ] Run on all supported platforms

---

### Phase 2: Release Artifacts Infrastructure

#### Task 2.1: Define Archive Structure for Compiled Libraries
- [ ] Determine what goes in "core-only" vs "full" archives
- [ ] Create CMake logic to package installed files
- [ ] Test packaging on all platforms

#### Task 2.2: Create Release Packaging Script/Workflow
- [ ] Create `scripts/package_release.py` or equivalent
- [ ] Define archive naming conventions
- [ ] Handle platform-specific differences (DLLs on Windows, etc.)

#### Task 2.3: Configure Python Wheel Versions Matrix
- [ ] Decide which Python versions to support ("viva" = supported)
- [ ] Update wheel workflow to build for multiple Python versions
- [ ] Test wheel installation on all platforms

---

### Phase 3: Release Workflow Integration

#### Task 3.1: Create CreateRelease.yml Workflow
- [ ] Define `workflow_dispatch` input for version
- [ ] Add job to run all existing builds/tests first
- [ ] Add job to generate release artifacts
- [ ] Add job to create GitHub Release with uploaded artifacts

#### Task 3.2: Version Update Integration
- [ ] Integrate `update_version.py` into release workflow
- [ ] Ensure version is updated before build starts
- [ ] Create git tag after successful release

#### Task 3.3: Documentation Packaging
- [ ] Add documentation generation to release workflow
- [ ] Package Doxygen output as zip artifact

---

### Phase 4: Polish & Best Practices

#### Task 4.1: Add CHANGELOG.md
- [ ] Create initial changelog
- [ ] Document format convention (Keep a Changelog style)
- [ ] Update process for releases

#### Task 4.2: Add CMake Package Version Testing
- [ ] Create test that verifies `find_package` version matching works
- [ ] Test REQUIRED, EXACT, and version range queries

#### Task 4.3: Add Installation Verification Tests
- [ ] Test that installed library can be used in a fresh project
- [ ] Verify headers, libraries, and CMake config are all present

#### Task 4.4: Consider Additional Artifacts
- [ ] Source tarball (without .git but with submodules)
- [ ] Pre-built documentation HTML
- [ ] Example binaries (optional)

---

## 6. Common CMake Library Best Practices to Verify/Implement

| Practice | Current Status | Action Needed |
|----------|---------------|---------------|
| `CMAKE_CXX_STANDARD` set correctly | ✅ Done (C++20) | None |
| Exported targets with proper interfaces | ❌ Missing | Task 1.1 |
| `find_package` support | ❌ Missing | Task 1.1-1.4 |
| Version compatibility file | ❌ Missing | Task 1.1 |
| Configurable install prefix | ✅ GNUInstallDirs | None |
| RPATH handling for shared libs | ⚠️ Partial (Qt only) | Review for all platforms |
| Position-independent code | ⚠️ Check | Verify for header-only |
| Exported compile definitions | ❌ Missing | Task 1.1 |
| Package config documentation | ❌ Missing | Add to docs |
| CMake package registry support (CMake 3.15+) | ❌ Missing | Consider for future |

---

## 7. Notes and Considerations

### Platform-Specific Notes
- **Windows**: DLLs need to be included in release archives; runtime DLLs (Embree, etc.) may need bundling
- **Linux**: Manylinux compatibility for wheels; RPATH configuration
- **macOS**: Universal binaries (x86_64 + arm64) consideration; dylib installation names

### Python "Viva" Versions
Need to clarify which Python versions are officially supported. Current CI tests only Python 3.12. Recommended matrix:
- Python 3.10, 3.11, 3.12, 3.13 (current)

### Render Module Tests
Tests are commented out because GUI rendering requires display infrastructure. Consider:
- Using `offscreen` rendering backend
- Using Xvfb on Linux
- Skipping render tests in CI entirely (document this decision)

---

## 8. Quick Start for First Task

To begin implementation, start with **Task 1.1**: Create CMake Export Configuration Files.

This requires:
1. Creating `cmake/vclibConfig.cmake.in`
2. Adding export install commands to module CMakeLists.txt files
3. Configuring the version file generation in the root `CMakeLists.txt`

Reference implementations:
- Eigen's `Eigen3Config.cmake`
- Catch2's `Catch2Config.cmake`
- pybind11's `pybind11Config.cmake`
