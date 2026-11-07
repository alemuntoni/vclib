// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_SHUFFLE_H
#define TEST_SHUFFLE_H

#include "types.h"

#include <vclib/algorithms/mesh/shuffle.h>
#include <vclib/meshes.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <print>
#include <set>
#include <type_traits>
#include <vector>

inline void testShuffleVertices()
{
    vcl::TriMesh mesh;
    for (uint i = 0; i < 20; ++i) {
        mesh.addVertex(Point3d(i, i * 2, i * 3));
    }
    assert(mesh.vertexCount() == 20);

    // 1. Shuffled vertex index vector
    auto indices = vcl::fillAndShuffleVertexIndexVector(mesh);
    static_assert(std::is_same_v<decltype(indices), std::vector<vcl::TriMesh::IndexType>>);
    assert(indices.size() == 20);

    std::set<uint> idxSet(indices.begin(), indices.end());
    assert(idxSet.size() == 20);
    for (uint i = 0; i < 20; ++i) {
        assert(idxSet.contains(i));
    }

    // 2. Shuffled vertex proxy vector (mutable)
    auto vVec = vcl::fillAndShuffleVertexVector(mesh);
    static_assert(std::is_same_v<decltype(vVec), std::vector<vcl::VertexType<vcl::TriMesh>>>);
    assert(vVec.size() == 20);
    std::set<uint> vIdxSet;
    for (auto v : vVec) {
        vIdxSet.insert(v.index());
    }
    assert(vIdxSet.size() == 20);

    // 3. Shuffled vertex proxy vector (const)
    const auto& cmesh = mesh;
    auto cvVec = vcl::fillAndShuffleVertexVector(cmesh);
    static_assert(std::is_same_v<decltype(cvVec), std::vector<vcl::ConstVertexType<vcl::TriMesh>>>);
    assert(cvVec.size() == 20);

    // 4. Deterministic with seed
    auto ind1 = vcl::fillAndShuffleVertexIndexVector(mesh, 12345u);
    auto ind2 = vcl::fillAndShuffleVertexIndexVector(mesh, 12345u);
    assert(ind1 == ind2);

    auto ind3 = vcl::fillAndShuffleVertexIndexVector(mesh, 54321u);
    assert(ind1 != ind3);

    // 5. Mesh with deleted vertices
    mesh.deleteVertex(5);
    mesh.deleteVertex(10);
    assert(mesh.vertexCount() == 18);
    assert(mesh.deletedVertexCount() == 2);

    auto delIndices = vcl::fillAndShuffleVertexIndexVector(mesh);
    assert(delIndices.size() == 18);
    for (auto idx : delIndices) {
        assert(idx != 5 && idx != 10);
        assert(!mesh.vertex(idx).deleted());
    }

    auto delVVec = vcl::fillAndShuffleVertexVector(mesh);
    assert(delVVec.size() == 18);
    for (auto v : delVVec) {
        assert(v.index() != 5 && v.index() != 10);
        assert(!v.deleted());
    }
}

inline void testShuffleFaces()
{
    vcl::TriMesh mesh;
    for (uint i = 0; i < 10; ++i) {
        mesh.addVertex(Point3d(i, 0, 0));
        mesh.addVertex(Point3d(i, 1, 0));
    }
    for (uint i = 0; i < 8; ++i) {
        mesh.addFace(i, i + 1, i + 2);
    }
    assert(mesh.faceCount() == 8);

    // 1. Shuffled face index vector
    auto fIndices = vcl::fillAndShuffleFaceIndexVector(mesh);
    static_assert(std::is_same_v<decltype(fIndices), std::vector<vcl::TriMesh::IndexType>>);
    assert(fIndices.size() == 8);

    std::set<uint> fIdxSet(fIndices.begin(), fIndices.end());
    assert(fIdxSet.size() == 8);
    for (uint i = 0; i < 8; ++i) {
        assert(fIdxSet.contains(i));
    }

    // 2. Shuffled face vector (mutable)
    auto fVec = vcl::fillAndShuffleFaceVector(mesh);
    static_assert(std::is_same_v<decltype(fVec), std::vector<vcl::FaceType<vcl::TriMesh>>>);
    assert(fVec.size() == 8);

    // 3. Shuffled face vector (const)
    const auto& cmesh = mesh;
    auto cfVec = vcl::fillAndShuffleFaceVector(cmesh);
    static_assert(std::is_same_v<decltype(cfVec), std::vector<vcl::ConstFaceType<vcl::TriMesh>>>);
    assert(cfVec.size() == 8);

    // 4. Mesh with deleted faces
    mesh.deleteFace(2);
    assert(mesh.faceCount() == 7);
    auto delFIndices = vcl::fillAndShuffleFaceIndexVector(mesh);
    assert(delFIndices.size() == 7);
    for (auto idx : delFIndices) {
        assert(idx != 2);
        assert(!mesh.face(idx).deleted());
    }
}

inline void testShuffleEdges()
{
    MyEdgeMesh mesh = createSampleEdgeMesh();
    assert(mesh.edgeCount() == 4);

    auto eIndices = vcl::fillAndShuffleEdgeIndexVector(mesh);
    static_assert(std::is_same_v<decltype(eIndices), std::vector<MyEdgeMesh::IndexType>>);
    assert(eIndices.size() == 4);

    auto eVec = vcl::fillAndShuffleEdgeVector(mesh);
    static_assert(std::is_same_v<decltype(eVec), std::vector<vcl::EdgeType<MyEdgeMesh>>>);
    assert(eVec.size() == 4);

    const auto& cmesh = mesh;
    auto ceVec = vcl::fillAndShuffleEdgeVector(cmesh);
    static_assert(std::is_same_v<decltype(ceVec), std::vector<vcl::ConstEdgeType<MyEdgeMesh>>>);
    assert(ceVec.size() == 4);
}

inline void testShuffle64Bit()
{
    vcl::TriMesh64 mesh;
    for (uint i = 0; i < 15; ++i) {
        mesh.addVertex(Point3d(i, i, i));
    }
    for (uint i = 0; i < 10; ++i) {
        mesh.addFace(i, i + 1, i + 2);
    }

    auto vIndices = vcl::fillAndShuffleVertexIndexVector(mesh, 42u);
    static_assert(std::is_same_v<decltype(vIndices), std::vector<uint64_t>>);
    assert(vIndices.size() == 15);

    auto fIndices = vcl::fillAndShuffleFaceIndexVector(mesh, 42u);
    static_assert(std::is_same_v<decltype(fIndices), std::vector<uint64_t>>);
    assert(fIndices.size() == 10);
}

inline void testShuffle()
{
    testShuffleVertices();
    testShuffleFaces();
    testShuffleEdges();
    testShuffle64Bit();
    std::println("Shuffle tests passed!");
}

#endif // TEST_SHUFFLE_H
