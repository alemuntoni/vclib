// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

inline void testVertexDeletionAndIteration(MyMesh& mesh)
{
    mesh.deleteVertex(1);

    REQUIRE(mesh.vertexContainerSize() == 4);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.deletedVertexCount() == 1);
    REQUIRE(!mesh.isCompact());

    // Iterating over active vertices (vertex 1 automatically skipped)
    std::size_t activeCount = 0;
    for (auto v : mesh.vertices()) {
        REQUIRE(v.index() != 1);
        REQUIRE(!v.deleted());
        ++activeCount;
    }
    REQUIRE(activeCount == 3);

    // Iterating over ALL vertices (including deleted, jumpDeleted = false)
    std::size_t allCount = 0;
    for (auto v : mesh.vertices(false)) {
        if (v.index() == 1) {
            REQUIRE(v.deleted());
        }
        ++allCount;
    }
    REQUIRE(allCount == 4);
}

inline void testComponentViewWithDeleted(MyMesh& mesh)
{
    // Test ComponentView jumping deleted elements
    REQUIRE(mesh.vertices().positions().size() == 3);
    REQUIRE(mesh.vertices(false).positions().size() == 4);

    std::size_t activePosCount = 0;
    for (auto& p : mesh.vertices().positions()) {
        (void) p;
        ++activePosCount;
    }
    REQUIRE(activePosCount == 3);

    std::size_t allPosCount = 0;
    for (auto& p : mesh.vertices(false).positions()) {
        (void) p;
        ++allPosCount;
    }
    REQUIRE(allPosCount == 4);
}

inline void testMeshCompaction(MyMesh& mesh)
{
    REQUIRE(!mesh.isCompact());
    mesh.compact();

    REQUIRE(mesh.isCompact());
    REQUIRE(mesh.vertexContainerSize() == 3);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.deletedVertexCount() == 0);

    using VertexIndex = MyMesh::Vertex::IndexType;
    auto formatVIdx   = [](VertexIndex idx) {
        return idx == MyMesh::NULL_INDEX ? "NULL" : std::to_string(idx);
    };

    // Vertex 1 was deleted, so Face 0 corner 1 is now NULL
    REQUIRE(mesh.face(0).vertexIndex(1) == MyMesh::NULL_INDEX);
    REQUIRE(!mesh.face(0).vertex(1).isValid());
    // Corner 0 is vertex 0
    REQUIRE(mesh.face(0).vertexIndex(0) == 0);
    REQUIRE(mesh.face(0).vertex(0).isValid());
}

inline void testFaceDeletionAndCompaction(MyMesh& mesh)
{
    // Test face deletion and compactFaces
    mesh.deleteFace(0);
    REQUIRE(!mesh.isCompact());
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.faceContainerSize() == 2);

    mesh.compactFaces();
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.faceContainerSize() == 1);
}

TEST_CASE("Deletion Compaction - 64 Bit Null Index And Compaction")
{
    Mesh64 m;
    m.addVertices(4);
    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    REQUIRE(m.vertex(0).index() != Mesh64::NULL_INDEX);
    REQUIRE(m.face(0).vertexIndex(0) == 0);

    // Delete vertex 1
    m.deleteVertex(1);
    REQUIRE(m.vertex(1).deleted());

    // Compact vertices
    m.compactVertices();
    REQUIRE(m.vertexCount() == 3);

    // Vertex 1 was deleted, so Face 0 corner 1 must be Mesh64::NULL_INDEX, NOT
    // 32-bit UINT_NULL
    REQUIRE(m.face(0).vertexIndex(1) == Mesh64::NULL_INDEX);
    REQUIRE(m.face(0).vertexIndex(1) != vcl::UINT_NULL);
    REQUIRE(!m.face(0).vertex(1).isValid());
}

TEST_CASE("Deletion Compaction")
{
    MyMesh mesh = createSampleQuadMesh();

    testVertexDeletionAndIteration(mesh);
    testComponentViewWithDeleted(mesh);
    testMeshCompaction(mesh);
    testFaceDeletionAndCompaction(mesh);
}
