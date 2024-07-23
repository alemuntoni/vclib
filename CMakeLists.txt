#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2024                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)
project(vclib)

### Common settings
include(cmake/vclib_common_settings.cmake)

### Options
option(VCLIB_BUILD_EXAMPLES "Build VCLib examples" ON)
option(VCLIB_BUILD_TESTS "Build VCLib tests" ON)

option(VCLIB_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)

option(VCLIB_TESTS_AND_EXAMPLES_HEADER_ONLY
    "If true, the examples and tests of VCLib will be built just setting the
    INCLUDE_PATH of the library (no cmake targets)." OFF)

option(VCLIB_BUILD_CXX20_MODULES "Use VCLib with C++20 Modules" OFF)

if (VCLIB_BUILD_CXX20_MODULES)
    cmake_minimum_required(VERSION 3.28)
    message(STATUS "VCLib: using C++20 Modules")
endif()

# external libraries
message(STATUS "VCLib: external libraries")
add_subdirectory(external)

### vclib target
set(VCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

file(GLOB_RECURSE MODULES CONFIGURE_DEPENDS "src/*.ixx")
list(FILTER MODULES EXCLUDE REGEX "src/vclib/ext/.*" )

if (TARGET vclib-external-vcg)
    file(GLOB_RECURSE VCG_MODULES CONFIGURE_DEPENDS "src/vclib/ext/vcg/*.ixx")

    list(APPEND MODULES
        src/vclib/ext/vcg.ixx
        ${VCG_MODULES}
    )
endif()

set(VCLIB_TARGET_LINK_TYPE INTERFACE) # header only library

if (VCLIB_BUILD_CXX20_MODULES)
    add_library(vclib)

    set(VCLIB_TARGET_LINK_TYPE PUBLIC)

    target_sources(vclib
      PUBLIC
        FILE_SET CXX_MODULES FILES ${MODULES}
    )

    target_compile_definitions(vclib PUBLIC VCLIB_WITH_MODULES)

    if (MSVC)
        target_compile_options(vclib PUBLIC "/bigobj")
    endif()

    # avoid warnings when using modules
    # todo: remove this when modules are and header units stable and supported by all compilers
    if (MSVC)
        target_compile_options(vclib PRIVATE "/wd5244;")
    else()
        target_compile_options(vclib PRIVATE -Wno-include-angled-in-module-purview)
    endif()
else()
    add_library(vclib INTERFACE ${HEADERS})
endif()

target_include_directories(vclib ${VCLIB_TARGET_LINK_TYPE} ${VCLIB_INCLUDE_DIR})

set_target_properties(vclib PROPERTIES
    VCLIB_INCLUDE_DIR ${VCLIB_INCLUDE_DIR})

target_link_libraries(vclib ${VCLIB_TARGET_LINK_TYPE} ${VCLIB_EXTERNAL_LIBRARIES})

target_sources(vclib PRIVATE ${HEADERS})

if (MSVC)
    target_compile_options(vclib INTERFACE "/bigobj")
endif()

### Examples and Tests
include(cmake/examples_and_tests.cmake)

set(VCLIB_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(VCLIB_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

if (VCLIB_BUILD_EXAMPLES)
    set_target_properties(vclib PROPERTIES
        VCLIB_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR})

    add_subdirectory(${VCLIB_EXAMPLES_DIR})
endif()

if (VCLIB_BUILD_TESTS)
    include(CTest)
    set_target_properties(vclib PROPERTIES
        VCLIB_TESTS_DIR ${VCLIB_TESTS_DIR})

    add_subdirectory(${VCLIB_TESTS_DIR})
endif()

