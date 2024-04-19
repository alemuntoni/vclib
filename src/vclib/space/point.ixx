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

#include <compare>

#include <Eigen/Core>
#include <Eigen/Geometry>

export module vclib.space.point;

import vclib.concepts.space;
import vclib.math.base;
import vclib.misc.hash;
import vclib.types;

export {
#include <vclib/space/point.h>
}

// for some reason, msvc does not compile the internal calls of the cross
// product, and therefore it is not exported.
// Adding this operation forces the compilation inside the point module.
// Without this, on msvc you get an unresolved external symbol error every time
// you try to use the cross product, and the solution would be to include
// Eigen/Geometry along with 'import vclib;' in the client code.
// TODO: test if this is still necessary when msvc gets updated.
vcl::Point3f dummyf
    = vcl::Point3f(1, 0, 0).cross(vcl::Point3f(0, 1, 0));
vcl::Point3d dummyd = vcl::Point3d(1, 0, 0).cross(vcl::Point3d(0, 1, 0));
