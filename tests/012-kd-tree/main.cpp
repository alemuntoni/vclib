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
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/space/complex/kd_tree.h>
#else
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <fstream>
#include <set>
import vclib;
#endif

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<typename MeshType, typename PointType>
std::vector<unsigned int> getKNearestNeighbors(
    const PointType& p,
    unsigned int     k,
    std::string      mesh = "bone.ply")
{
    MeshType m = vcl::loadPly<MeshType>(VCLIB_ASSETS_PATH "/" + mesh);

    vcl::KDTree tree(m);

    return tree.kNearestNeighborsIndices(p, k);
}

TEMPLATE_TEST_CASE(
    "KD-Tree nearest neighbors to [0.5, 0.5, 0.5] in bone.ply",
    "",
    vcl::TriMesh,
    vcl::TriMeshf)
{
    using TriMesh = TestType;

    using PointType = TriMesh::VertexType::CoordType;

    const PointType p(0.5, 0.5, 0.5);

    REQUIRE(getKNearestNeighbors<TriMesh>(p, 1)[0] == 1558);
    REQUIRE(
        getKNearestNeighbors<TriMesh>(p, 5) ==
        std::vector<unsigned int> {1558, 1613, 1720, 1576, 163});
}
