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
#include <cassert>
#include <cmath>
#include <random>
#include <set>
#include <string>
#include <vector>

#include <Eigen/Core>

// Apple clang does not support c++17 parallel algorithms.
// To compensate this lack, waiting for Apple to support them, we use pstld
// (https://github.com/mikekazakov/pstld) that implements them in the stl
// namespace
#if defined(__clang__) && defined(__APPLE__)
#include <pstld/pstld.h>
#else
#include <execution>
#endif

export module vclib.algorithms.mesh:base;

import vclib.algorithms.core;
import vclib.concepts;
import vclib.exceptions;
import vclib.mesh;
import vclib.misc;
import vclib.space.core;
import vclib.space.complex;
import vclib.types;

export {
#include <vclib/algorithms/mesh/check_pointers.h>
#include <vclib/algorithms/mesh/face_topology.h>
#include <vclib/algorithms/mesh/sort.h>
#include <vclib/algorithms/mesh/shuffle.h>
}

