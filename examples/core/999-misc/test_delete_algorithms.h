// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_DELETE_ALGORITHMS_H
#define TEST_DELETE_ALGORITHMS_H

#include "types.h"

#include <vclib/algorithms/mesh/delete.h>

#include <cassert>
#include <print>
#include <type_traits>
#include <vector>

inline void testDeleteVerticesIfFunction()
{
    // Quad mesh has 4 vertices:
    // v0: (0, 0, 1), v1: (1, 0, 0), v2: (0, 1, 0), v3: (1, 1, 0)
    // Face 0: (0, 1, 2), Face 1: (1, 3, 2)
    MyMesh mesh = createSampleQuadMesh();

    // Delete vertices with x > 0.5 (vertices 1 and 3)
    auto count = vcl::deleteVerticesIf(
        mesh, [](auto v) { return v.position().x() > 0.5f; });

    static_assert(std::is_same_v<decltype(count), MyMesh::IndexType>);
    assert(count == 2);
    assert(mesh.vertexCount() == 2);
    assert(mesh.deletedVertexCount() == 2);
    assert(mesh.vertex(1).deleted());
    assert(mesh.vertex(3).deleted());
    assert(!mesh.vertex(0).deleted());
    assert(!mesh.vertex(2).deleted());

    // Check that face vertex references to deleted vertices are invalidated (NULL_INDEX)
    assert(mesh.face(0).vertexIndex(0) == 0);
    assert(mesh.face(0).vertexIndex(1) == MyMesh::NULL_INDEX);
    assert(mesh.face(0).vertexIndex(2) == 2);

    assert(mesh.face(1).vertexIndex(0) == MyMesh::NULL_INDEX);
    assert(mesh.face(1).vertexIndex(1) == MyMesh::NULL_INDEX);
    assert(mesh.face(1).vertexIndex(2) == 2);
}

inline void testDeleteVerticesIfRange()
{
    MyMesh mesh = createSampleQuadMesh();

    // Range of booleans for valid vertices (one entry per valid vertex: 0, 1, 2, 3)
    std::vector<bool> filter = {true, false, false, false}; // delete vertex 0

    auto count = vcl::deleteVerticesIf(mesh, filter);

    assert(count == 1);
    assert(mesh.vertexCount() == 3);
    assert(mesh.deletedVertexCount() == 1);
    assert(mesh.vertex(0).deleted());
    assert(!mesh.vertex(1).deleted());

    // Face 0 had vertex 0, so corner 0 becomes NULL_INDEX
    assert(mesh.face(0).vertexIndex(0) == MyMesh::NULL_INDEX);
    assert(mesh.face(0).vertexIndex(1) == 1);
    assert(mesh.face(0).vertexIndex(2) == 2);

    // Face 1 had vertices (1, 3, 2), unaffected
    assert(mesh.face(1).vertexIndex(0) == 1);
    assert(mesh.face(1).vertexIndex(1) == 3);
    assert(mesh.face(1).vertexIndex(2) == 2);
}

inline void testDeleteFacesIfFunction()
{
    MyMesh mesh = createSampleQuadMesh();

    // Delete face 0
    auto count = vcl::deleteFacesIf(mesh, [](auto f) { return f.index() == 0; });

    static_assert(std::is_same_v<decltype(count), MyMesh::IndexType>);
    assert(count == 1);
    assert(mesh.faceCount() == 1);
    assert(mesh.deletedFaceCount() == 1);
    assert(mesh.face(0).deleted());
    assert(!mesh.face(1).deleted());
}

inline void testDeleteFacesIfRange()
{
    MyMesh mesh = createSampleQuadMesh();

    // Range for valid faces: delete face 1
    std::vector<bool> filter = {false, true};

    auto count = vcl::deleteFacesIf(mesh, filter);

    assert(count == 1);
    assert(mesh.faceCount() == 1);
    assert(mesh.deletedFaceCount() == 1);
    assert(!mesh.face(0).deleted());
    assert(mesh.face(1).deleted());
}

inline void testDeleteEdgesIfFunction()
{
    // Sample edge mesh with 4 edges: (0, 1), (1, 3), (3, 2), (2, 0)
    MyEdgeMesh mesh = createSampleEdgeMesh();

    // Delete edges with odd indices (edges 1 and 3)
    auto count = vcl::deleteEdgesIf(
        mesh, [](auto e) { return e.index() % 2 == 1; });

    static_assert(std::is_same_v<decltype(count), MyEdgeMesh::IndexType>);
    assert(count == 2);
    assert(mesh.edgeCount() == 2);
    assert(mesh.deletedEdgeCount() == 2);
    assert(!mesh.edge(0).deleted());
    assert(mesh.edge(1).deleted());
    assert(!mesh.edge(2).deleted());
    assert(mesh.edge(3).deleted());
}

inline void testDeleteEdgesIfRange()
{
    MyEdgeMesh mesh = createSampleEdgeMesh();

    std::vector<bool> filter = {true, false, false, true}; // delete edge 0 and 3

    auto count = vcl::deleteEdgesIf(mesh, filter);

    assert(count == 2);
    assert(mesh.edgeCount() == 2);
    assert(mesh.deletedEdgeCount() == 2);
    assert(mesh.edge(0).deleted());
    assert(!mesh.edge(1).deleted());
    assert(!mesh.edge(2).deleted());
    assert(mesh.edge(3).deleted());
}

inline void testDeleteWithPriorDeletedElements()
{
    // Test on mesh that already has deleted elements before deleteVerticesIf
    MyMesh mesh = createSampleQuadMesh();

    // Pre-delete vertex 1
    mesh.deleteVertex(1);
    assert(mesh.vertexCount() == 3);

    // Filter over remaining valid vertices (0, 2, 3)
    // Vertex 2 has y = 1.0f, Vertex 3 has y = 1.0f
    auto count = vcl::deleteVerticesIf(
        mesh, [](auto v) { return v.position().y() > 0.5f; });

    assert(count == 2);
    assert(mesh.vertexCount() == 1); // Only vertex 0 remains
    assert(mesh.deletedVertexCount() == 3);
    assert(mesh.vertex(1).deleted());
    assert(mesh.vertex(2).deleted());
    assert(mesh.vertex(3).deleted());
    assert(!mesh.vertex(0).deleted());

    // Also test Range overload on a mesh with prior deleted elements:
    MyMesh mesh2 = createSampleQuadMesh();
    mesh2.deleteVertex(0); // Valid vertices are 1, 2, 3

    // Range has size 3 (one for each valid vertex: 1, 2, 3)
    std::vector<bool> filter = {false, true, false}; // delete vertex 2
    auto count2 = vcl::deleteVerticesIf(mesh2, filter);

    assert(count2 == 1);
    assert(mesh2.vertexCount() == 2);
    assert(mesh2.vertex(0).deleted());
    assert(mesh2.vertex(2).deleted());
    assert(!mesh2.vertex(1).deleted());
    assert(!mesh2.vertex(3).deleted());
}

inline void testDelete64BitMesh()
{
    Mesh64 m;
    m.addVertices(4);
    m.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m.vertex(3).position() = Point3d(1.0, 1.0, 0.0);

    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    auto count = vcl::deleteVerticesIf(
        m, [](auto v) { return v.index() == 1; });

    static_assert(std::is_same_v<decltype(count), uint64_t>);
    assert(count == 1);
    assert(m.vertexCount() == 3);
    assert(m.face(0).vertexIndex(1) == Mesh64::NULL_INDEX);
    assert(m.face(1).vertexIndex(0) == Mesh64::NULL_INDEX);
}

inline void testDeleteAllOrNone()
{
    MyMesh mesh = createSampleQuadMesh();

    // Delete none
    auto countNone = vcl::deleteVerticesIf(mesh, [](auto) { return false; });
    assert(countNone == 0);
    assert(mesh.vertexCount() == 4);

    // Delete all
    auto countAll = vcl::deleteVerticesIf(mesh, [](auto) { return true; });
    assert(countAll == 4);
    assert(mesh.vertexCount() == 0);
    assert(mesh.deletedVertexCount() == 4);

    // Face corners should all be NULL_INDEX
    assert(mesh.face(0).vertexIndex(0) == MyMesh::NULL_INDEX);
    assert(mesh.face(0).vertexIndex(1) == MyMesh::NULL_INDEX);
    assert(mesh.face(0).vertexIndex(2) == MyMesh::NULL_INDEX);
}

inline void testDeleteAlgorithms()
{
    std::println("--- Testing Delete Algorithms ---");

    testDeleteVerticesIfFunction();
    testDeleteVerticesIfRange();
    testDeleteFacesIfFunction();
    testDeleteFacesIfRange();
    testDeleteEdgesIfFunction();
    testDeleteEdgesIfRange();
    testDeleteWithPriorDeletedElements();
    testDelete64BitMesh();
    testDeleteAllOrNone();

    std::println("Delete algorithms verified successfully!\n");
}

#endif // TEST_DELETE_ALGORITHMS_H
