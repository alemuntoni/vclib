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

#ifndef VCLIB_WITH_MODULES
#include <vclib/space.h>
#include <vclib/views.h>
#else
#include <ranges>
import vclib;
#endif

struct Point
{
    float x, y, z;
};

class Vertex
{
    Point p;

public:
    Vertex(float x, float y, float z) : p {x, y, z} {}

    Point& coord() { return p; }

    const Point& coord() const { return p; }
};

struct VertexIndex
{
    VertexIndex(uint vi) : vi(vi) {}

    operator uint() const { return vi; }
private:
    uint vi;
};

struct FaceIndex
{
    FaceIndex(uint vi) : vi(vi) {}

    operator uint() const { return vi; }
private:
    uint vi;
};

void foo(VertexIndex vi)
{
    std::cout << vi << std::endl;
}

int main()
{
    foo(56);

    FaceIndex f = 4;

    // VertexIndex vv = f; // error

    std::vector<Vertex> v;

    v.push_back(Vertex(-0.5, -0.5, 0.5));
    v.push_back(Vertex(0.5, -0.5, 0.5));
    v.push_back(Vertex(-0.5, 0.5, 0.5));
    v.push_back(Vertex(0.5, 0.5, 0.5));

    for (auto& p : v | vcl::views::coords) {
        p.x += 1;
    }

    for (const auto& p : v | vcl::views::coords) {
        std::cout << p.x << " " << p.y << " " << p.z << std::endl;
    }

    vcl::Vector<Vertex, -1> vv;

    vv.pushBack(Vertex(-0.5, -0.5, 0.5));
    vv.pushBack(Vertex(0.5, -0.5, 0.5));
    vv.pushBack(Vertex(-0.5, 0.5, 0.5));
    vv.pushBack(Vertex(0.5, 0.5, 0.5));

    vcl::Vector<Vertex, -1>::Iterator it = vv.begin();

    vv.set(it, Vertex(1, 1, 1));

    for (const auto& p : vv | vcl::views::coords) {
        std::cout << p.x << " " << p.y << " " << p.z << std::endl;
    }
}
