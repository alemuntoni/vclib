/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

module;

#include <any>
#include <algorithm>
#include <cassert>
#include <compare>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

export module vclib.mesh.components;

import vclib.concepts;
import vclib.io;
import vclib.misc;
import vclib.space.core;
import vclib.types;

import vclib.mesh.iterators;

export {
// base data structures:
#include <vclib/mesh/components/detail/component_data.h>
#include <vclib/mesh/components/detail/custom_components_data.h>
#include <vclib/mesh/components/parent_mesh_pointer.h>

#include <vclib/mesh/components/bases/component.h>
#include <vclib/mesh/components/bases/container_component.h>
#include <vclib/mesh/components/bases/index_container_component.h>
#include <vclib/mesh/components/bases/pointer_container_component.h>

#include <vclib/mesh/components/bases/reference_container_component.h>

// components:
#include <vclib/mesh/components/adjacent_edges.h>
#include <vclib/mesh/components/adjacent_faces.h>
#include <vclib/mesh/components/adjacent_vertices.h>
#include <vclib/mesh/components/bit_flags.h>
#include <vclib/mesh/components/bounding_box.h>
#include <vclib/mesh/components/color.h>
#include <vclib/mesh/components/coordinate.h>
#include <vclib/mesh/components/custom_components.h>
#include <vclib/mesh/components/mark.h>
#include <vclib/mesh/components/name.h>
#include <vclib/mesh/components/normal.h>
#include <vclib/mesh/components/polygon_bit_flags.h>
#include <vclib/mesh/components/principal_curvature.h>
#include <vclib/mesh/components/quality.h>
#include <vclib/mesh/components/tex_coord.h>
#include <vclib/mesh/components/texture_images.h>
#include <vclib/mesh/components/texture_paths.h>
#include <vclib/mesh/components/transform_matrix.h>
#include <vclib/mesh/components/triangle_bit_flags.h>
#include <vclib/mesh/components/vertex_references.h>
#include <vclib/mesh/components/wedge_colors.h>
#include <vclib/mesh/components/wedge_tex_coords.h>
}
