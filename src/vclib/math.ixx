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
#include <limits>
#include <random>
#include <set>
#include <vector>

export module vclib.math;

import vclib.concepts.space;
import vclib.space.point;
import vclib.types;

export import vclib.math.base;
// todo: transform

export {
#include <vclib/math/distribution.h>
#include <vclib/math/fibonacci.h>
#include <vclib/math/histogram.h>
#include <vclib/math/min_max.h>
#include <vclib/math/perlin_noise.h>
#include <vclib/math/random.h>
}
