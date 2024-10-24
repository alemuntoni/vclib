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
#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#else
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <fstream>
#include <set>
import vclib.core;
#endif

int main()
{
    vcl::TriMesh m;

    vcl::loadPly(m, VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    m.enablePerVertexAdjacentFaces();
    vcl::updatePerVertexAdjacentFaces(m);

    for (const vcl::TriMesh::Vertex& v : m.vertices()) {
        std::cerr << "Adj faces to vertex " << m.index(v) << ": \n\t";
        for (const vcl::TriMesh::Face* f : v.adjFaces()) {
            std::cerr << m.index(f) << "; ";
        }
        std::cerr << std::endl;
    }

    std::cerr << std::endl << std::endl;

    m.enablePerVertexAdjacentVertices();
    vcl::updatePerVertexAdjacentVertices(m);

    for (const vcl::TriMesh::Vertex& v : m.vertices()) {
        std::cerr << "Adj vertices to vertex " << m.index(v) << ": \n\t";
        for (const vcl::TriMesh::Vertex* av : v.adjVertices()) {
            std::cerr << m.index(av) << "; ";
        }
        std::cerr << std::endl;
    }

    std::cerr << std::endl << std::endl;

    m.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(m);

    for (const vcl::TriMesh::Face& f : m.faces()) {
        std::cerr << "Adj faces to face " << m.index(f) << ": \n\t";
        for (const vcl::TriMesh::Face* fadj : f.adjFaces()) {
            if (fadj != nullptr)
                std::cerr << m.index(fadj) << "; ";
            else
                std::cerr << "-1; ";
        }
        std::cerr << std::endl;
    }

    return 0;
}
