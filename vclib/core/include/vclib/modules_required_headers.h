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

#ifndef VCL_MODULES_REQUIRED_HEADERS_H
#define VCL_MODULES_REQUIRED_HEADERS_H

// this is a list of headers that, at the time of writing, are required to be
// included before `import vclib.core` when using c++20 modules

// probably it is not necessary to include them all in your code (it depends on
// which parts of the library you are using).
// The headers have been listed here in order to do not add them manually in
// each file that uses `import vclib.core` (examples and tests).

// they are required because otherwise you'll get linking errors (depending on
// the compiler you are using) like "undefined reference to" some operators
// (like `operator<<` for `std::ostream`), that appears that are not exported
// when using modules.
// they are probably bugs in the compilers, that hopefully won't be necessary
// when the modules support will be more mature.

#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <Eigen/Geometry>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

#endif // VCL_MODULES_REQUIRED_HEADERS_H
