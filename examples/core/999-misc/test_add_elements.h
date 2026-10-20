// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ADD_ELEMENTS_H
#define TEST_ADD_ELEMENTS_H

#include "types.h"

inline void testAddVertex(MyMesh& mesh)
{
    // addVertex() and addVertex(pos)
    vcl::uint v0 = mesh.addVertex();
    mesh.vertex(v0).position() = Point3f(0.0f, 0.0f, 0.0f);
    assert(mesh.vertexCount() == 1);
    assert(mesh.vertex(v0).position() == Point3f(0.0f, 0.0f, 0.0f));

    vcl::uint v1 = mesh.addVertex(Point3f(1.0f, 0.0f, 0.0f));
    assert(mesh.vertexCount() == 2);
    assert(mesh.vertex(v1).position() == Point3f(1.0f, 0.0f, 0.0f));
}

inline void testAddVertices(MyMesh& mesh)
{
    // 1. addVertices(count)
    vcl::uint baseV = mesh.addVertices(2);
    assert(baseV == 2);
    assert(mesh.vertexCount() == 4);
    mesh.vertex(baseV).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(baseV + 1).position() = Point3f(1.0f, 1.0f, 0.0f);

    // 2. addVertices(variadic positions)
    vcl::uint baseVVar = mesh.addVertices(Point3f(2.0f, 0.0f, 0.0f), Point3f(2.0f, 1.0f, 0.0f));
    assert(baseVVar == 4);
    assert(mesh.vertexCount() == 6);
    assert(mesh.vertex(4).position() == Point3f(2.0f, 0.0f, 0.0f));
    assert(mesh.vertex(5).position() == Point3f(2.0f, 1.0f, 0.0f));

    // 3. addVertices(range of positions)
    std::vector<Point3f> newPos = {Point3f(3.0f, 0.0f, 0.0f), Point3f(3.0f, 1.0f, 0.0f)};
    vcl::uint baseVRng = mesh.addVertices(newPos);
    assert(baseVRng == 6);
    assert(mesh.vertexCount() == 8);
    assert(mesh.vertex(6).position() == Point3f(3.0f, 0.0f, 0.0f));
    assert(mesh.vertex(7).position() == Point3f(3.0f, 1.0f, 0.0f));
}

inline void testAddFace(MyMesh& mesh)
{
    // 1. addFace() and addFace(indices)
    vcl::uint addF0 = mesh.addFace();
    mesh.face(addF0).setVertices(0, 1, 2);
    assert(mesh.faceCount() == 1);
    assert(mesh.face(addF0).vertexIndex(0) == 0 && mesh.face(addF0).vertexIndex(1) == 1 && mesh.face(addF0).vertexIndex(2) == 2);

    vcl::uint addF1 = mesh.addFace(1, 3, 2);
    assert(mesh.faceCount() == 2);
    assert(mesh.face(addF1).vertexIndex(0) == 1 && mesh.face(addF1).vertexIndex(1) == 3 && mesh.face(addF1).vertexIndex(2) == 2);

    // 2. addFace(vertex proxies)
    vcl::uint addF2 = mesh.addFace(mesh.vertex(0), mesh.vertex(1), mesh.vertex(3));
    assert(mesh.faceCount() == 3);
    assert(mesh.face(addF2).vertexIndex(0) == 0 && mesh.face(addF2).vertexIndex(1) == 1 && mesh.face(addF2).vertexIndex(2) == 3);

    // 3. addFace(range of indices)
    std::vector<vcl::uint> faceIndices = {4, 5, 2};
    vcl::uint addF3 = mesh.addFace(faceIndices);
    assert(mesh.faceCount() == 4);
    assert(mesh.face(addF3).vertexIndex(0) == 4 && mesh.face(addF3).vertexIndex(1) == 5 && mesh.face(addF3).vertexIndex(2) == 2);
}

inline void testAddFaces(MyMesh& mesh)
{
    // 1. addFaces(count)
    vcl::uint baseF = mesh.addFaces(2);
    assert(baseF == 4);
    assert(mesh.faceCount() == 6);

    // 2. addFaces(range of ranges)
    std::vector<std::vector<vcl::uint>> multiFaces = {{6, 7, 4}, {7, 5, 4}};
    vcl::uint baseFMulti = mesh.addFaces(multiFaces);
    assert(baseFMulti == 6);
    assert(mesh.faceCount() == 8);
    assert(mesh.face(6).vertexIndex(0) == 6 && mesh.face(6).vertexIndex(1) == 7 && mesh.face(6).vertexIndex(2) == 4);
    assert(mesh.face(7).vertexIndex(0) == 7 && mesh.face(7).vertexIndex(1) == 5 && mesh.face(7).vertexIndex(2) == 4);
}

inline void testAddPolygonFaces()
{
    PolyMesh polyMesh;
    polyMesh.addVertices(Point3f(0.0f, 0.0f, 0.0f), Point3f(1.0f, 0.0f, 0.0f),
                         Point3f(1.0f, 1.0f, 0.0f), Point3f(0.0f, 1.0f, 0.0f),
                         Point3f(0.5f, 1.5f, 0.0f));
    assert(polyMesh.vertexCount() == 5);

    // Quad face
    vcl::uint polyF0 = polyMesh.addFace(0, 1, 2, 3);
    assert(polyMesh.face(polyF0).vertexCount() == 4);
    assert(polyMesh.face(polyF0).vertexIndex(0) == 0);
    assert(polyMesh.face(polyF0).vertexIndex(3) == 3);

    // Pentagon face from range
    std::vector<vcl::uint> pentagon = {0, 1, 2, 4, 3};
    vcl::uint polyF1 = polyMesh.addFace(pentagon);
    assert(polyMesh.face(polyF1).vertexCount() == 5);
    assert(polyMesh.face(polyF1).vertexIndex(3) == 4);
}

inline void testAddElements()
{
    std::println("--- Testing addVertex/addVertices & addFace/addFaces ---");
    MyMesh mesh;

    testAddVertex(mesh);
    testAddVertices(mesh);
    testAddFace(mesh);
    testAddFaces(mesh);
    testAddPolygonFaces();

    std::println("addVertex, addVertices, addFace, addFaces verified successfully!\n");
}

#endif // TEST_ADD_ELEMENTS_H
