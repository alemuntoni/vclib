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

#include <vclib/algorithms.h>

int main()
{
    /*
     *  3 6------------------5
     *    |                  |
     *  2 |                  |
     *    |                  |
     *  1 |  1--+--2         |
     *    |  |     |         |
     *  0 7--0     3--+--+---4
     *    0  1     2  3
     */

    vcl::Polygon2d polygon = {
        vcl::Point2d(1, 0),
        vcl::Point2d(1, 1),
        vcl::Point2d(2, 1),
        vcl::Point2d(2, 0),
        vcl::Point2d(3, 0),
        vcl::Point2d(3, 3),
        vcl::Point2d(0, 3),
        vcl::Point2d(0, 0)};

    std::vector<unsigned int> tri = vcl::earCut(polygon.begin(), polygon.end());

    std::cerr << "Triangle indices: \n";
    for (unsigned int i = 0; i < tri.size(); i += 3) {
        std::cerr << tri[i] << "; " << tri[i + 1] << "; " << tri[i + 2] << "\n";
    }

    return 0;
}