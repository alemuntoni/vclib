#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
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

# This file is a configuration module, NOT a downloadable library.
# Emscripten is a compiler toolchain and must be installed via emsdk
# before invoking CMake. See docs/pages/0_installation.md for instructions.
#
# When building with the Emscripten toolchain (CMAKE_TOOLCHAIN_FILE pointing
# to Emscripten.cmake), this module sets up the WebGL2/WASM environment
# required by the bgfx backend on web targets.

if (NOT EMSCRIPTEN)
    message(WARNING
        "emscripten.cmake included but the Emscripten toolchain is not active")
    return()
endif()

message(STATUS "- Emscripten - configuring WebGL2/WASM target")

add_library(vclib-3rd-emscripten INTERFACE)

# WebGL2 is the web equivalent of OpenGL ES 3.0, required by bgfx on web.
# ALLOW_MEMORY_GROWTH lets the WASM heap grow dynamically, which is needed
# for 3D workloads that allocate GPU buffers at runtime.
target_link_options(vclib-3rd-emscripten INTERFACE
    -sUSE_WEBGL2=1
    -sMIN_WEBGL_VERSION=2
    -sMAX_WEBGL_VERSION=2
    -sALLOW_MEMORY_GROWTH=1
    -sWASM=1
)

# ccall/cwrap are the minimal JS-side runtime helpers for calling C++
# functions by name; Embind (-lembind) enables the higher-level Embind API
# used for structured JS<->C++ interop.
target_link_options(vclib-3rd-emscripten INTERFACE
    "-sEXPORTED_RUNTIME_METHODS=ccall,cwrap"
    -lembind
)

target_compile_definitions(vclib-3rd-emscripten INTERFACE
    VCLIB_WITH_EMSCRIPTEN
)

list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-emscripten)
