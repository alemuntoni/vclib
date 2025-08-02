/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

export module vclib.base:concepts;

export import :concepts.const_correctness;
export import :concepts.iterators;
export import :concepts.pointers;
export import :concepts.polymorphism;
export import :concepts.range;
export import :concepts.serialization;
export import :concepts.types;

/**
 * @defgroup util_concepts Utility Concepts
 *
 * @brief List of utility concepts used in the library, that allows to check and
 * constrain iterators, pointers, const correctness management, ranges, etc.
 */
