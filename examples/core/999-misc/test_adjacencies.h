// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ADJACENCIES_H
#define TEST_ADJACENCIES_H

#include "types.h"

#include <cassert>
#include <print>
#include <vector>

inline void testAdjacencies()
{
    std::println("--- Testing Adjacencies Component (AdjacentFaces, AdjacentVertices, AdjacentEdges) ---");

    AdjMesh m;
    m.resizeVertices(4);
    m.resizeFaces(2);

    m.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    m.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    m.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    m.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    m.face(0).setVertices(0, 1, 2);
    m.face(1).setVertices(1, 3, 2);

    // 1. Static face adjacencies (TriangleAdjacentFaces: size 3)
    auto f0 = m.face(0);
    auto f1 = m.face(1);

    assert(f0.adjFaceCount() == 3);
    assert(f1.adjFaceCount() == 3);

    // Initially NULL_INDEX
    assert(f0.adjFaceIndex(0) == vcl::NULL_INDEX<uint>);
    assert(f0.adjFaceIndex(1) == vcl::NULL_INDEX<uint>);
    assert(f0.adjFaceIndex(2) == vcl::NULL_INDEX<uint>);

    // Set face 0's edge 1 adjacent to face 1, face 1's edge 2 adjacent to face 0
    f0.setAdjFace(1, 1);
    f1.setAdjFace(2, f0); // using proxy

    assert(f0.adjFaceIndex(1) == 1);
    assert(f0.adjFace(1).index() == 1);
    assert(f1.adjFaceIndex(2) == 0);
    assert(f1.adjFace(2).index() == 0);

    // Modular access
    assert(f0.adjFaceIndexMod(1) == 1);
    assert(f0.adjFaceIndexMod(4) == 1); // 4 % 3 = 1
    assert(f0.adjFaceIndexMod(-2) == 1); // (-2 % 3 + 3) % 3 = 1
    assert(f0.adjFaceMod(4).index() == 1);
    assert(f0.adjFaceMod(-2).index() == 1);

    // setAdjFaceMod
    f0.setAdjFaceMod(-2, 1);
    assert(f0.adjFaceIndex(1) == 1);

    // Search: containsAdjFace & indexOfAdjFace
    assert(f0.containsAdjFace(1));
    assert(f0.containsAdjFace(f1));
    assert(!f0.containsAdjFace(99));
    assert(f0.indexOfAdjFace(1) == 1);
    assert(f0.indexOfAdjFace(f1) == 1);
    assert(f0.indexOfAdjFace(99) == UINT_NULL);

    // Range-based iteration
    uint count = 0;
    for (uint fi : f0.adjFaceIndices()) {
        if (count == 1) {
            assert(fi == 1);
        } else {
            assert(fi == vcl::NULL_INDEX<uint>);
        }
        ++count;
    }
    assert(count == 3);

    // 2. Dynamic vertex adjacencies (AdjacentFaces and AdjacentVertices on Vertex)
    auto v0 = m.vertex(0);
    auto v1 = m.vertex(1);
    auto v2 = m.vertex(2);
    auto v3 = m.vertex(3);

    assert(v0.adjFaceCount() == 0);
    assert(v0.adjVertexCount() == 0);

    // Dynamic operations on Vertex AdjacentFaces
    v0.pushAdjFace(0);
    assert(v0.adjFaceCount() == 1);
    assert(v0.adjFaceIndex(0) == 0);
    assert(v0.adjFace(0).index() == 0);

    v1.pushAdjFace(0);
    v1.pushAdjFace(f1);
    assert(v1.adjFaceCount() == 2);
    assert(v1.adjFaceIndex(0) == 0);
    assert(v1.adjFaceIndex(1) == 1);

    v2.resizeAdjFaces(2);
    v2.setAdjFace(0, 0);
    v2.setAdjFace(1, 1);
    assert(v2.adjFaceCount() == 2);

    v3.pushAdjFace(1);
    assert(v3.adjFaceCount() == 1);

    // Test insert, erase, clear on vertex adjacencies
    v0.insertAdjFace(0, 42);
    assert(v0.adjFaceCount() == 2);
    assert(v0.adjFaceIndex(0) == 42);
    assert(v0.adjFaceIndex(1) == 0);

    v0.eraseAdjFace(0);
    assert(v0.adjFaceCount() == 1);
    assert(v0.adjFaceIndex(0) == 0);

    // Dynamic operations on Vertex AdjacentVertices
    v0.pushAdjVertex(v1);
    v0.pushAdjVertex(2);
    assert(v0.adjVertexCount() == 2);
    assert(v0.adjVertexIndex(0) == 1);
    assert(v0.adjVertexIndex(1) == 2);
    assert(v0.adjVertex(0).index() == 1);
    assert(v0.adjVertex(1).index() == 2);

    std::vector<uint> v1_adj_verts = {0, 2, 3};
    v1.setAdjVertices(v1_adj_verts);
    assert(v1.adjVertexCount() == 3);
    assert(v1.adjVertexIndex(0) == 0);
    assert(v1.adjVertexIndex(1) == 2);
    assert(v1.adjVertexIndex(2) == 3);

    // Iterate through adjVertices()
    uint adjCount = 0;
    for (auto adjV : v1.adjVertices()) {
        assert(adjV.index() == v1_adj_verts[adjCount]);
        ++adjCount;
    }
    assert(adjCount == 3);

    // 3. Compaction test
    AdjMesh mComp = m;
    assert(mComp.vertex(1).adjFaceIndex(0) == 0);
    assert(mComp.vertex(1).adjFaceIndex(1) == 1);
    assert(mComp.face(0).adjFaceIndex(1) == 1);
    assert(mComp.face(1).adjFaceIndex(2) == 0);

    // Delete face 0 and compact
    mComp.deleteFace(0);
    mComp.compact();
    assert(mComp.faceCount() == 1);
    // Face 1 became Face 0.
    // Face 0's adjFace at index 2 pointed to old Face 0 (which was deleted).
    assert(mComp.face(0).adjFaceIndex(2) == vcl::NULL_INDEX<uint>);

    // 4. Append test
    AdjMesh mApp1 = m;
    AdjMesh mApp2 = m;
    uint initVCount = mApp1.vertexCount();
    uint initFCount = mApp1.faceCount();

    mApp1.append(mApp2);
    assert(mApp1.vertexCount() == initVCount * 2);
    assert(mApp1.faceCount() == initFCount * 2);

    // Face in second half (old face 0, now face initFCount) should have its adjFace index shifted by initFCount!
    assert(mApp1.face(initFCount).adjFaceIndex(1) == 1 + initFCount);
    assert(mApp1.face(initFCount + 1).adjFaceIndex(2) == 0 + initFCount);

    // Vertex in second half had adjFace {0, 1} and adjVertex {0, 2, 3}:
    auto vApp2_1 = mApp1.vertex(initVCount + 1);
    assert(vApp2_1.adjFaceIndex(0) == 0 + initFCount);
    assert(vApp2_1.adjFaceIndex(1) == 1 + initFCount);
    assert(vApp2_1.adjVertexIndex(0) == 0 + initVCount);
    assert(vApp2_1.adjVertexIndex(1) == 2 + initVCount);
    assert(vApp2_1.adjVertexIndex(2) == 3 + initVCount);

    // 5. Optional component test (AdjacentEdges on Vertex and Face)
    assert(f0.isAdjacentFacesAvailable());
    assert(m.vertex(0).isAdjacentFacesAvailable());
    assert(m.vertex(0).isAdjacentVerticesAvailable());
    assert(!m.vertex(0).isAdjacentEdgesAvailable());
    assert(!m.face(0).isAdjacentEdgesAvailable());
    assert((!m.isElementComponentAvailable<ElemId::VERTEX, CompId::ADJACENT_EDGES>()));
    assert((!m.isElementComponentAvailable<ElemId::FACE, CompId::ADJACENT_EDGES>()));

    m.enableElementComponent<ElemId::VERTEX, CompId::ADJACENT_EDGES>();
    m.enableElementComponent<ElemId::FACE, CompId::ADJACENT_EDGES>();

    assert(m.vertex(0).isAdjacentEdgesAvailable());
    assert(m.face(0).isAdjacentEdgesAvailable());
    assert((m.isElementComponentAvailable<ElemId::VERTEX, CompId::ADJACENT_EDGES>()));
    assert((m.isElementComponentAvailable<ElemId::FACE, CompId::ADJACENT_EDGES>()));

    m.vertex(0).pushAdjEdge(10);
    assert(m.vertex(0).adjEdgeCount() == 1);
    assert(m.vertex(0).adjEdgeIndex(0) == 10);

    m.face(0).setAdjEdge(0, 100);
    assert(m.face(0).adjEdgeIndex(0) == 100);

    std::println("Adjacencies component tests passed successfully!");
}

#endif // TEST_ADJACENCIES_H
