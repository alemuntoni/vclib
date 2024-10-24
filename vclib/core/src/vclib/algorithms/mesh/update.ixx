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
#include <cmath>
#include <cstdint>
#include <exception>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>

export module vclib.algorithms.mesh:update;

import vclib.algorithms.core;
import vclib.concepts;
import vclib.math;
import vclib.mesh;
import vclib.misc;
import vclib.space.complex;
import vclib.space.core;
import vclib.types;
import vclib.views;

import :base;
import :clean;
import :intersection;
import :point_sampling;
import :stat;

export {
#include <vclib/algorithms/mesh/update/bounding_box.h>
#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/flag.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/algorithms/mesh/update/quality.h>
#include <vclib/algorithms/mesh/update/selection.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/algorithms/mesh/update/transform.h>

// depends on update/normal
#include <vclib/algorithms/mesh/update/curvature.h>
}

