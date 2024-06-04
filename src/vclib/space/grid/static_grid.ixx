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
#include <ranges>
#include <set>
#include <vector>

export module vclib.space.grid.static_grid;

import vclib.concepts.const_correctness;
import vclib.concepts.mesh.components;
import vclib.concepts.mesh.elements;
import vclib.concepts.ranges;
import vclib.concepts.space;
import vclib.iterators.space;
import vclib.misc;
import vclib.space.grid.abstract_grid;
import vclib.space.grid.regular_grid;
import vclib.types;

export {
#include <vclib/space/grid/static_grid.h>
}
