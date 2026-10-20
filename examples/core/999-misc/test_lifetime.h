// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_LIFETIME_H
#define TEST_LIFETIME_H

#include "types.h"

inline void testElementValidity(MyMesh& mesh)
{
    MyMesh::Vertex validV = mesh.vertex(0);
    MyMesh::Vertex invalidV;

    assert((bool)validV);
    assert(validV.isValid());
    assert(!(bool)invalidV);
    assert(!invalidV.isValid());

    MyMesh::Face validF = mesh.face(0);
    MyMesh::Face invalidF;
    assert((bool)validF);
    assert(validF.isValid());
    assert(!(bool)invalidF);
    assert(!invalidF.isValid());
}

inline void testMeshCopy(const MyMesh& mesh)
{
    MyMesh meshCopy = mesh;
    assert(meshCopy.color() == Color::Red);
    assert(meshCopy.vertexCount() == mesh.vertexCount());
    assert(meshCopy.faceCount() == mesh.faceCount());
    assert(meshCopy.face(0).vertex(0).position() == mesh.face(0).vertex(0).position());

    // Deep copy check: modifying copy must not affect original
    meshCopy.vertex(0).position() = Point3f(99.0f, 99.0f, 99.0f);
    assert(mesh.vertex(0).position() != Point3f(99.0f, 99.0f, 99.0f));
}

inline void testMeshMove(MyMesh meshToMove)
{
    std::size_t origVCount = meshToMove.vertexCount();
    MyMesh meshMoved = std::move(meshToMove);
    assert(meshMoved.color() == Color::Red);
    assert(meshMoved.vertexCount() == origVCount);
    assert(meshMoved.face(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
}

inline void testMeshSwap(MyMesh& mesh)
{
    std::size_t origVCount = mesh.vertexCount();
    MyMesh meshSwapped;
    swap(mesh, meshSwapped);
    assert(meshSwapped.color() == Color::Red);
    assert(meshSwapped.vertexCount() == origVCount);
    assert(mesh.vertexCount() == 0);

    // Swap back
    swap(mesh, meshSwapped);
    assert(mesh.vertexCount() == origVCount);
}

inline void testMeshAssignment(const MyMesh& mesh)
{
    MyMesh meshAssigned;
    meshAssigned = mesh; // Copy assignment
    assert(meshAssigned.color() == Color::Red);
    assert(meshAssigned.vertexCount() == mesh.vertexCount());

    MyMesh meshMoveAssigned;
    meshMoveAssigned = std::move(meshAssigned); // Move assignment
    assert(meshMoveAssigned.color() == Color::Red);
    assert(meshMoveAssigned.vertexCount() == mesh.vertexCount());
}

inline void testMeshLifetime()
{
    std::println("--- Testing Element Validity & Mesh Copy/Move/Swap ---");
    MyMesh mesh = createSampleQuadMesh();

    testElementValidity(mesh);
    testMeshCopy(mesh);
    testMeshMove(createSampleQuadMesh());
    testMeshSwap(mesh);
    testMeshAssignment(mesh);

    std::println("Mesh lifetime and value semantics verified successfully!\n");
}

#endif // TEST_LIFETIME_H
