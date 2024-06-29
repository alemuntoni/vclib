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

if (WIN32 OR TARGET vclib-external-tbb OR TARGET vclib-external-pstld)
    message(STATUS "VCLib: using parallel algorithms")
else()
    message(STATUS "VCLib: not using parallel algorithms")
endif()

### vclib target
set(VCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

set(MODULES
    src/vclib.ixx
    src/vclib/algorithms.ixx
    src/vclib/concepts.ixx
    src/vclib/exceptions.ixx
    src/vclib/iterators.ixx
    src/vclib/load_save.ixx
    src/vclib/math.ixx
    src/vclib/mesh.ixx
    src/vclib/meshes.ixx
    src/vclib/misc.ixx
    src/vclib/space.ixx
    src/vclib/types.ixx
    src/vclib/views.ixx
    src/vclib/algorithms/core.ixx
    src/vclib/algorithms/mesh.ixx
    src/vclib/algorithms/core/bounding_box.ixx
    src/vclib/algorithms/core/distance.ixx
    src/vclib/algorithms/core/fitting.ixx
    src/vclib/algorithms/core/intersection.ixx
    src/vclib/algorithms/core/polygon.ixx
    src/vclib/algorithms/core/stat.ixx
    src/vclib/algorithms/mesh/bounding_box.ixx
    src/vclib/algorithms/mesh/clean.ixx
    src/vclib/algorithms/mesh/create.ixx
    src/vclib/algorithms/mesh/distance.ixx
    src/vclib/algorithms/mesh/filter.ixx
    src/vclib/algorithms/mesh/import_export.ixx
    src/vclib/algorithms/mesh/intersection.ixx
    src/vclib/algorithms/mesh/point_sampling.ixx
    src/vclib/algorithms/mesh/polygon.ixx
    src/vclib/algorithms/mesh/shuffle.ixx
    src/vclib/algorithms/mesh/smooth.ixx
    src/vclib/algorithms/mesh/sort.ixx
    src/vclib/algorithms/mesh/stat.ixx
    src/vclib/algorithms/mesh/create/cone.ixx
    src/vclib/algorithms/mesh/create/dodecahedron.ixx
    src/vclib/algorithms/mesh/create/hexahedron.ixx
    src/vclib/algorithms/mesh/create/icosahedron.ixx
    src/vclib/algorithms/mesh/create/sphere.ixx
    src/vclib/algorithms/mesh/create/tetrahedron.ixx
    src/vclib/algorithms/mesh/stat/quality.ixx
    src/vclib/algorithms/mesh/stat/selection.ixx
    src/vclib/algorithms/mesh/update.ixx
    src/vclib/algorithms/mesh/update/bounding_box.ixx
    src/vclib/algorithms/mesh/update/color.ixx
    src/vclib/algorithms/mesh/update/curvature.ixx
    src/vclib/algorithms/mesh/update/flag.ixx
    src/vclib/algorithms/mesh/update/normal.ixx
    src/vclib/algorithms/mesh/update/quality.ixx
    src/vclib/algorithms/mesh/update/selection.ixx
    src/vclib/algorithms/mesh/update/topology.ixx
    src/vclib/algorithms/mesh/update/transform.ixx
    src/vclib/concepts/const_correctness.ixx
    src/vclib/concepts/iterators.ixx
    src/vclib/concepts/mesh.ixx
    src/vclib/concepts/pointers.ixx
    src/vclib/concepts/ranges.ixx
    src/vclib/concepts/space.ixx
    src/vclib/concepts/mesh/components.ixx
    src/vclib/concepts/mesh/containers.ixx
    src/vclib/concepts/mesh/element_mesh_concept.ixx
    src/vclib/concepts/mesh/elements.ixx
    src/vclib/concepts/mesh/mesh_concept.ixx
    src/vclib/concepts/mesh/per_element.ixx
    src/vclib/concepts/mesh/per_mesh.ixx
    src/vclib/concepts/mesh/components/component.ixx
    src/vclib/concepts/mesh/containers/element_container.ixx
    src/vclib/concepts/mesh/elements/element.ixx
    src/vclib/concepts/ranges/mesh.ixx
    src/vclib/concepts/ranges/range.ixx
    src/vclib/io/file_info.ixx
    src/vclib/io/image.ixx
    src/vclib/io/mesh.ixx
    src/vclib/io/read.ixx
    src/vclib/io/write.ixx
    src/vclib/io/mesh/capability.ixx
    src/vclib/io/mesh/load.ixx
    src/vclib/io/mesh/save.ixx
    src/vclib/io/mesh/obj/capability.ixx
    src/vclib/io/mesh/obj/load.ixx
    src/vclib/io/mesh/obj/material.ixx
    src/vclib/io/mesh/obj/save.ixx
    src/vclib/io/mesh/off/capability.ixx
    src/vclib/io/mesh/off/load.ixx
    src/vclib/io/mesh/off/save.ixx
    src/vclib/io/mesh/ply/capability.ixx
    src/vclib/io/mesh/ply/detail.ixx
    src/vclib/io/mesh/ply/load.ixx
    src/vclib/io/mesh/ply/save.ixx
    src/vclib/io/mesh/stl/capability.ixx
    src/vclib/io/mesh/stl/load.ixx
    src/vclib/io/mesh/stl/save.ixx
    src/vclib/iterators/const_pointer_iterator.ixx
    src/vclib/iterators/mesh.ixx
    src/vclib/iterators/space.ixx
    src/vclib/math/base.ixx
    src/vclib/math/transform.ixx
    src/vclib/mesh/components.ixx
    src/vclib/mesh/containers.ixx
    src/vclib/mesh/elements.ixx
    src/vclib/mesh/mesh.ixx
    src/vclib/mesh/mesh_components.ixx
    src/vclib/mesh/requirements.ixx
    src/vclib/mesh/utils.ixx
    src/vclib/mesh/components/bases.ixx
    src/vclib/mesh/components/detail.ixx
    src/vclib/mesh/components/parent_mesh_pointer.ixx
    src/vclib/mesh/containers/custom_component_vector_handle.ixx
    src/vclib/mesh/containers/custom_components_vector_map.ixx
    src/vclib/mesh/containers/element_container.ixx
    src/vclib/mesh/containers/vertical_components_vector_tuple.ixx
    src/vclib/mesh/elements/edge.ixx
    src/vclib/mesh/elements/face.ixx
    src/vclib/mesh/elements/element.ixx
    src/vclib/mesh/elements/vertex.ixx
    src/vclib/mesh/requirements/element_requirements.ixx
    src/vclib/mesh/utils/mesh_check.ixx
    src/vclib/mesh/utils/mesh_edge_util.ixx
    src/vclib/mesh/utils/mesh_inertia.ixx
    src/vclib/mesh/utils/mesh_info.ixx
    src/vclib/mesh/utils/mesh_pos.ixx
    src/vclib/mesh/utils/tri_poly_index_bimap.ixx
    src/vclib/meshes/detail.ixx
    src/vclib/meshes/edge_mesh.ixx
    src/vclib/meshes/point_cloud.ixx
    src/vclib/meshes/poly_mesh.ixx
    src/vclib/meshes/tri_edge_mesh.ixx
    src/vclib/meshes/tri_mesh.ixx
    src/vclib/misc/hash.ixx
    src/vclib/misc/logger.ixx
    src/vclib/misc/parallel.ixx
    src/vclib/misc/timer.ixx
    src/vclib/misc/logger/logger.ixx
    src/vclib/space/array.ixx
    src/vclib/space/bit_set.ixx
    src/vclib/space/box.ixx
    src/vclib/space/color.ixx
    src/vclib/space/graph.ixx
    src/vclib/space/grid.ixx
    src/vclib/space/image.ixx
    src/vclib/space/kd_tree.ixx
    src/vclib/space/matrix.ixx
    src/vclib/space/plane.ixx
    src/vclib/space/point.ixx
    src/vclib/space/polygon.ixx
    src/vclib/space/principal_curvature.ixx
    src/vclib/space/quaternion.ixx
    src/vclib/space/sampler.ixx
    src/vclib/space/segment.ixx
    src/vclib/space/sphere.ixx
    src/vclib/space/tex_coord.ixx
    src/vclib/space/triangle.ixx
    src/vclib/space/triangle_wrapper.ixx
    src/vclib/space/vector.ixx
    src/vclib/space/graph/bipartite.ixx
    src/vclib/space/graph/undirected_node.ixx
    src/vclib/space/grid/abstract_grid.ixx
    src/vclib/space/grid/hash_table_grid.ixx
    src/vclib/space/grid/regular_grid.ixx
    src/vclib/space/grid/static_grid.ixx
    src/vclib/space/sampler/mesh_sampler.ixx
    src/vclib/space/sampler/point_sampler.ixx
    src/vclib/space/sampler/vertex_sampler.ixx
    src/vclib/views/mesh.ixx
    src/vclib/views/view.ixx
    src/vclib/views/mesh/components.ixx
    src/vclib/views/mesh/elements.ixx
)

if (TARGET vclib-external-vcg)
    list(APPEND MODULES
        src/vclib/ext/vcg.ixx
        src/vclib/ext/vcg/concepts.ixx
        src/vclib/ext/vcg/import_export.ixx
        src/vclib/ext/vcg/type_mapping.ixx
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

