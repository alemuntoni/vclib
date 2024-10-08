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

### Options

option(VCLIB_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)
# Optional modules build options
option(VCLIB_BUILD_RENDER_MODULE "Build the render module" OFF)

### Common settings
include(cmake/vclib_common_settings.cmake)

### Examples and Tests
include(cmake/examples_and_tests.cmake)

add_library(vclib INTERFACE)

add_subdirectory(vclib/core)
target_link_libraries(vclib INTERFACE vclib-core)

if (VCLIB_BUILD_RENDER_MODULE)
    add_subdirectory(vclib/render)
    target_link_libraries(vclib INTERFACE vclib-render)
endif()
