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

#include <cassert>
#include <cmath>
#include <functional>
#include <optional>
#include <ranges>
#include <set>
#include <vector>

#include <Eigen/Eigenvalues>

export module vclib.algorithms.core;

import vclib.concepts;
import vclib.exceptions;
import vclib.math;
import vclib.misc;
import vclib.space.core;
import vclib.types;
import vclib.views;

export {
#include <vclib/algorithms/core/bounding_box.h>
#include <vclib/algorithms/core/polygon/create.h>
#include <vclib/algorithms/core/polygon/geometry.h>
#include <vclib/algorithms/core/polygon/topology.h>
#include <vclib/algorithms/core/stat.h>

// depends on stat
#include <vclib/algorithms/core/fitting.h>

// depend on bounding_box and polygon
#include <vclib/algorithms/core/distance/misc.h>
#include <vclib/algorithms/core/intersection/misc.h>

// depend on distance/intersection misc
#include <vclib/algorithms/core/distance/element.h>
#include <vclib/algorithms/core/distance/functions.h>
#include <vclib/algorithms/core/intersection/element.h>
#include <vclib/algorithms/core/intersection/functions.h>

}

