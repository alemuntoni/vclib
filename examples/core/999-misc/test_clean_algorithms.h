// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CLEAN_ALGORITHMS_H
#define TEST_CLEAN_ALGORITHMS_H

#include "types.h"

#include <vclib/algorithms/mesh/clean.h>
#include <vclib/meshes.h>

#include <cassert>
#include <cmath>
#include <limits>
#include <print>

inline void testRemoveUnreferencedVertices()
{
    MyMesh mesh = createSampleQuadMesh();
    assert(mesh.vertexCount() == 4);

    // Add 2 unreferenced vertices
    mesh.addVertex(Point3f(5.0f, 5.0f, 5.0f));
    mesh.addVertex(Point3f(6.0f, 6.0f, 6.0f));
    assert(mesh.vertexCount() == 6);

    auto removed = vcl::removeUnreferencedVertices(mesh);

    assert(removed == 2);
    assert(mesh.vertexCount() == 4);
    assert(mesh.deletedVertexCount() == 2);
    assert(mesh.vertex(4).deleted());
    assert(mesh.vertex(5).deleted());

    // Calling again should remove 0
    auto removedAgain = vcl::removeUnreferencedVertices(mesh);
    assert(removedAgain == 0);

    // Test with 64-bit mesh
    vcl::TriMesh64 m64;
    m64.addVertices(4);
    m64.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m64.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m64.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m64.vertex(3).position() = Point3d(2.0, 2.0, 2.0); // unreferenced
    m64.addFace(0, 1, 2);

    assert(vcl::unreferencedVertexCount(m64) == 1);
    auto rem64 = vcl::removeUnreferencedVertices(m64);
    static_assert(std::is_same_v<decltype(rem64), uint64_t>);
    assert(rem64 == 1);
    assert(m64.vertexCount() == 3);
    assert(m64.deletedVertexCount() == 1);
    assert(m64.vertex(3).deleted());
}

inline void testRemoveDuplicateVertices()
{
    MyMesh mesh;
    mesh.addVertices(4);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() = Point3f(1.0f, 0.0f, 0.0f); // duplicate of vertex 1!

    mesh.addFace(0, 1, 2);
    mesh.addFace(3, 2, 0);

    auto removed = vcl::removeDuplicateVertices(mesh);

    assert(removed == 1);
    assert(mesh.vertexCount() == 3);
    assert(mesh.deletedVertexCount() == 1);
    assert(mesh.vertex(3).deleted());

    // Face 1 was (3, 2, 0), vertex 3 was merged to vertex 1
    assert(mesh.face(1).vertexIndex(0) == 1);
    assert(mesh.face(1).vertexIndex(1) == 2);
    assert(mesh.face(1).vertexIndex(2) == 0);
}

inline void testRemoveDuplicateFaces()
{
    MyMesh mesh;
    mesh.addVertices(3);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);

    mesh.addFace(0, 1, 2);
    mesh.addFace(1, 2, 0); // same vertices cycle
    mesh.addFace(0, 2, 1); // same vertices inverted

    assert(mesh.faceCount() == 3);

    auto removed = vcl::removeDuplicateFaces(mesh);

    assert(removed == 2);
    assert(mesh.faceCount() == 1);
    assert(mesh.deletedFaceCount() == 2);
}

inline void testRemoveDegenerateVertices()
{
    MyMesh mesh;
    mesh.addVertices(4);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() = Point3f(
        std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f);

    mesh.addFace(0, 1, 2);
    mesh.addFace(0, 1, 3); // incident to NaN vertex

    auto removed = vcl::removeDegenerateVertices(mesh, true);

    assert(removed == 1);
    assert(mesh.vertexCount() == 3);
    assert(mesh.vertex(3).deleted());

    // Face 1 was incident to vertex 3, so deleteAlsoIncidentElements deletes it
    assert(!mesh.face(0).deleted());
    assert(mesh.face(1).deleted());
    assert(mesh.faceCount() == 1);
}

inline void testRemoveDegenerateFaces()
{
    MyMesh mesh;
    mesh.addVertices(3);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);

    mesh.addFace(0, 1, 2); // valid
    mesh.addFace(0, 0, 1); // degenerate: corners 0 and 1 are vertex 0
    mesh.addFace(1, 2, 2); // degenerate: corners 1 and 2 are vertex 2

    assert(mesh.faceCount() == 3);

    auto removed = vcl::removeDegenerateFaces(mesh);

    assert(removed == 2);
    assert(mesh.faceCount() == 1);
    assert(!mesh.face(0).deleted());
    assert(mesh.face(1).deleted());
    assert(mesh.face(2).deleted());
}

inline void testCleanAlgorithms()
{
    std::println("--- Testing Clean Algorithms ---");

    testRemoveUnreferencedVertices();
    testRemoveDuplicateVertices();
    testRemoveDuplicateFaces();
    testRemoveDegenerateVertices();
    testRemoveDegenerateFaces();

    std::println("Clean algorithms verified successfully!\n");
}

#endif // TEST_CLEAN_ALGORITHMS_H

