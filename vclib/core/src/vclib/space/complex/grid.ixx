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
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <ranges>
#include <set>
#include <unordered_map>
#include <vector>

export module vclib.space.complex:grid;

import vclib.algorithms.core;
import vclib.concepts;
import vclib.misc;
import vclib.space.core;
import vclib.types;

#include <vclib/space/complex/grid/abstract_grid.h>
#include <vclib/space/complex/grid/iterators/cell_iterator.h>
#include <vclib/space/complex/grid/iterators/static_grid_iterator.h>

export {
#include <vclib/space/complex/grid/regular_grid.h>

#include <vclib/space/complex/grid/hash_table_grid.h>
#include <vclib/space/complex/grid/static_grid.h>
}
