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

#include <cmath>
#include <vector>

export module vclib.algorithms.core.intersection;

import vclib.algorithms.core.polygon;
import vclib.concepts.mesh.elements;
import vclib.concepts.space;
import vclib.space.box;
import vclib.space.plane;
import vclib.space.point;
import vclib.space.sphere;
import vclib.space.triangle_wrapper;

export {
#include <vclib/algorithms/core/intersection/misc.h>

#include <vclib/algorithms/core/intersection/element.h>
#include <vclib/algorithms/core/intersection/functions.h>

}

