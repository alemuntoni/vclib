// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PER_VERTEX_H
#define PER_VERTEX_H

#include <vclib/meshes.h>

void perVertexStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasVertexAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasVertexAdjacentEdges");

    static_assert(
        HasVertexAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexAdjacentFaces");

    static_assert(
        HasVertexAdjacentVertices<TriMesh>,
        "TriMesh does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexAdjacentVertices");

    static_assert(
        HasVertexColors<TriMesh>,
        "TriMesh does not satisfy the HasVertexColors");
    static_assert(
        HasVertexColors<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexColors");
    static_assert(
        HasVertexColors<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexColors");
    static_assert(
        HasVertexColors<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexColors");
    static_assert(
        HasVertexColors<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexColors");

    static_assert(
        HasVertexNormals<TriMesh>,
        "TriMesh does not satisfy the HasVertexNormals");
    static_assert(
        HasVertexNormals<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexNormals");
    static_assert(
        HasVertexNormals<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexNormals");
    static_assert(
        HasVertexNormals<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexNormals");
    static_assert(
        HasVertexNormals<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexNormals");

    static_assert(
        HasVertexPrincipalCurvatures<TriMesh>,
        "TriMesh does not satisfy the HasVertexPrincipalCurvatures");
    static_assert(
        HasVertexPrincipalCurvatures<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexPrincipalCurvatures");
    static_assert(
        HasVertexPrincipalCurvatures<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexPrincipalCurvatures");
    static_assert(
        HasVertexPrincipalCurvatures<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexPrincipalCurvatures");
    static_assert(
        HasVertexPrincipalCurvatures<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexPrincipalCurvatures");

    static_assert(
        HasVertexQualities<TriMesh>,
        "TriMesh does not satisfy the HasVertexQualities");
    static_assert(
        HasVertexQualities<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexQualities");
    static_assert(
        HasVertexQualities<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexQualities");
    static_assert(
        HasVertexQualities<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexQualities");
    static_assert(
        HasVertexQualities<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexQualities");

    static_assert(
        HasVertexTexCoords<TriMesh>,
        "TriMesh does not satisfy the HasVertexTexCoords");
    static_assert(
        HasVertexTexCoords<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexTexCoords");
    static_assert(
        HasVertexTexCoords<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexTexCoords");
    static_assert(
        HasVertexTexCoords<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexTexCoords");
    static_assert(
        HasVertexTexCoords<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexTexCoords");

    static_assert(
        HasVertexCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexCustomComponents");
}

#endif // PER_VERTEX_H
