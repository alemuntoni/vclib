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
#include <compare>
#include <concepts>
#include <functional>
#include <iomanip>
#include <iosfwd>
#include <list>
#include <memory>
#include <ostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>

#include <Eigen/Core>
#include <Eigen/Geometry>

export module vclib.space.core;

import vclib.concepts;
import vclib.exceptions;
import vclib.math;
import vclib.misc;
import vclib.serialization;
import vclib.types;

export {
#include <vclib/space/core/array.h>
#include <vclib/space/core/bit_set/bit_proxy.h>
#include <vclib/space/core/bit_set.h>
#include <vclib/space/core/matrix.h>
#include <vclib/space/core/point.h>
#include <vclib/space/core/vector/vector.h>

// depend on point
#include <vclib/space/core/box.h>
#include <vclib/space/core/color.h>
#include <vclib/space/core/plane.h>
#include <vclib/space/core/principal_curvature.h>
#include <vclib/space/core/quaternion.h>
#include <vclib/space/core/segment.h>
#include <vclib/space/core/tex_coord.h>
#include <vclib/space/core/tex_coord_indexed.h>
#include <vclib/space/core/triangle.h>

// depends on array, color
#include <vclib/space/core/image.h>
#include <vclib/space/core/texture.h>

// depends on box
#include <vclib/space/core/sphere.h>

// depend on triangle
#include <vclib/space/core/polygon.h>
#include <vclib/space/core/triangle_wrapper.h>

// depends on vector
#include <vclib/space/core/vector/pointer_vector.h>

// depends on pointer_vector
#include <vclib/space/core/vector/polymorphic_object_vector.h>
}

// for some reason, msvc does not compile the internal calls of the cross
// product, and therefore it is not exported.
// Adding this operation forces the compilation inside this module.
// Without this, on msvc you get an unresolved external symbol error every time
// you try to use this module, and the solution would be to `#include
// <Eigen/Geometry>` along with `import vclib.core;` in the client code.
// note: these dummy objects are not exported
// TODO: test if this is still necessary when msvc gets updated.
vcl::Point3f dummyf
    = vcl::Point3f(1, 0, 0).cross(vcl::Point3f(0, 1, 0));
vcl::Point3d dummyd = vcl::Point3d(1, 0, 0).cross(vcl::Point3d(0, 1, 0));
