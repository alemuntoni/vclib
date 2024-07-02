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

export module vclib.mesh.views.components;

import vclib.concepts;
import vclib.types;

export {
#include <vclib/mesh/views/components/adj_edges.h>
#include <vclib/mesh/views/components/adj_faces.h>
#include <vclib/mesh/views/components/adj_vertices.h>
#include <vclib/mesh/views/components/colors.h>
#include <vclib/mesh/views/components/coords.h>
#include <vclib/mesh/views/components/normals.h>
#include <vclib/mesh/views/components/principal_curvatures.h>
#include <vclib/mesh/views/components/quality.h>
#include <vclib/mesh/views/components/selection.h>
#include <vclib/mesh/views/components/tex_coords.h>
}
