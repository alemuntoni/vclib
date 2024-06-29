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

#include <concepts>
#include <string>
#include <type_traits>

export module vclib.concepts;

import vclib.types;

export import vclib.concepts.const_correctness;
export import vclib.concepts.iterators;
export import vclib.concepts.mesh;
export import vclib.concepts.pointers;
export import vclib.concepts.polymorphism;
export import vclib.concepts.ranges;
export import vclib.concepts.space;

export {
#include <vclib/concepts/logger.h>
#include <vclib/concepts/types.h>
}
