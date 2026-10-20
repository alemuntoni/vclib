// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_APPEND_H
#define TEST_APPEND_H

#include "types.h"

inline void testBasicAppend()
{
    MyMesh m1 = createSampleQuadMesh(); // 4 vertices, 2 faces
    MyMesh m2 = createSampleQuadMesh(); // 4 vertices, 2 faces

    // Offset positions and change face colors of m2
    for (auto v : m2.vertices()) {
        v.position() += Point3f(10.0f, 0.0f, 0.0f);
    }
    m2.face(0).color() = Color::Green;
    m2.face(1).color() = Color::Cyan;

    // Append m2 to m1
    m1.append(m2);

    assert(m1.vertexCount() == 8);
    assert(m1.vertexContainerSize() == 8);
    assert(m1.faceCount() == 4);
    assert(m1.faceContainerSize() == 4);

    // Initial m1 faces: (0, 1, 2) and (1, 3, 2)
    assert(m1.face(0).vertexIndex(0) == 0 && m1.face(0).vertexIndex(1) == 1 && m1.face(0).vertexIndex(2) == 2);
    assert(m1.face(1).vertexIndex(0) == 1 && m1.face(1).vertexIndex(1) == 3 && m1.face(1).vertexIndex(2) == 2);

    // Appended m2 faces: shifted by 4 -> (4, 5, 6) and (5, 7, 6)
    assert(m1.face(2).vertexIndex(0) == 4 && m1.face(2).vertexIndex(1) == 5 && m1.face(2).vertexIndex(2) == 6);
    assert(m1.face(3).vertexIndex(0) == 5 && m1.face(3).vertexIndex(1) == 7 && m1.face(3).vertexIndex(2) == 6);

    // Check positions
    assert(m1.face(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(m1.face(2).vertex(0).position() == Point3f(10.0f, 0.0f, 1.0f));

    // Check colors
    assert(m1.face(0).color() == Color::DarkRed);
    assert(m1.face(1).color() == Color::DarkBlue);
    assert(m1.face(2).color() == Color::Green);
    assert(m1.face(3).color() == Color::Cyan);

    // Mesh color preserved
    assert(m1.color() == Color::Red);
}

inline void testAppendWithDeletedElements()
{
    MyMesh m1 = createSampleQuadMesh();
    MyMesh m2 = createSampleQuadMesh();

    // Delete vertex 1 and face 0 in m2
    m2.deleteVertex(1);
    m2.deleteFace(0);
    assert(m2.vertexCount() == 3);
    assert(m2.deletedVertexCount() == 1);
    assert(m2.faceCount() == 1);
    assert(m2.deletedFaceCount() == 1);

    m1.append(m2);

    assert(m1.vertexContainerSize() == 8);
    assert(m1.vertexCount() == 7);
    assert(m1.deletedVertexCount() == 1);

    assert(m1.faceContainerSize() == 4);
    assert(m1.faceCount() == 3);
    assert(m1.deletedFaceCount() == 1);

    // Deleted element in m2 (vertex index 1) is now at index 4 + 1 = 5
    assert(m1.vertex(5).deleted());
    assert(!m1.vertex(4).deleted());
    assert(!m1.vertex(6).deleted());

    // Deleted face in m2 (face index 0) is now at index 2 + 0 = 2
    assert(m1.face(2).deleted());
    assert(!m1.face(0).deleted());
    assert(!m1.face(1).deleted());
    assert(!m1.face(3).deleted());

    // Verify iteration skips deleted elements
    std::size_t activeVCount = 0;
    for (auto v : m1.vertices()) {
        assert(!v.deleted());
        ++activeVCount;
    }
    assert(activeVCount == 7);

    std::size_t activeFCount = 0;
    for (auto f : m1.faces()) {
        assert(!f.deleted());
        ++activeFCount;
    }
    assert(activeFCount == 3);
}

inline void testSelfAppend()
{
    MyMesh m = createSampleQuadMesh();
    m.append(m); // Self-append

    assert(m.vertexCount() == 8);
    assert(m.vertexContainerSize() == 8);
    assert(m.faceCount() == 4);
    assert(m.faceContainerSize() == 4);

    assert(m.face(2).vertexIndex(0) == 4 && m.face(2).vertexIndex(1) == 5 && m.face(2).vertexIndex(2) == 6);
    assert(m.face(3).vertexIndex(0) == 5 && m.face(3).vertexIndex(1) == 7 && m.face(3).vertexIndex(2) == 6);
}

inline void testAppend()
{
    std::println("--- Testing Mesh::append ---");
    testBasicAppend();
    testAppendWithDeletedElements();
    testSelfAppend();
    std::println("Mesh::append verified successfully!\n");
}

#endif // TEST_APPEND_H
