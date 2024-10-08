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
project(vclib-render)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Options
set(VCLIB_RENDER_ENGINE "bgfx" CACHE STRING "Select the rendering engine")
set_property(CACHE VCLIB_RENDER_ENGINE PROPERTY STRINGS bgfx opengl2 none)

# external libraries
message(STATUS "VCLib-Render: external libraries")
add_subdirectory(external)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/render/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/vclib/render/*.cpp")

file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "assets/*")

# remove all files in ${CMAKE_CURRENT_BINARY_DIR}/include/vclib
file(GLOB_RECURSE VCLIB_RENDER_GEN_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/include/vclib/*")
foreach(FILE ${VCLIB_RENDER_GEN_HEADERS})
    file(REMOVE ${FILE})
endforeach()

# if render engine is bgfx - include sources that use bgfx
if (VCLIB_RENDER_ENGINE STREQUAL "bgfx")
    get_target_property(VCLIB_BGFX_DIR vclib-external-bgfx BGFX_DIR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)

    file(GLOB_RECURSE HEADERS_BGFX CONFIGURE_DEPENDS
        "include/vclib_bgfx/render/*.h")
    list(APPEND HEADERS ${HEADERS_BGFX})

    file(GLOB_RECURSE SOURCES_BGFX CONFIGURE_DEPENDS
        "src/vclib_bgfx/render/*.cpp")
    list(APPEND SOURCES ${SOURCES_BGFX})

    if (APPLE)
        file(GLOB_RECURSE SOURCES_MM CONFIGURE_DEPENDS
            "src/vclib_bgfx/render/*.mm")
        list(APPEND SOURCES ${SOURCES_MM})
    endif()

    file(GLOB_RECURSE SHADERS_BGFX CONFIGURE_DEPENDS
        "shaders/vclib_bgfx/render/*.sh" "shaders/vclib_bgfx/render/*.sc")
    list(APPEND SHADERS ${SHADERS_BGFX})

    # required files that use bgfx
    set(LINK_FILES
        include/vclib_bgfx/render/canvas.h
        include/vclib_bgfx/render/viewer_canvas.h
        include/vclib_bgfx/render/drawable/drawable_mesh.h)

    foreach(FILE ${LINK_FILES})
        string(REPLACE "vclib_bgfx" "vclib" TARGET_FILE ${FILE})

        get_filename_component(TARGET_DIR ${TARGET_FILE} DIRECTORY)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR})

        file(CREATE_LINK
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_FILE} COPY_ON_ERROR)
    endforeach()
endif()

# if render engine is opengl2 - include sources that use opengl2
if (VCLIB_RENDER_ENGINE STREQUAL "opengl2")
    file(GLOB_RECURSE HEADERS_OPENGL2 CONFIGURE_DEPENDS
        "include/vclib_opengl2/render/*.h")
    list(APPEND HEADERS ${HEADERS_OPENGL2})

    file(GLOB_RECURSE SOURCES_OPENGL2 CONFIGURE_DEPENDS
        "src/vclib_opengl2/render/*.cpp")
    list(APPEND SOURCES ${SOURCES_OPENGL2})

    # required files that use opengl2
    set(LINK_FILES
        include/vclib_opengl2/render/canvas.h
        include/vclib_opengl2/render/viewer_canvas.h
        include/vclib_opengl2/render/drawable/drawable_mesh.h)

    foreach(FILE ${LINK_FILES})
        string(REPLACE "vclib_opengl2" "vclib" TARGET_FILE ${FILE})

        get_filename_component(TARGET_DIR ${TARGET_FILE} DIRECTORY)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR})

        file(CREATE_LINK
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_FILE} COPY_ON_ERROR)
    endforeach()
endif()

if (VCLIB_RENDER_ENGINE STREQUAL "bgfx")
    if (TARGET vclib-external-glfw)
        file(GLOB_RECURSE HEADERS_GLFW CONFIGURE_DEPENDS
            "include/vclib/ext/glfw/*.h")
        list(APPEND HEADERS ${HEADERS_GLFW})

        file(GLOB_RECURSE SOURCES_GLFW CONFIGURE_DEPENDS "src/vclib/ext/glfw/*.cpp")
        list(APPEND SOURCES ${SOURCES_GLFW})
    endif()

    if (TARGET vclib-external-qt)
        file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/ext/qt/*.h")
        list(APPEND HEADERS ${HEADERS_QT})

        file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/ext/qt/*.cpp")
        list(APPEND SOURCES ${SOURCES_QT})

        file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/ext/qt/*.ui")
        list(APPEND FORMS ${FORMS_QT})

        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})
    endif()
endif()

if (VCLIB_RENDER_ENGINE STREQUAL "opengl2")
    list(APPEND HEADERS
        include/vclib/ext/glfw/canvas_window.h
        include/vclib/ext/glfw/event_manager_window.h
        include/vclib/ext/glfw/input.h
        )

    list(APPEND SOURCES
        src/vclib/ext/glfw/canvas_window.cpp
        src/vclib/ext/glfw/event_manager_window.cpp
        src/vclib/ext/glfw/input.cpp
        src/vclib/ext/glfw/viewer_window.cpp
        )

    if (TARGET vclib-external-qt)
        file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/ext/qt/*.h")
        list(APPEND HEADERS ${HEADERS_QT})

        file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/ext/qt/*.cpp")
        list(APPEND SOURCES ${SOURCES_QT})

        file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/ext/qt/*.ui")
        list(APPEND FORMS ${FORMS_QT})

        if (TARGET vclib-external-qglviewer)
            file(GLOB_RECURSE HEADERS_QGLVIEWER CONFIGURE_DEPENDS
                "include/vclib/ext/qglviewer/*.h")
            list(APPEND HEADERS ${HEADERS_QGLVIEWER})

            file(GLOB_RECURSE SOURCES_QGLVIEWER CONFIGURE_DEPENDS
                "src/vclib/ext/qglviewer/*.cpp")
            list(APPEND SOURCES ${SOURCES_QGLVIEWER})

            file(GLOB_RECURSE FORMS_QGLVIEWER CONFIGURE_DEPENDS
                "src/vclib/ext/qglviewer/*.ui")
            list(APPEND FORMS ${FORMS_QGLVIEWER})
        endif()

        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})
    endif()
endif()

# vclib-render target
if (WIN32)
    # on windows vclib-render must be static - needed for bgfx
    # TODO - try to make it work also on windows with SHARED...
    set(VLIB_RENDER_LIB_TYPE STATIC)
else()
    set(VLIB_RENDER_LIB_TYPE SHARED)
endif()

add_library(${PROJECT_NAME} ${VLIB_RENDER_LIB_TYPE}
    ${HEADERS} ${SOURCES} ${SHADERS} ${FORMS})

target_include_directories(${PROJECT_NAME} PUBLIC ${VCLIB_RENDER_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_SHADER_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

if (VCLIB_RENDER_ENGINE STREQUAL "bgfx")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_ENGINE_BGFX)

    build_bgfx_shaders_to_headers(${SHADERS})
    build_assets_to_headers(${ASSETS})
endif()

if (VCLIB_RENDER_ENGINE STREQUAL "opengl2")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_ENGINE_OPENGL2)
endif()

if (TARGET vclib-external-qt)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTORCC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC ON)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${VCLIB_RENDER_EXTERNAL_LIBRARIES} vclib-core)
target_include_directories(${PROJECT_NAME} PRIVATE ${VCLIB_RENDER_INCLUDE_DIR})

### Examples

if (VCLIB_BUILD_CORE_EXAMPLES)
    include(cmake/examples.cmake)
    get_target_property(VCLIB_EXAMPLES_DIR vclib-core VCLIB_CORE_EXAMPLES_DIR)
    add_subdirectory(examples)
endif()
