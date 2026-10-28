// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PER_EDGE_H
#define PER_EDGE_H

#include <vclib/meshes.h>

void perEdgeStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasEdgeAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeAdjacentEdges");

    static_assert(
        HasEdgeAdjacentFaces<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeAdjacentFaces");

    static_assert(
        HasEdgeColors<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeColors");
    static_assert(
        HasEdgeColors<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeColors");
    static_assert(
        HasEdgeColors<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeColors");
    static_assert(
        HasEdgeColors<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeColors");
    static_assert(
        HasEdgeColors<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeColors");

    static_assert(
        HasEdgeCustomComponents<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeCustomComponents");

    static_assert(
        HasEdgeNormals<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeNormals");
    static_assert(
        HasEdgeNormals<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeNormals");
    static_assert(
        HasEdgeNormals<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeNormals");
    static_assert(
        HasEdgeNormals<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeNormals");
    static_assert(
        HasEdgeNormals<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeNormals");

    static_assert(
        HasEdgeQualities<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeQualities");
    static_assert(
        HasEdgeQualities<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeQualities");
    static_assert(
        HasEdgeQualities<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeQualities");
    static_assert(
        HasEdgeQualities<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeQualities");
    static_assert(
        HasEdgeQualities<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeQualities");
}

#endif // PER_EDGE_H
