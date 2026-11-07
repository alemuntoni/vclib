// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

inline void testElementValidity(MyMesh& mesh)
{
    MyMesh::Vertex validV = mesh.vertex(0);
    MyMesh::Vertex invalidV;

    REQUIRE((bool) validV);
    REQUIRE(validV.isValid());
    REQUIRE(!(bool) invalidV);
    REQUIRE(!invalidV.isValid());

    MyMesh::Face validF = mesh.face(0);
    MyMesh::Face invalidF;
    REQUIRE((bool) validF);
    REQUIRE(validF.isValid());
    REQUIRE(!(bool) invalidF);
    REQUIRE(!invalidF.isValid());
}

inline void testMeshCopy(const MyMesh& mesh)
{
    MyMesh meshCopy = mesh;
    REQUIRE(meshCopy.color() == Color::Red);
    REQUIRE(meshCopy.vertexCount() == mesh.vertexCount());
    REQUIRE(meshCopy.faceCount() == mesh.faceCount());
    REQUIRE(
        meshCopy.face(0).vertex(0).position() ==
        mesh.face(0).vertex(0).position());

    // Deep copy check: modifying copy must not affect original
    meshCopy.vertex(0).position() = Point3f(99.0f, 99.0f, 99.0f);
    REQUIRE(mesh.vertex(0).position() != Point3f(99.0f, 99.0f, 99.0f));
}

inline void testMeshMove(MyMesh meshToMove)
{
    std::size_t origVCount = meshToMove.vertexCount();
    MyMesh      meshMoved  = std::move(meshToMove);
    REQUIRE(meshMoved.color() == Color::Red);
    REQUIRE(meshMoved.vertexCount() == origVCount);
    REQUIRE(
        meshMoved.face(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
}

inline void testMeshSwap(MyMesh& mesh)
{
    std::size_t origVCount = mesh.vertexCount();
    MyMesh      meshSwapped;
    swap(mesh, meshSwapped);
    REQUIRE(meshSwapped.color() == Color::Red);
    REQUIRE(meshSwapped.vertexCount() == origVCount);
    REQUIRE(mesh.vertexCount() == 0);

    // Swap back
    swap(mesh, meshSwapped);
    REQUIRE(mesh.vertexCount() == origVCount);
}

inline void testMeshAssignment(const MyMesh& mesh)
{
    MyMesh meshAssigned;
    meshAssigned = mesh; // Copy assignment
    REQUIRE(meshAssigned.color() == Color::Red);
    REQUIRE(meshAssigned.vertexCount() == mesh.vertexCount());

    MyMesh meshMoveAssigned;
    meshMoveAssigned = std::move(meshAssigned); // Move assignment
    REQUIRE(meshMoveAssigned.color() == Color::Red);
    REQUIRE(meshMoveAssigned.vertexCount() == mesh.vertexCount());
}

TEST_CASE("Lifetime")
{
    MyMesh mesh = createSampleQuadMesh();

    testElementValidity(mesh);
    testMeshCopy(mesh);
    testMeshMove(createSampleQuadMesh());
    testMeshSwap(mesh);
    testMeshAssignment(mesh);
}
