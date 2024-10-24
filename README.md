# Visual Computing Library

[![BuildAndRunTests](https://github.com/cnr-isti-vclab/vclib/actions/workflows/BuildAndRunTests.yml/badge.svg)](https://github.com/cnr-isti-vclab/vclib/actions/workflows/BuildAndRunTests.yml)

The Visual Computing Library is a C++20 modern, multi-platform Mesh and Geometry Processing library.

The library is divided in several modules, each one with a specific purpose:
  - **Core**: contains the basic core library, that is templated header only and contains the basic data structures and algorithms; all the other modules depend on this one.
  - **External**: contains algorithms and data structures that use external libraries, like VCG, etc.
  - **Processing**: contains algorithms for high level mesh and geometry processing tasks that can be used as black boxes plugins.
  - **Render**: contains the rendering library, that allows to render meshes using different rendering engines and different window managers and GUI libraries.

All the modules can be found in the [vclib](vclib) directory, and can be built separately.

By default, the library is built with the `Core` module only, but you can enable the other modules by setting the `VCLIB_BUILD_MODULE_<module>` CMake variable to `ON`.

## Disclaimer

I designed and developed this library mostly for personal use and to study new features of C++17 and C++20.
Most of the features of this library are poorly tested: I maintain the library during my free time.

If you appreciate this work, or you think it could be improved, let me know by opening a thread in the discussion tab!
Looking forward for your feedback!

## Install

The core module of library is header only, therefore to install it you just need to add the `vclib/core/include` directory in your
`INCLUDE_PATH`.
Otherwise, you can use the library by using the `cmake` command `add_subdirectory`, by adding the path to this repository
in your `cmake` project. CMake will create a target called `vclib`, that you can link using the `target_link_libraries` command.

To use C++20 modules, you'll need CMake to generate the `vclib` module. You can configure the project by setting manyally the option
`VCLIB_BUILD_CXX20_MODULES` to `ON`.

## Compiler support

VCLib requires recent compiler versions in order to build the newly features of the C++20 language, that have been recently 
integrated in the major compilers.

### Header Only Libary

VCLib currently builds with the following compilers:
  - GCC 11.3 (default in Ubuntu 22.04)
  - CLang 16 (default in XCode 15.0 on MacOS)
  - MSVC 19.36

There is no guarantee that old compiler versions are capable to build the library.

If you use CMake, the minimum required version is 3.24.

### Modules Library

To use C++20 modules (`import vclib;`), the following compilers are supported:
  - LLVM Clang 18 (available [here](https://apt.llvm.org/) for Ubuntu)
  - MSVC 19.36
  
Moreover, you'll need CMake 3.28 and Ninja 1.11.1.

## C++20

VCLib is designed to work with the most recent features of C++20:
  - Concepts
  - Ranges and Views
  - Modules (if enabled).
  
Due to the poorly support of the major compilers (especially GCC) to C++20 modules, right now the library is designed in order
to let the user choose to use it as a header only library or with modules. When the support for modules will be strong 
enough by major compilers, build systems and IDEs, the library will be refactored and will use only modules, avoiding whenever 
is possible the usage of `#include`.

Right now the organization of the library is not optimized for modules: 
- all the names of the library are currently exported; 
- headers code is included in module interface units (*.ixx files);
- there are no module impementation units.

## Documentation

Right now, the library is a work in progress, and therefore is a work in progress also the documentation.

The best way to learn how the library works is to look at the code in the [examples](examples), which is built at every commit and 
therefore it is guaranteed to work.

A first draft of the (not up-to-date) documentation of the library can be found [here](https://alemuntoni.github.io/vclib/).

You can build the documentation using doxygen (tested on 1.9.1 version):

```
doxygen docs/Doxyfile
```

Be sure to call doxygen from the root directory of the repository, otherwise the paths in the Doxyfile will be wrong.

The output will be placed in the `docs/_doxygen/html` directory.

## License

The library is under the [MPL2 license](LICENSE).
