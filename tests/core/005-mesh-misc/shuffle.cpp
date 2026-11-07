// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

#include <vclib/algorithms/mesh/shuffle.h>
#include <vclib/meshes.h>

#include <algorithm>
#include <cstdint>
#include <set>
#include <type_traits>
#include <vector>

TEST_CASE("Shuffle - Shuffle Vertices")
{
    vcl::TriMesh mesh;
    for (uint i = 0; i < 20; ++i) {
        mesh.addVertex(Point3d(i, i * 2, i * 3));
    }
    REQUIRE(mesh.vertexCount() == 20);

    // 1. Shuffled vertex index vector
    auto indices = vcl::fillAndShuffleVertexIndexVector(mesh);
    static_assert(
        std::
            is_same_v<decltype(indices), std::vector<vcl::TriMesh::IndexType>>);
    REQUIRE(indices.size() == 20);

    std::set<uint> idxSet(indices.begin(), indices.end());
    REQUIRE(idxSet.size() == 20);
    for (uint i = 0; i < 20; ++i) {
        REQUIRE(idxSet.contains(i));
    }

    // 2. Shuffled vertex proxy vector (mutable)
    auto vVec = vcl::fillAndShuffleVertexVector(mesh);
    static_assert(std::is_same_v<
                  decltype(vVec),
                  std::vector<vcl::VertexType<vcl::TriMesh>>>);
    REQUIRE(vVec.size() == 20);
    std::set<uint> vIdxSet;
    for (auto v : vVec) {
        vIdxSet.insert(v.index());
    }
    REQUIRE(vIdxSet.size() == 20);

    // 3. Shuffled vertex proxy vector (const)
    const auto& cmesh = mesh;
    auto        cvVec = vcl::fillAndShuffleVertexVector(cmesh);
    static_assert(std::is_same_v<
                  decltype(cvVec),
                  std::vector<vcl::ConstVertexType<vcl::TriMesh>>>);
    REQUIRE(cvVec.size() == 20);

    // 4. Deterministic with seed
    auto ind1 = vcl::fillAndShuffleVertexIndexVector(mesh, 12345u);
    auto ind2 = vcl::fillAndShuffleVertexIndexVector(mesh, 12345u);
    REQUIRE(ind1 == ind2);

    auto ind3 = vcl::fillAndShuffleVertexIndexVector(mesh, 54321u);
    REQUIRE(ind1 != ind3);

    // 5. Mesh with deleted vertices
    mesh.deleteVertex(5);
    mesh.deleteVertex(10);
    REQUIRE(mesh.vertexCount() == 18);
    REQUIRE(mesh.deletedVertexCount() == 2);

    auto delIndices = vcl::fillAndShuffleVertexIndexVector(mesh);
    REQUIRE(delIndices.size() == 18);
    for (auto idx : delIndices) {
        REQUIRE((idx != 5 && idx != 10));
        REQUIRE(!mesh.vertex(idx).deleted());
    }

    auto delVVec = vcl::fillAndShuffleVertexVector(mesh);
    REQUIRE(delVVec.size() == 18);
    for (auto v : delVVec) {
        REQUIRE((v.index() != 5 && v.index() != 10));
        REQUIRE(!v.deleted());
    }
}

TEST_CASE("Shuffle - Shuffle Faces")
{
    vcl::TriMesh mesh;
    for (uint i = 0; i < 10; ++i) {
        mesh.addVertex(Point3d(i, 0, 0));
        mesh.addVertex(Point3d(i, 1, 0));
    }
    for (uint i = 0; i < 8; ++i) {
        mesh.addFace(i, i + 1, i + 2);
    }
    REQUIRE(mesh.faceCount() == 8);

    // 1. Shuffled face index vector
    auto fIndices = vcl::fillAndShuffleFaceIndexVector(mesh);
    static_assert(std::is_same_v<
                  decltype(fIndices),
                  std::vector<vcl::TriMesh::IndexType>>);
    REQUIRE(fIndices.size() == 8);

    std::set<uint> fIdxSet(fIndices.begin(), fIndices.end());
    REQUIRE(fIdxSet.size() == 8);
    for (uint i = 0; i < 8; ++i) {
        REQUIRE(fIdxSet.contains(i));
    }

    // 2. Shuffled face vector (mutable)
    auto fVec = vcl::fillAndShuffleFaceVector(mesh);
    static_assert(std::is_same_v<
                  decltype(fVec),
                  std::vector<vcl::FaceType<vcl::TriMesh>>>);
    REQUIRE(fVec.size() == 8);

    // 3. Shuffled face vector (const)
    const auto& cmesh = mesh;
    auto        cfVec = vcl::fillAndShuffleFaceVector(cmesh);
    static_assert(std::is_same_v<
                  decltype(cfVec),
                  std::vector<vcl::ConstFaceType<vcl::TriMesh>>>);
    REQUIRE(cfVec.size() == 8);

    // 4. Mesh with deleted faces
    mesh.deleteFace(2);
    REQUIRE(mesh.faceCount() == 7);
    auto delFIndices = vcl::fillAndShuffleFaceIndexVector(mesh);
    REQUIRE(delFIndices.size() == 7);
    for (auto idx : delFIndices) {
        REQUIRE(idx != 2);
        REQUIRE(!mesh.face(idx).deleted());
    }
}

TEST_CASE("Shuffle - Shuffle Edges")
{
    MyEdgeMesh mesh = createSampleEdgeMesh();
    REQUIRE(mesh.edgeCount() == 4);

    auto eIndices = vcl::fillAndShuffleEdgeIndexVector(mesh);
    static_assert(
        std::is_same_v<decltype(eIndices), std::vector<MyEdgeMesh::IndexType>>);
    REQUIRE(eIndices.size() == 4);

    auto eVec = vcl::fillAndShuffleEdgeVector(mesh);
    static_assert(
        std::is_same_v<decltype(eVec), std::vector<vcl::EdgeType<MyEdgeMesh>>>);
    REQUIRE(eVec.size() == 4);

    const auto& cmesh = mesh;
    auto        ceVec = vcl::fillAndShuffleEdgeVector(cmesh);
    static_assert(std::is_same_v<
                  decltype(ceVec),
                  std::vector<vcl::ConstEdgeType<MyEdgeMesh>>>);
    REQUIRE(ceVec.size() == 4);
}

TEST_CASE("Shuffle - Shuffle64 Bit")
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
    REQUIRE(vIndices.size() == 15);

    auto fIndices = vcl::fillAndShuffleFaceIndexVector(mesh, 42u);
    static_assert(std::is_same_v<decltype(fIndices), std::vector<uint64_t>>);
    REQUIRE(fIndices.size() == 10);
}
