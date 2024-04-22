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

set(HEADERS
    include/vclib/algorithms.h
    include/vclib/concepts.h
    include/vclib/exceptions.h
    include/vclib/load_save.h
    include/vclib/math.h
    include/vclib/meshes.h
    include/vclib/miscellaneous.h
    include/vclib/space.h
    include/vclib/types.h
    include/vclib/views.h
    include/vclib/algorithms/core.h
    include/vclib/algorithms/mesh.h
    include/vclib/algorithms/core/bounding_box.h
    include/vclib/algorithms/core/distance.h
    include/vclib/algorithms/core/fitting.h
    include/vclib/algorithms/core/intersection.h
    include/vclib/algorithms/core/polygon.h
    include/vclib/algorithms/core/stat.h
    include/vclib/algorithms/core/distance/functions.h
    include/vclib/algorithms/core/distance/element.h
    include/vclib/algorithms/core/distance/misc.h
    include/vclib/algorithms/core/intersection/element.h
    include/vclib/algorithms/core/intersection/functions.h
    include/vclib/algorithms/core/intersection/misc.h
    include/vclib/algorithms/core/polygon/create.h
    include/vclib/algorithms/core/polygon/geometry.h
    include/vclib/algorithms/core/polygon/topology.h
    include/vclib/algorithms/mesh/bounding_box.h
    include/vclib/algorithms/mesh/clean.h
    include/vclib/algorithms/mesh/create.h
    include/vclib/algorithms/mesh/distance.h
    include/vclib/algorithms/mesh/filter.h
    include/vclib/algorithms/mesh/import_export.h
    include/vclib/algorithms/mesh/intersection.h
    include/vclib/algorithms/mesh/point_sampling.h
    include/vclib/algorithms/mesh/polygon.h
    include/vclib/algorithms/mesh/shuffle.h
    include/vclib/algorithms/mesh/smooth.h
    include/vclib/algorithms/mesh/sort.h
    include/vclib/algorithms/mesh/stat.h
    include/vclib/algorithms/mesh/update.h
    include/vclib/algorithms/mesh/create/cone.h
    include/vclib/algorithms/mesh/create/dodecahedron.h
    include/vclib/algorithms/mesh/create/hexahedron.h
    include/vclib/algorithms/mesh/create/icosahedron.h
    include/vclib/algorithms/mesh/create/sphere.h
    include/vclib/algorithms/mesh/create/tetrahedron.h
    include/vclib/algorithms/mesh/import_export/export_matrix.h
    include/vclib/algorithms/mesh/import_export/import_matrix.h
    include/vclib/algorithms/mesh/stat/quality.h
    include/vclib/algorithms/mesh/stat/selection.h
    include/vclib/algorithms/mesh/update/bounding_box.h
    include/vclib/algorithms/mesh/update/color.h
    include/vclib/algorithms/mesh/update/curvature.h
    include/vclib/algorithms/mesh/update/flag.h
    include/vclib/algorithms/mesh/update/normal.h
    include/vclib/algorithms/mesh/update/quality.h
    include/vclib/algorithms/mesh/update/selection.h
    include/vclib/algorithms/mesh/update/topology.h
    include/vclib/algorithms/mesh/update/transform.h
    include/vclib/concepts/const_correctness.h
    include/vclib/concepts/iterators.h
    include/vclib/concepts/logger.h
    include/vclib/concepts/mesh.h
    include/vclib/concepts/pointers.h
    include/vclib/concepts/range.h
    include/vclib/concepts/space.h
    include/vclib/concepts/types.h
    include/vclib/concepts/mesh/components.h
    include/vclib/concepts/mesh/containers.h
    include/vclib/concepts/mesh/edge_mesh_concept.h
    include/vclib/concepts/mesh/face_mesh_concept.h
    include/vclib/concepts/mesh/mesh_concept.h
    include/vclib/concepts/mesh/per_edge.h
    include/vclib/concepts/mesh/per_face.h
    include/vclib/concepts/mesh/per_mesh.h
    include/vclib/concepts/mesh/per_vertex.h
    include/vclib/concepts/mesh/components/adjacent_edges.h
    include/vclib/concepts/mesh/components/adjacent_faces.h
    include/vclib/concepts/mesh/components/adjacent_vertices.h
    include/vclib/concepts/mesh/components/bit_flags.h
    include/vclib/concepts/mesh/components/bounding_box.h
    include/vclib/concepts/mesh/components/color.h
    include/vclib/concepts/mesh/components/component.h
    include/vclib/concepts/mesh/components/coordinate.h
    include/vclib/concepts/mesh/components/custom_components.h
    include/vclib/concepts/mesh/components/mark.h
    include/vclib/concepts/mesh/components/name.h
    include/vclib/concepts/mesh/components/normal.h
    include/vclib/concepts/mesh/components/principal_curvature.h
    include/vclib/concepts/mesh/components/quality.h
    include/vclib/concepts/mesh/components/tex_coord.h
    include/vclib/concepts/mesh/components/texture_paths.h
    include/vclib/concepts/mesh/components/transform_matrix.h
    include/vclib/concepts/mesh/components/vertex_pointers.h
    include/vclib/concepts/mesh/components/wedge_colors.h
    include/vclib/concepts/mesh/components/wedge_tex_coords.h
    include/vclib/concepts/mesh/containers/edge_container.h
    include/vclib/concepts/mesh/containers/element_container.h
    include/vclib/concepts/mesh/containers/face_container.h
    include/vclib/concepts/mesh/containers/vertex_container.h
    include/vclib/concepts/mesh/elements/edge.h
    include/vclib/concepts/mesh/elements/element.h
    include/vclib/concepts/mesh/elements/face.h
    include/vclib/concepts/mesh/elements/vertex.h
    include/vclib/concepts/ranges/mesh.h
    include/vclib/concepts/ranges/range.h
    include/vclib/concepts/ranges/mesh/edge_range.h
    include/vclib/concepts/ranges/mesh/face_range.h
    include/vclib/concepts/ranges/mesh/vertex_range.h
    include/vclib/concepts/space/array.h
    include/vclib/concepts/space/box.h
    include/vclib/concepts/space/matrix.h
    include/vclib/concepts/space/plane.h
    include/vclib/concepts/space/point.h
    include/vclib/concepts/space/polygon.h
    include/vclib/concepts/space/sampler.h
    include/vclib/concepts/space/segment.h
    include/vclib/concepts/space/sphere.h
    include/vclib/concepts/space/triangle.h
    include/vclib/exceptions/io_exceptions.h
    include/vclib/exceptions/mesh_exceptions.h
    include/vclib/exceptions/misc_exceptions.h
    include/vclib/io/file_info.h
    include/vclib/io/image.h
    include/vclib/io/mesh.h
    include/vclib/io/read.h
    include/vclib/io/write.h
    include/vclib/io/mesh/load.h
    include/vclib/io/mesh/save.h
    include/vclib/io/mesh/obj/load.h
    include/vclib/io/mesh/obj/material.h
    include/vclib/io/mesh/obj/save.h
    include/vclib/io/mesh/off/load.h
    include/vclib/io/mesh/off/save.h
    include/vclib/io/mesh/ply/load.h
    include/vclib/io/mesh/ply/save.h
    include/vclib/io/mesh/ply/detail/ply.h
    include/vclib/io/mesh/ply/detail/edge.h
    include/vclib/io/mesh/ply/detail/extra.h
    include/vclib/io/mesh/ply/detail/face.h
    include/vclib/io/mesh/ply/detail/header.h
    include/vclib/io/mesh/ply/detail/tristrip.h
    include/vclib/io/mesh/ply/detail/vertex.h
    include/vclib/io/mesh/stl/load.h
    include/vclib/io/mesh/stl/save.h
    include/vclib/iterators/mesh.h
    include/vclib/iterators/mesh/face.h
    include/vclib/iterators/mesh/element_container_iterator.h
    include/vclib/iterators/mesh/face/edge_adj_face_iterator.h
    include/vclib/iterators/space/grid/cell_iterator.h
    include/vclib/iterators/space/grid/static_grid_iterator.h
    include/vclib/math/base.h
    include/vclib/math/distribution.h
    include/vclib/math/fibonacci.h
    include/vclib/math/histogram.h
    include/vclib/math/min_max.h
    include/vclib/math/perlin_noise.h
    include/vclib/math/random.h
    include/vclib/math/transform.h
    include/vclib/mesh/components.h
    include/vclib/mesh/elements.h
    include/vclib/mesh/mesh.h
    include/vclib/mesh/mesh_components.h
    include/vclib/mesh/mesh_containers.h
    include/vclib/mesh/requirements.h
    include/vclib/mesh/components/adjacent_edges.h
    include/vclib/mesh/components/adjacent_faces.h
    include/vclib/mesh/components/adjacent_vertices.h
    include/vclib/mesh/components/bit_flags.h
    include/vclib/mesh/components/bounding_box.h
    include/vclib/mesh/components/color.h
    include/vclib/mesh/components/coordinate.h
    include/vclib/mesh/components/custom_components.h
    include/vclib/mesh/components/mark.h
    include/vclib/mesh/components/name.h
    include/vclib/mesh/components/normal.h
    include/vclib/mesh/components/parent_mesh_pointer.h
    include/vclib/mesh/components/polygon_bit_flags.h
    include/vclib/mesh/components/principal_curvature.h
    include/vclib/mesh/components/quality.h
    include/vclib/mesh/components/vertex_pointers.h
    include/vclib/mesh/components/tex_coord.h
    include/vclib/mesh/components/texture_paths.h
    include/vclib/mesh/components/transform_matrix.h
    include/vclib/mesh/components/triangle_bit_flags.h
    include/vclib/mesh/components/wedge_colors.h
    include/vclib/mesh/components/wedge_tex_coords.h
    include/vclib/mesh/components/bases/component.h
    include/vclib/mesh/components/bases/container_component.h
    include/vclib/mesh/components/bases/pointers_container_component.h
    include/vclib/mesh/components/detail/component_data.h
    include/vclib/mesh/components/detail/custom_components_data.h
    include/vclib/mesh/containers/custom_component_vector_handle.h
    include/vclib/mesh/containers/custom_components_vector_map.h
    include/vclib/mesh/containers/edge_container.h
    include/vclib/mesh/containers/element_container.h
    include/vclib/mesh/containers/face_container.h
    include/vclib/mesh/containers/vertex_container.h
    include/vclib/mesh/containers/vertical_components_vector_tuple.h
    include/vclib/mesh/elements/edge.h
    include/vclib/mesh/elements/edge_components.h
    include/vclib/mesh/elements/element.h
    include/vclib/mesh/elements/face.h
    include/vclib/mesh/elements/face_components.h
    include/vclib/mesh/elements/vertex.h
    include/vclib/mesh/elements/vertex_components.h
    include/vclib/mesh/requirements/edge_requirements.h
    include/vclib/mesh/requirements/element_requirements.h
    include/vclib/mesh/requirements/face_requirements.h
    include/vclib/mesh/requirements/mesh_requirements.h
    include/vclib/mesh/requirements/vertex_requirements.h
    include/vclib/mesh/utils/mesh_check.h
    include/vclib/mesh/utils/mesh_edge_util.h
    include/vclib/mesh/utils/mesh_inertia.h
    include/vclib/mesh/utils/mesh_info.h
    include/vclib/mesh/utils/mesh_pos.h
    include/vclib/mesh/utils/tri_poly_index_bimap.h
    include/vclib/meshes/edge_mesh.h
    include/vclib/meshes/point_cloud.h
    include/vclib/meshes/poly_mesh.h
    include/vclib/meshes/tri_edge_mesh.h
    include/vclib/meshes/tri_mesh.h
    include/vclib/meshes/detail/tmp_meshes.h
    include/vclib/misc/compactness.h
    include/vclib/misc/comparators.h
    include/vclib/misc/hash.h
    include/vclib/misc/logger.h
    include/vclib/misc/nested_initializer_lists.h
    include/vclib/misc/pair.h
    include/vclib/misc/parallel.h
    include/vclib/misc/string.h
    include/vclib/misc/timer.h
    include/vclib/misc/tokenizer.h
    include/vclib/misc/tuple.h
    include/vclib/misc/logger/console_logger.h
    include/vclib/misc/logger/logger.h
    include/vclib/misc/logger/null_logger.h
    include/vclib/space/array.h
    include/vclib/space/bit_set.h
    include/vclib/space/box.h
    include/vclib/space/color.h
    include/vclib/space/graph.h
    include/vclib/space/grid.h
    include/vclib/space/image.h
    include/vclib/space/kd_tree.h
    include/vclib/space/matrix.h
    include/vclib/space/plane.h
    include/vclib/space/point.h
    include/vclib/space/polygon.h
    include/vclib/space/principal_curvature.h
    include/vclib/space/quaternion.h
    include/vclib/space/sampler.h
    include/vclib/space/segment.h
    include/vclib/space/sphere.h
    include/vclib/space/tex_coord.h
    include/vclib/space/triangle.h
    include/vclib/space/triangle_wrapper.h
    include/vclib/space/vector.h
    include/vclib/space/bit_set/bit_proxy.h
    include/vclib/space/graph/undirected_node.h
    include/vclib/space/graph/bipartite/bipartite_graph.h
    include/vclib/space/graph/bipartite/iterator/adjacent_left_node_iterator.h
    include/vclib/space/graph/bipartite/iterator/adjacent_node_iterator.h
    include/vclib/space/graph/bipartite/iterator/adjacent_right_node_iterator.h
    include/vclib/space/graph/bipartite/iterator/node_iterator.h
    include/vclib/space/grid/abstract_grid.h
    include/vclib/space/grid/hash_table_grid.h
    include/vclib/space/grid/regular_grid.h
    include/vclib/space/grid/static_grid.h
    include/vclib/space/sampler/mesh_sampler.h
    include/vclib/space/sampler/point_sampler.h
    include/vclib/space/sampler/vertex_sampler.h
    include/vclib/types/base.h
    include/vclib/types/const_correctness.h
    include/vclib/types/inheritance.h
    include/vclib/types/mesh_components.h
    include/vclib/types/mesh_elements.h
    include/vclib/types/pointers.h
    include/vclib/types/variadic_templates.h
    include/vclib/views/mesh.h
    include/vclib/views/view.h
    include/vclib/views/views.h
    include/vclib/views/mesh/components.h
    include/vclib/views/mesh/elements.h
    include/vclib/views/mesh/components/adj_edges.h
    include/vclib/views/mesh/components/adj_faces.h
    include/vclib/views/mesh/components/adj_vertices.h
    include/vclib/views/mesh/components/colors.h
    include/vclib/views/mesh/components/coords.h
    include/vclib/views/mesh/components/normals.h
    include/vclib/views/mesh/components/principal_curvatures.h
    include/vclib/views/mesh/components/quality.h
    include/vclib/views/mesh/components/selection.h
    include/vclib/views/mesh/components/tex_coords.h
    include/vclib/views/mesh/elements/edge.h
    include/vclib/views/mesh/elements/face.h
    include/vclib/views/mesh/elements/vertex.h
)

# headers and sources to include only if using VCG
if (TARGET vclib-external-vcg)
    list(APPEND HEADERS
        include/vclib/ext/vcg/concepts.h
        include/vclib/ext/vcg/export.h
        include/vclib/ext/vcg/import.h
        include/vclib/ext/vcg/type_mapping.h)
endif()

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
    src/vclib/io/mesh/load.ixx
    src/vclib/io/mesh/save.ixx
    src/vclib/io/mesh/obj/load.ixx
    src/vclib/io/mesh/obj/material.ixx
    src/vclib/io/mesh/obj/save.ixx
    src/vclib/io/mesh/off/load.ixx
    src/vclib/io/mesh/off/save.ixx
    src/vclib/io/mesh/ply/detail.ixx
    src/vclib/io/mesh/ply/load.ixx
    src/vclib/io/mesh/ply/save.ixx
    src/vclib/io/mesh/stl/load.ixx
    src/vclib/io/mesh/stl/save.ixx
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

