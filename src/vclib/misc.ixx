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
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <mutex>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Apple clang does not support c++17 parallel algorithms.
// To compensate this lack, waiting for Apple to support them, we use pstld
// (https://github.com/mikekazakov/pstld) that implements them in the stl
// namespace
#if defined(__clang__) && defined(__APPLE__)
#include <pstld/pstld.h>
#else
#include <execution>
#endif

export module vclib.misc;

import vclib.concepts;
import vclib.types;

export {
#include <vclib/misc/compactness.h>
#include <vclib/misc/comparators.h>
#include <vclib/misc/hash.h>
#include <vclib/misc/iterators/const_pointer_iterator.h>
#include <vclib/misc/nested_initializer_lists.h>
#include <vclib/misc/pair.h>
#include <vclib/misc/parallel.h>
#include <vclib/misc/string.h>
#include <vclib/misc/tokenizer.h>
#include <vclib/misc/tuple.h>
#include <vclib/misc/timer.h>

// depend on timer
#include <vclib/misc/logger/abstract_logger.h>

#include <vclib/misc/logger/logger.h>
#include <vclib/misc/logger/null_logger.h>

#include <vclib/misc/logger/console_logger.h>
}
