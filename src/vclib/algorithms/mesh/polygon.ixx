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

#include <set>
#include <string>
#include <vector>

export module vclib.algorithms.mesh.polygon;

import vclib.algorithms.core.polygon;
import vclib.concepts;
import vclib.exceptions;
import vclib.misc;
import vclib.space.core;
import vclib.types;


export {
#include <vclib/algorithms/mesh/polygon.h>
}

// for some reason, clang does not compile the internal calls of the == operator
// between std::set iterators, and therefore it is not exported.
// Adding this operation forces the compilation inside this module.
// Without this, on clang you get an unresolved external symbol error every time
// you try to use this module, and the solution would be to `#include <set>`
// along with `import vclib;` in the client code.
// note: these dummy objects are not exported
// TODO: test if this is still necessary when clang gets updated.
std::set<std::pair<uint, uint>, vcl::UnorderedPairComparator<uint>>
    dummySet;

bool dummyB = dummySet.find(std::make_pair(0, 0)) == dummySet.end();
