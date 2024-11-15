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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef COLOR_H
#define COLOR_H

#ifndef VCLIB_WITH_MODULES
#include <vclib/space.h>
#else
#include <Eigen/Core>
import vclib.core;
#endif

void colorStaticAsserts()
{
    using namespace vcl;

    // color
    static_assert(
        ColorConcept<Color>, "Color does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<Color&>, "Color& does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<const Color>,
        "const Color does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<const Color&>,
        "const Color& does not satisfy the ColorConcept");
    static_assert(
        ColorConcept<Color&&>, "Color&& does not satisfy the ColorConcept");

    static_assert(Serializable<Color>, "Color is not serializable");
}

#endif // COLOR_H
