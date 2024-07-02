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

#include <string>
#include <vector>

export module vclib.concepts.mesh.components;

import vclib.concepts.ranges.range;
import vclib.types;

export {
#include <vclib/concepts/mesh/components/component.h>

#include <vclib/concepts/mesh/components/adjacent_edges.h>
#include <vclib/concepts/mesh/components/adjacent_faces.h>
#include <vclib/concepts/mesh/components/adjacent_vertices.h>
#include <vclib/concepts/mesh/components/bit_flags.h>
#include <vclib/concepts/mesh/components/bounding_box.h>
#include <vclib/concepts/mesh/components/color.h>
#include <vclib/concepts/mesh/components/coordinate.h>
#include <vclib/concepts/mesh/components/custom_components.h>
#include <vclib/concepts/mesh/components/mark.h>
#include <vclib/concepts/mesh/components/name.h>
#include <vclib/concepts/mesh/components/normal.h>
#include <vclib/concepts/mesh/components/principal_curvature.h>
#include <vclib/concepts/mesh/components/quality.h>
#include <vclib/concepts/mesh/components/tex_coord.h>
#include <vclib/concepts/mesh/components/texture_images.h>
#include <vclib/concepts/mesh/components/texture_paths.h>
#include <vclib/concepts/mesh/components/transform_matrix.h>
#include <vclib/concepts/mesh/components/vertex_references.h>
#include <vclib/concepts/mesh/components/wedge_colors.h>
#include <vclib/concepts/mesh/components/wedge_tex_coords.h>
}
