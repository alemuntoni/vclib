// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_VERTEX_REFERENCES_H
#define TEST_VERTEX_REFERENCES_H

#include "types.h"

inline void testTriangleReferences(MyMesh& mesh)
{
    auto testF = mesh.face(0);
    // testF indices are (0, 1, 2)
    assert(testF.vertexIndexMod(0) == 0);
    assert(testF.vertexIndexMod(3) == 0);
    assert(testF.vertexIndexMod(-1) == 2);
    assert(testF.vertexIndexMod(-3) == 0);

    assert(testF.containsVertex(0));
    assert(testF.containsVertex(1));
    assert(!testF.containsVertex(3));
    assert(testF.containsVertex(mesh.vertex(2)));

    assert(testF.indexOfVertex(0) == 0);
    assert(testF.indexOfVertex(1) == 1);
    assert(testF.indexOfVertex(2) == 2);
    assert(testF.indexOfVertex(3) == vcl::UINT_NULL);

    // Edge (0, 1) -> edge 0
    // Edge (1, 2) -> edge 1
    // Edge (2, 0) -> edge 2
    assert(testF.indexOfEdge(0, 1) == 0);
    assert(testF.indexOfEdge(1, 0) == 0);
    assert(testF.indexOfEdge(1, 2) == 1);
    assert(testF.indexOfEdge(2, 1) == 1);
    assert(testF.indexOfEdge(2, 0) == 2);
    assert(testF.indexOfEdge(0, 2) == 2);
    assert(testF.indexOfEdge(0, 3) == vcl::UINT_NULL);

    // Test setVertexMod
    testF.setVertexMod(-1, 2);

    // Test setVertices range
    std::array<vcl::uint, 3> newIdxs = {0, 1, 2};
    testF.setVertices(newIdxs);
    assert(testF.vertexIndex(0) == 0 && testF.vertexIndex(1) == 1 && testF.vertexIndex(2) == 2);

    // Test setVertices variadic
    testF.setVertices(0, 1, 2);
    assert(testF.vertexIndex(0) == 0 && testF.vertexIndex(1) == 1 && testF.vertexIndex(2) == 2);
}

inline void testPolygonReferences()
{
    PolyMesh polyMesh;
    polyMesh.addVertices(5);

    vcl::uint fi = polyMesh.addFace(0, 1, 2, 3);
    auto f = polyMesh.face(fi);
    assert(f.vertexCount() == 4);

    // containsVertex & indexOfVertex
    assert(f.containsVertex(0));
    assert(f.containsVertex(3));
    assert(!f.containsVertex(4));
    assert(f.indexOfVertex(2) == 2);
    assert(f.indexOfVertex(4) == vcl::UINT_NULL);

    // indexOfEdge
    assert(f.indexOfEdge(0, 1) == 0);
    assert(f.indexOfEdge(1, 0) == 0);
    assert(f.indexOfEdge(1, 2) == 1);
    assert(f.indexOfEdge(2, 3) == 2);
    assert(f.indexOfEdge(3, 0) == 3);
    assert(f.indexOfEdge(0, 3) == 3);
    assert(f.indexOfEdge(0, 2) == vcl::UINT_NULL);

    // Dynamic operations on polygon
    f.pushVertex(4);
    assert(f.vertexCount() == 5);
    assert(f.vertexIndex(4) == 4);
    assert(f.containsVertex(4));

    f.insertVertex(1, 99);
    assert(f.vertexCount() == 6);
    assert(f.vertexIndex(1) == 99);
    assert(f.vertexIndex(2) == 1);

    f.eraseVertex(1);
    assert(f.vertexCount() == 5);
    assert(f.vertexIndex(1) == 1);

    f.clearVertices();
    assert(f.vertexCount() == 0);
}

inline void testVertexReferences()
{
    std::println("--- Testing VertexReferences Component API ---");
    MyMesh mesh = createSampleQuadMesh();

    testTriangleReferences(mesh);
    testPolygonReferences();

    std::println("All VertexReferences methods verified successfully!\n");
}

#endif // TEST_VERTEX_REFERENCES_H
