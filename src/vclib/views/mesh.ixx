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

#include <ranges>
#include <type_traits>

export module vclib.views.mesh;

import vclib.concepts;
import vclib.types;

export {
// components
#include <vclib/views/mesh/components/adj_edges.h>
#include <vclib/views/mesh/components/adj_faces.h>
#include <vclib/views/mesh/components/adj_vertices.h>
#include <vclib/views/mesh/components/colors.h>
#include <vclib/views/mesh/components/coords.h>
#include <vclib/views/mesh/components/normals.h>
#include <vclib/views/mesh/components/principal_curvatures.h>
#include <vclib/views/mesh/components/quality.h>
#include <vclib/views/mesh/components/selection.h>
#include <vclib/views/mesh/components/tex_coords.h>

// elements
#include <vclib/views/mesh/elements/edge.h>
#include <vclib/views/mesh/elements/face.h>
#include <vclib/views/mesh/elements/vertex.h>
}
