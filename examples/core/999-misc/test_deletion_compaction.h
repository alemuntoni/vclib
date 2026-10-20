// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_DELETION_COMPACTION_H
#define TEST_DELETION_COMPACTION_H

#include "types.h"

inline void testVertexDeletionAndIteration(MyMesh& mesh)
{
    mesh.deleteVertex(1);

    assert(mesh.vertexContainerSize() == 4);
    assert(mesh.vertexCount() == 3);
    assert(mesh.deletedVertexCount() == 1);
    assert(!mesh.isCompact());

    // Iterating over active vertices (vertex 1 automatically skipped)
    std::size_t activeCount = 0;
    for (auto v : mesh.vertices()) {
        assert(v.index() != 1);
        assert(!v.deleted());
        ++activeCount;
    }
    assert(activeCount == 3);

    // Iterating over ALL vertices (including deleted, jumpDeleted = false)
    std::size_t allCount = 0;
    for (auto v : mesh.vertices(false)) {
        if (v.index() == 1) {
            assert(v.deleted());
        }
        ++allCount;
    }
    assert(allCount == 4);
}

inline void testComponentViewWithDeleted(MyMesh& mesh)
{
    // Test ComponentView jumping deleted elements
    assert(mesh.vertices().positions().size() == 3);
    assert(mesh.vertices(false).positions().size() == 4);

    std::size_t activePosCount = 0;
    for (auto& p : mesh.vertices().positions()) {
        (void)p;
        ++activePosCount;
    }
    assert(activePosCount == 3);

    std::size_t allPosCount = 0;
    for (auto& p : mesh.vertices(false).positions()) {
        (void)p;
        ++allPosCount;
    }
    assert(allPosCount == 4);
}

inline void testMeshCompaction(MyMesh& mesh)
{
    assert(!mesh.isCompact());
    mesh.compact();

    assert(mesh.isCompact());
    assert(mesh.vertexContainerSize() == 3);
    assert(mesh.vertexCount() == 3);
    assert(mesh.deletedVertexCount() == 0);

    using VertexIndex = MyMesh::Vertex::IndexType;
    auto formatVIdx = [](VertexIndex idx) {
        return idx == std::numeric_limits<VertexIndex>::max() ? "NULL" : std::to_string(idx);
    };

    // Vertex 1 was deleted, so Face 0 corner 1 is now NULL
    assert(mesh.face(0).vertexIndex(1) == vcl::UINT_NULL);
    assert(!mesh.face(0).vertex(1).isValid());
    // Corner 0 is vertex 0
    assert(mesh.face(0).vertexIndex(0) == 0);
    assert(mesh.face(0).vertex(0).isValid());
}

inline void testFaceDeletionAndCompaction(MyMesh& mesh)
{
    // Test face deletion and compactFaces
    mesh.deleteFace(0);
    assert(!mesh.isCompact());
    assert(mesh.faceCount() == 1);
    assert(mesh.faceContainerSize() == 2);

    mesh.compactFaces();
    assert(mesh.faceCount() == 1);
    assert(mesh.faceContainerSize() == 1);
}

inline void testDeletionCompaction()
{
    std::println("--- Testing Deletion & Compaction ---");
    MyMesh mesh = createSampleQuadMesh();

    testVertexDeletionAndIteration(mesh);
    testComponentViewWithDeleted(mesh);
    testMeshCompaction(mesh);
    testFaceDeletionAndCompaction(mesh);

    std::println("Deletion and compaction verified successfully!\n");
}

#endif // TEST_DELETION_COMPACTION_H
