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

find_package(bgfx QUIET)

set(VCLIB_BGFX_DIR ${CMAKE_CURRENT_LIST_DIR}/bgfx)

if (VCLIB_ALLOW_SYSTEM_BGFX AND bgfx_FOUND)
    message(STATUS "- bgfx - using system-provided library")

    add_library(vclib-3rd-bgfx INTERFACE)

    target_link_libraries(vclib-3rd-bgfx INTERFACE
        bgfx::bx bgfx::bgfx bgfx::bimg bgfx::bimg_decode bgfx::bimg_encode)

    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/iconfontheaders/include)
    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sdf/include)
    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb/include)
    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tinystl/include)

    # make sure that the imported targets are global
    set_target_properties(bgfx::bin2c PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::texturec PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::texturev PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::geometryc PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::geometryv PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::shaderc PROPERTIES IMPORTED_GLOBAL TRUE)

    # get bgfx include path
    get_target_property(BGFX_INCLUDE_PATH bgfx::bgfx INTERFACE_INCLUDE_DIRECTORIES)

    # for each path in BGFX_INCLUDE_PATH:
    foreach(PATH ${BGFX_INCLUDE_PATH})
        # if $PATH/bgfx_shader.sh exists
        if (EXISTS ${PATH}/bgfx_shader.sh)
            # set BGFX_SHADER_INCLUDE_PATH to $PATH/bgfx
            set (BGFX_SHADER_INCLUDE_PATH ${PATH}/bgfx)
        endif()
    endforeach(PATH ${BGFX_INCLUDE_PATH})

    set_target_properties(vclib-3rd-bgfx PROPERTIES
        BGFX_SHADER_INCLUDE_PATH ${BGFX_SHADER_INCLUDE_PATH})

    set_target_properties(vclib-3rd-bgfx PROPERTIES
        BGFX_CMAKE_SCRIPTS_PATH ${bgfx_DIR})

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-bgfx)

elseif(VCLIB_ALLOW_DOWNLOAD_BGFX)

    message(STATUS "- bgfx - using downloaded source")

    # leave the option to build bgfx examples, but set it to OFF by default
    option(BGFX_BUILD_EXAMPLES "Build bgfx examples" OFF)

    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(BGFX_OPENGL_VERSION 44)
    if (BUILD_SHARED_LIBS)
        set(BGFX_LIBRARY_TYPE SHARED CACHE STRING "bgfx library type" FORCE)
    endif()

    set(BGFX_WITH_WAYLAND ${VCLIB_RENDER_WITH_WAYLAND})
    set(BIMG_DECODE ON CACHE BOOL "" FORCE)
    set(BIMG_CUBEMAP ON CACHE BOOL "" FORCE)

    # bimg_encode contains nvtt (NVIDIA Texture Tools) which does not support
    # WebAssembly: posh.h cannot detect the WASM target CPU. Texture encoding
    # is a CPU-side offline tool and is not useful in a browser anyway.
    if (EMSCRIPTEN)
        set(BIMG_ENCODE OFF CACHE BOOL "" FORCE)
    endif()

    FetchContent_Declare(bgfx
        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake
        GIT_TAG        v1.143.9226-530
        EXCLUDE_FROM_ALL)

    FetchContent_MakeAvailable(bgfx)

    add_library(vclib-3rd-bgfx INTERFACE)

    # there are three warnings on gcc that we need to ignore
    set_property(TARGET bgfx PROPERTY COMPILE_WARNING_AS_ERROR OFF)

    if (EMSCRIPTEN)
        target_link_libraries(vclib-3rd-bgfx INTERFACE
            bx bgfx bimg bimg_decode)
    else()
        target_link_libraries(vclib-3rd-bgfx INTERFACE
            bx bgfx bimg bimg_decode bimg_encode)
    endif()

    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${bgfx_SOURCE_DIR}/bgfx/3rdparty)

    set_target_properties(vclib-3rd-bgfx PROPERTIES
        BGFX_CMAKE_SCRIPTS_PATH ${bgfx_SOURCE_DIR}/cmake)

    set_target_properties(vclib-3rd-bgfx PROPERTIES
        BGFX_SHADER_INCLUDE_PATH ${bgfx_SOURCE_DIR}/bgfx/src)

    if (EMSCRIPTEN)
        # When cross-compiling for WebAssembly, bgfx::shaderc is itself compiled
        # to WASM (shaderc.js) and runs inside Node.js with Emscripten's virtual
        # MEMFS filesystem. MEMFS does not expose the host filesystem, so shaderc
        # cannot open source files by absolute path.
        # Additionally, CMAKE_CROSSCOMPILING_EMULATOR is set to 'node' by the
        # Emscripten toolchain, so CMake would prepend 'node' to every executable
        # target used in COMMAND - even if we replaced the imported location.
        #
        # Solution: build a native shaderc (for the host) via ExternalProject_Add
        # using explicit host compilers, then store its path so bgfx_config.cmake
        # can reference it directly as a file path (bypassing the emulator logic).
        include(ExternalProject)

        find_program(VCLIB_NATIVE_C_COMPILER   NAMES gcc cc   clang   REQUIRED)
        find_program(VCLIB_NATIVE_CXX_COMPILER NAMES g++ c++ clang++ REQUIRED)

        set(_VCLIB_BGFX_HOST_SHADERC_BINDIR
            "${CMAKE_CURRENT_BINARY_DIR}/bgfx-host-shaderc")
        set(_VCLIB_BGFX_HOST_SHADERC_EXE
            "${_VCLIB_BGFX_HOST_SHADERC_BINDIR}/cmake/bgfx/shaderc")
        # bin2c is a bx utility, output goes into the bx subdirectory
        set(_VCLIB_BGFX_HOST_BIN2C_EXE
            "${_VCLIB_BGFX_HOST_SHADERC_BINDIR}/cmake/bx/bin2c")

        ExternalProject_Add(vclib-bgfx-host-shaderc
            SOURCE_DIR    "${bgfx_SOURCE_DIR}"
            BINARY_DIR    "${_VCLIB_BGFX_HOST_SHADERC_BINDIR}"
            INSTALL_COMMAND ""
            # Build shaderc and bin2c (both needed for shader/asset compilation).
            BUILD_COMMAND
                ${CMAKE_COMMAND} --build <BINARY_DIR>
                                 --target shaderc bin2c
                                 --config Release
            CMAKE_ARGS
                -DCMAKE_BUILD_TYPE=Release
                "-DCMAKE_C_COMPILER=${VCLIB_NATIVE_C_COMPILER}"
                "-DCMAKE_CXX_COMPILER=${VCLIB_NATIVE_CXX_COMPILER}"
                -DBGFX_BUILD_EXAMPLES=OFF
                -DBGFX_BUILD_TOOLS=ON
                -DBGFX_BUILD_TOOLS_SHADERC=ON
                -DBGFX_BUILD_TOOLS_GEOMETRYC=OFF
                -DBGFX_BUILD_TOOLS_TEXTUREC=OFF
                -DBGFX_BUILD_TOOLS_TEXTUREV=OFF
                -DBIMG_DECODE=OFF
                -DBIMG_ENCODE=OFF
                -DBIMG_CUBEMAP=OFF
            BUILD_BYPRODUCTS
                "${_VCLIB_BGFX_HOST_SHADERC_EXE}"
                "${_VCLIB_BGFX_HOST_BIN2C_EXE}"
        )

        message(STATUS
            "- bgfx - native shaderc/bin2c will be built at: "
            "${_VCLIB_BGFX_HOST_SHADERC_BINDIR}")

        # Store for use in bgfx_config.cmake.
        # Using a file path (not the imported target name) in COMMAND bypasses
        # CMAKE_CROSSCOMPILING_EMULATOR which would otherwise prepend 'node'.
        set_target_properties(vclib-3rd-bgfx PROPERTIES
            BGFX_NATIVE_SHADERC     "${_VCLIB_BGFX_HOST_SHADERC_EXE}"
            BGFX_NATIVE_BIN2C       "${_VCLIB_BGFX_HOST_BIN2C_EXE}"
            BGFX_NATIVE_SHADERC_DEP vclib-bgfx-host-shaderc)
    endif()

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-bgfx)
else()
    message(FATAL_ERROR
        "bgfx is required - be sure to clone recursively the vclib repository.")
endif()
