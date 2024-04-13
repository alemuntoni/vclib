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

#include <iostream>

#include <vector>

import vclib;

static_assert(
    vcl::IteratorConcept<std::vector<int>::iterator>,
    "std::vector<int>::iterator is not an iterator");

static_assert(
    vcl::IsPointer<int*>, "int* is not a pointer");

static_assert(
    !vcl::LoggerConcept<int>, "int is a Logger");

static_assert(
    vcl::IntegralOrEnum<vcl::PrimitiveType>, "PrimitiveType is not an integral or enum");

static_assert(
    !vcl::NonBoolIntegralOrEnum<bool>, "bool is a NonBoolIntegralOrEnum");

static_assert(
    vcl::NonBoolIntegralOrEnum<int>, "int is not a NonBoolIntegralOrEnum");

int main()
{
    auto k = vcl::FLOAT;

    std::cerr << vcl::asConst(k) << "\n";

    return 0;
}
