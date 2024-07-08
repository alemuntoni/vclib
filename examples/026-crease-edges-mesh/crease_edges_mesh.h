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

#ifndef BIMBA_SPHERE_INTERSECTION_H
#define BIMBA_SPHERE_INTERSECTION_H

#ifndef VCLIB_WITH_MODULES
#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#else
#include <fstream>
#include <ranges>
#include <set>
import vclib;
#endif

template<vcl::EdgeMeshConcept OutMeshType, vcl::FaceMeshConcept InMeshType>
OutMeshType creaseEdgesMesh(InMeshType& m)
{
    m.enablePerFaceAdjacentFaces();

    vcl::updatePerFaceAdjacentFaces(m);

    // select crease edges
    vcl::selectCreaseFaceEdges(m, -vcl::toRad(30.f), vcl::toRad(30.f));

    return vcl::perFaceEdgeSelectionMeshFilter<OutMeshType>(m);
}

#endif // BIMBA_SPHERE_INTERSECTION_H
