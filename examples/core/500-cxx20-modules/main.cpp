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

#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

import vclib.base;

int main()
{
    vcl::uint myValue = vcl::UINT_NULL;

    std::cerr << "UINT_NULL value: " << myValue << std::endl;

    std::vector<vcl::uint> vec = {1, 2, 3, vcl::UINT_NULL, 5};

    std::vector<vcl::uint> newIndices = {0, 1, vcl::UINT_NULL, 2, 3};

    vcl::compactVector(vec, newIndices);

    std::cerr << "Compacted vector: " << std::endl;
    for (const auto& val : vec) {
        std::cerr << val << " ";
    }
    std::cerr << std::endl;

    vcl::uint val = 4;

    vcl::uint* ptr = &val;

    std::cerr << "Dereference ptr: " << vcl::dereferencePtr(val) << "; " << vcl::dereferencePtr(ptr) << std::endl;

    std::cerr << "Address of: " << vcl::addressOfObj(val) << "; " << vcl::addressOfObj(ptr) << std::endl;

    std::cerr << "Size of vector: " << vcl::asConst(vec).size() << std::endl;

    std::cerr << "Position of std::set<vcl::uint> in variadic templates: "
              << vcl::indexInTypePack<
                     std::set<vcl::uint>,
                     int,
                     float,
                     std::set<vcl::uint>,
                     double>()
              << std::endl;

    std::cerr << "Is shared pointer a pointer: " << vcl::IsAnyPointer<std::shared_ptr<int>> << std::endl;
    std::cerr << "Is plain pointer a pointer: " << vcl::IsAnyPointer<int*> << std::endl;
    std::cerr << "Is int a pointer: " << vcl::IsAnyPointer<int> << std::endl;

    std::size_t hash = 0;

    vcl::hashCombine(hash, 42, 3.14);

    std::cerr << "Hash value: " << hash << std::endl;

    std::cerr << "Is 3.14 degenerate? " << vcl::isDegenerate(3.14) << std::endl;
    std::cerr << "Is NaN degenerate? " << vcl::isDegenerate(std::nan("")) << std::endl;

    return 0;
}
