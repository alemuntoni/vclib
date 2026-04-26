#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2026                                                    *
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
project(vclib-emscripten-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

if (NOT TARGET vclib-3rd-emscripten)
    message(FATAL_ERROR
        "vclib-emscripten examples require the Emscripten toolchain. "
        "Use an emscripten CMake preset.")
endif()

# vclib_add_example(... VCLIB_MODULE render ...) always links this target.
# It is normally defined by examples/render/common, which is not included in
# the emscripten build.  A stub INTERFACE target is sufficient here.
if (NOT TARGET vclib-render-examples-common)
    add_library(vclib-render-examples-common INTERFACE)
endif()

add_subdirectory(00-hello-triangle)
add_subdirectory(01-viewer)
