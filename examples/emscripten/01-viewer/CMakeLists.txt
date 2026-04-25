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

set(EXAMPLE_NAME 01-viewer)
project(vclib-emscripten-example-${EXAMPLE_NAME})

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp)

# creates target 'vclib-render-example-01-viewer'
vclib_add_example(${EXAMPLE_NAME}
    VCLIB_MODULE render
    SOURCES ${SOURCES})

set(TARGET_NAME vclib-render-example-${EXAMPLE_NAME})

# Retrieve the path to the vclib-render cmake directory where the HTML shell
# template lives. It is stored as a property on the vclib-render target.
get_target_property(VCLIB_RENDER_CMAKE_DIR vclib-render VCLIB_RENDER_CMAKE_DIR)

# Emscripten: output as .html using our custom shell, keep the runtime alive
# after main() returns (the requestAnimationFrame loop is the real main loop).
set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".html")
target_link_options(${TARGET_NAME} PRIVATE
    "SHELL:--shell-file ${VCLIB_RENDER_CMAKE_DIR}/emscripten_shell.html"
    -sEXIT_RUNTIME=0
    -sALLOW_MEMORY_GROWTH=1
    $<$<CONFIG:Debug>:-sASSERTIONS=2>)
