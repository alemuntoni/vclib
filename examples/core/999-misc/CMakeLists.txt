# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-core-example-${EXAMPLE_NAME})

set(SOURCES
    types.h
    test_concepts.h
    test_navigation.h
    test_container_proxy.h
    test_const_correctness.h
    test_vertex_references.h
    test_lifetime.h
    test_deletion_compaction.h
    test_optional_components.h
    test_add_elements.h
    test_import_from.h
    main.cpp
)

vclib_add_example(
    ${EXAMPLE_NAME}
    SOURCES ${SOURCES}
    ${HEADER_ONLY_OPTION}
)

if(TARGET pybind11::pybind11)
    target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::embed)
endif()
