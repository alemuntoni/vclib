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

#include <functional>
#include <ranges>
#include <set>

export module vclib.space.grid.abstract_grid;

import vclib.algorithms.core.bounding_box;
import vclib.algorithms.core.distance;
import vclib.concepts.mesh.elements;
import vclib.concepts.ranges;
import vclib.concepts.space;
import vclib.misc;
import vclib.space.box;
import vclib.space.point;
import vclib.space.sphere;
import vclib.types;

export {
#include <vclib/space/grid/abstract_grid.h>
}