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

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <numeric>
#include <queue>
#include <string>
#include <typeindex>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>

export module vclib.space.complex;

import vclib.algorithms.core;
import vclib.concepts;
import vclib.mesh;
import vclib.space.core;
import vclib.types;

export import :graph;
export import :grid;
export import :sampler;

export {
#include <vclib/space/complex/kd_tree.h>
#include <vclib/space/complex/mesh_edge_util.h>
#include <vclib/space/complex/mesh_inertia.h>
#include <vclib/space/complex/mesh_info.h>
#include <vclib/space/complex/mesh_pos.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>
}
