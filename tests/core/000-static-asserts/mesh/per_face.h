// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PER_FACE_H
#define PER_FACE_H

#include <vclib/meshes.h>

void perFaceStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasTriangles<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasTriangles");
    static_assert(
        HasTriangles<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasTriangles");

    static_assert(
        HasPolygons<PolyMesh>, "PolyMesh does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<const PolyMesh>,
        "const PolyMesh does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<PolyMesh&>, "PolyMesh& does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasPolygons");
    static_assert(
        HasPolygons<PolyMesh&&>, "PolyMesh&& does not satisfy the HasPolygons");

    static_assert(
        HasFaceAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasFaceAdjacentEdges");
    static_assert(
        HasFaceAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasFaceAdjacentEdges");

    static_assert(
        HasFaceAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceAdjacentFaces");
    static_assert(
        HasFaceAdjacentFaces<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceAdjacentFaces");

    static_assert(
        HasFaceColors<TriMesh>,
        "TriMesh does not satisfy the HasFaceColors");
    static_assert(
        HasFaceColors<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceColors");
    static_assert(
        HasFaceColors<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceColors");
    static_assert(
        HasFaceColors<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceColors");
    static_assert(
        HasFaceColors<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceColors");

    static_assert(
        HasFaceCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceCustomComponents");
    static_assert(
        HasFaceCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceCustomComponents");

    static_assert(
        HasFaceNormals<PolyMesh>,
        "PolyMesh does not satisfy the HasFaceNormals");
    static_assert(
        HasFaceNormals<const PolyMesh>,
        "const PolyMesh does not satisfy the HasFaceNormals");
    static_assert(
        HasFaceNormals<PolyMesh&>,
        "PolyMesh& does not satisfy the HasFaceNormals");
    static_assert(
        HasFaceNormals<const PolyMesh&>,
        "const PolyMesh& does not satisfy the HasFaceNormals");
    static_assert(
        HasFaceNormals<PolyMesh&&>,
        "PolyMesh&& does not satisfy the HasFaceNormals");

    // static_assert(
    //     HasFacePrincipalCurvatures<TriMesh>,
    //     "TriMesh does not satisfy the HasFacePrincipalCurvatures");
    // static_assert(
    //     HasFacePrincipalCurvatures<const TriMesh>,
    //     "const TriMesh does not satisfy the HasFacePrincipalCurvatures");
    // static_assert(
    //     HasFacePrincipalCurvatures<TriMesh&>,
    //     "TriMesh& does not satisfy the HasFacePrincipalCurvatures");
    // static_assert(
    //     HasFacePrincipalCurvatures<const TriMesh&>,
    //     "const TriMesh& does not satisfy the HasFacePrincipalCurvatures");
    // static_assert(
    //     HasFacePrincipalCurvatures<TriMesh&&>,
    //     "TriMesh&& does not satisfy the HasFacePrincipalCurvatures");

    static_assert(
        HasFaceQualities<TriMesh>,
        "TriMesh does not satisfy the HasFaceQualities");
    static_assert(
        HasFaceQualities<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceQualities");
    static_assert(
        HasFaceQualities<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceQualities");
    static_assert(
        HasFaceQualities<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceQualities");
    static_assert(
        HasFaceQualities<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceQualities");

    // static_assert(
    //     HasFaceWedgeColors<TriMesh>,
    //     "TriMesh does not satisfy the HasFaceWedgeColors");
    // static_assert(
    //     HasFaceWedgeColors<const TriMesh>,
    //     "const TriMesh does not satisfy the HasFaceWedgeColors");
    // static_assert(
    //     HasFaceWedgeColors<TriMesh&>,
    //     "TriMesh& does not satisfy the HasFaceWedgeColors");
    // static_assert(
    //     HasFaceWedgeColors<const TriMesh&>,
    //     "const TriMesh& does not satisfy the HasFaceWedgeColors");
    // static_assert(
    //     HasFaceWedgeColors<TriMesh&&>,
    //     "TriMesh&& does not satisfy the HasFaceWedgeColors");

    static_assert(
        HasFaceWedgeTexCoords<TriMesh>,
        "TriMesh does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<const TriMesh>,
        "const TriMesh does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<TriMesh&>,
        "TriMesh& does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<const TriMesh&>,
        "const TriMesh& does not satisfy the HasFaceWedgeTexCoords");
    static_assert(
        HasFaceWedgeTexCoords<TriMesh&&>,
        "TriMesh&& does not satisfy the HasFaceWedgeTexCoords");
}

#endif // PER_FACE_H
