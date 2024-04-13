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

module; //Begin global module fragment.

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <initializer_list>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

export module vclib.misc; //Begin the actual module purview

import vclib.types;

export import vclib.misc.hash;
export import vclib.misc.logger;
export import vclib.misc.parallel;
export import vclib.misc.timer;

export {
#include <vclib/misc/compactness.h>
#include <vclib/misc/comparators.h>
#include <vclib/misc/nested_initializer_lists.h>
#include <vclib/misc/pair.h>
#include <vclib/misc/string.h>
#include <vclib/misc/tokenizer.h>
#include <vclib/misc/tuple.h>
}
