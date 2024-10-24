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

#ifndef VCLIB_WITH_MODULES
#include <vclib/algorithms/mesh/convex_hull.h>
#include <vclib/load_save.h>
#include <vclib/meshes/tri_mesh.h>
#else
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <unordered_set>
import vclib.core;
#endif

int main()
{
    vcl::TriMesh mesh =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    vcl::ConsoleLogger log;

    log.enablePrintTimer();
    vcl::TriMesh ch = vcl::convexHull<vcl::TriMesh>(
        mesh.vertices() | vcl::views::coords, log);

    vcl::save(ch, VCLIB_RESULTS_PATH "/convex_hull.ply");

    return 0;
}
