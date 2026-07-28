# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(EXAMPLE_NAME 00-hello-triangle-bgfx-qml)
project(vclib-render-example-${EXAMPLE_NAME})

set(HEADERS
    ../00-hello-triangle-bgfx-common/common.h
    ../00-hello-triangle-bgfx-common/hello_triangle_drawer.h
    qml_item_manager.h
)

set(SOURCES main.cpp)

set(SHADERS
    shaders/vs_vertex_shader.sc
    shaders/fs_fragment_shader.sc
    shaders/varying.def.sc
)

# QML specific resources
set(CMAKE_AUTORCC ON)

vclib_add_example(${EXAMPLE_NAME}
    MODULE render
    SOURCES ${HEADERS} ${SOURCES} qml.qrc
)

vclib_target_add_shaders(${PROJECT_NAME} ${SHADERS})

find_package(Qt6 COMPONENTS Quick REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick)
