# VCLib Release Workflow - Status and To-Do List

## 1. Current State Analysis

VCLib is a C++ library composed of several modules: `core`, `external`, `render`, and Python `bindings`.
The repository contains several GitHub Actions workflows in `.github/workflows`:
- `Core_BuildAndRunTests.yml`
- `External_BuildAndRunTests.yml`
- `Render_BuildAndRunTests.yml`
- `Bindings_BuildAndTestWheel.yml`
- `BuildDocs.yml`
- `RunClangFormat.yml`

### What is already done:
- **Testing:** The library has dedicated workflows to test the C++ modules (`core`, `external`, `render`).
- **Python Bindings:** There is an existing workflow `Bindings_BuildAndTestWheel.yml` that builds the Python wheels for various OSes and architectures (Ubuntu x86/ARM, macOS Intel/ARM, Windows).
- **Documentation:** A workflow to build documentation (`BuildDocs.yml`) is already in place.
- **FetchContent Support:** The library can be consumed via CMake's `FetchContent` (tested through standalone examples, assuming they exist/work properly).

### What is missing / needs to be done:
- **`find_package` Support:** Currently, the CMake configuration lacks the generation of `vclibConfig.cmake` and the `install(EXPORT ...)` directives. As a result, users cannot install the library and seamlessly use `find_package(vclib)` in their own CMake projects.
- **Standalone Examples for `find_package`:** There are no standalone examples that verify the `find_package` installation method.
- **Precompiled C++ Archives:** There are no workflows to build, package, and upload precompiled versions of the C++ library (core-only and full versions) across different OSes and architectures.
- **Unified Release Workflow:** There is no single `CreateRelease` workflow to orchestrate testing, artifact generation (C++ archives, Python wheels for all alive Python versions, documentation zip), and GitHub Release creation.

---

## 2. To-Do List (Ordered Small Features)

To reach the final goal of a fully automated `CreateRelease` workflow, we should proceed with the following incremental steps:

### Phase 1: CMake Configuration & `find_package` Support (Focus on Core First)
- [x] **1. Add `install(EXPORT)` to CMakeLists.txt (Core Module):** 
   - Modify the root `CMakeLists.txt` and `3rdparty/CMakeLists.txt` to conditionally export the `core` module and its 3rdparty dependencies.
   - Ensure `vclib-core` installs correctly, handling external static dependencies (like `tinygltf` and `stb`) and dynamically linking system dependencies (like TBB/Qt).
   - Create a CMake preset (e.g. `vclib-core-install`) for building and installing the standalone core module.
   - Create a `cmake/vclibConfig.cmake.in` file to configure the package for `find_package(vclib)`.
- [x] **2. Create Standalone Examples for `find_package`:**
   - Add new standalone examples in the `examples/standalone` directory that test both `find_package(vclib)` and `FetchContent`.
- [x] **3. Test `find_package` in CI:**
   - Update the GitHub Actions workflow (`StandaloneExamples.yml`) with a matrix to test both `find_package` (installing the library first) and `FetchContent`.
- [x] **4. FetchContent Support for External Module in Standalone Examples:**
   - Standalone CMake configuration updated to support testing the `external` module alongside `core`.
   - Dynamic dependency checking implemented for external standalone examples via `requirements.cmake`.
   - `StandaloneExamples.yml` workflow expanded to build and test the `external` module standalone using `FetchContent` across the OS matrix.
- [ ] **5. Extend `find_package` to External and Render Modules:**
   - Extend the installation and export logic to the `external` and `render` modules.
   - Once implemented, remove the `FATAL_ERROR` barrier in standalone examples and update the `StandaloneExamples.yml` matrix to remove the `exclude` rule for `external` + `find_package`.

### Phase 2: Expand Artifact Generation
- [ ] **6. Generalize Python Wheel Building:**
   - Update the Python bindings workflow (or prepare the logic for the release workflow) to build wheels for **all alive Python versions** (e.g., 3.9, 3.10, 3.11, 3.12, 3.13) across the required OS/arch matrix.
- [ ] **7. Create C++ Precompiled Archives Workflow:**
   - Implement the logic to build the C++ library and create install archives (.zip / .tar.gz).
   - Create two variations: **Core-Only** (just the core module) and **Full** (core + external + render).
- [ ] **8. Package Documentation:**
   - Ensure the documentation workflow can export the generated HTML documentation as a `.zip` artifact.

### Phase 3: The `CreateRelease` Workflow
- [ ] **9. Orchestrate the `CreateRelease` GitHub Action:**
   - Create a new workflow `.github/workflows/CreateRelease.yml` with a `workflow_dispatch` trigger, accepting the `version` as an input.
   - **Step 1: Run Checks:** Execute all existing tests (Core, External, Render, ClangFormat) to ensure the codebase is stable.
   - **Step 2: Build Artifacts:** 
     - Build C++ Core and Full archives.
     - Build Python wheels for all supported OSes, architectures, and Python versions.
     - Build and zip the documentation.
   - **Step 3: Draft GitHub Release:** Create a new GitHub Release corresponding to the input version tag and attach all generated artifacts (C++ archives, Python wheels, docs zip).
