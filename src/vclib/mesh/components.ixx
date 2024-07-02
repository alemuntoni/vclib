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
#include <string>
#include <typeindex>
#include <vector>

export module vclib.mesh.components;

import vclib.concepts;
import vclib.io.serialization;
import vclib.mesh.components.bases;
import vclib.mesh.components.detail;
import vclib.mesh.components.parent_mesh_pointer;
import vclib.mesh.iterators;
import vclib.space.bit_set;
import vclib.space.box;
import vclib.space.color;
import vclib.space.matrix;
import vclib.space.point;
import vclib.space.principal_curvature;
import vclib.space.tex_coord;
import vclib.space.texture;
import vclib.space.vector;
import vclib.types;

export {
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
