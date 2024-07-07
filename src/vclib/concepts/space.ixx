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

#include <cstdint>
#include <concepts>
#include <string>
#include <vector>

export module vclib.concepts:space;

import vclib.types;

import :core;

export {
#include <vclib/concepts/space/array.h>
#include <vclib/concepts/space/box.h>
#include <vclib/concepts/space/color.h>
#include <vclib/concepts/space/image.h>
#include <vclib/concepts/space/matrix.h>
#include <vclib/concepts/space/plane.h>
#include <vclib/concepts/space/point.h>
#include <vclib/concepts/space/polygon.h>
#include <vclib/concepts/space/sampler.h>
#include <vclib/concepts/space/segment.h>
#include <vclib/concepts/space/sphere.h>
#include <vclib/concepts/space/texture.h>
#include <vclib/concepts/space/triangle.h>
}
