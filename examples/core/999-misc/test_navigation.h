// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_NAVIGATION_H
#define TEST_NAVIGATION_H

#include "types.h"

inline void testMeshCreationAndColors(const MyMesh& mesh)
{
    assert(mesh.vertexCount() == 4);
    assert(mesh.faceCount() == 2);

    // Mesh color
    assert(mesh.color() == Color::Red);
    assert(meshv2::mesh::isColorAvailableOn(mesh));
    assert(meshv2::comp::isColorAvailableOn(mesh));

    // Vertex colors
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert(meshv2::comp::isColorAvailableOn(mesh.vertex(0)));
    assert(mesh.vertex(0).color() == Color::Blue);
    assert(mesh.vertex(1).color() == Color::Green);
    assert(mesh.vertex(2).color() == Color::Yellow);
    assert(mesh.vertex(3).color() == Color::Cyan);

    // Face colors
    assert((mesh.isPerElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
    assert(meshv2::comp::isColorAvailableOn(mesh.face(0)));
    assert(mesh.face(0).color() == Color::DarkRed);
    assert(mesh.face(1).color() == Color::DarkBlue);

    std::println("  Vertices: {}, Faces: {}", mesh.vertexCount(), mesh.faceCount());
    std::println("  Mesh color: {}", mesh.color());
    std::println("  Vertex 0 color: {}", mesh.vertex(0).color());
    std::println("  Face 0 color: {}", mesh.face(0).color());
}

inline void testFaceToVertexNavigation(MyMesh& mesh)
{
    std::println("  Topological Navigation from Face 0:");
    assert(mesh.face(0).vertexCount() == 3);
    for (vcl::uint i = 0; i < mesh.face(0).vertexCount(); ++i) {
        std::println("    Corner {}: vertex index = {}, position = {}",
                     i, mesh.face(0).vertexIndex(i), mesh.face(0).vertex(i).position());
    }

    std::println("  Iterating over Face 1 vertices with face.vertices():");
    vcl::uint cornerIdx = 0;
    for (MyMesh::Vertex v : mesh.face(1).vertices()) {
        std::println("    Vertex {} pos = {} norm = {}", v.index(), v.position(), v.normal());
        ++cornerIdx;
    }
    assert(cornerIdx == 3);
}

inline void testMeshElementIteration(MyMesh& mesh)
{
    std::println("  Iterating over all Vertices:");
    std::size_t vCount = 0;
    for (auto v : mesh.vertices()) {
        std::println("    Vertex {} pos = {} norm = {}", v.index(), v.position(), v.normal());
        ++vCount;
    }
    assert(vCount == 4);

    std::println("  Iterating over all Faces:");
    std::size_t fCount = 0;
    for (auto f : mesh.faces()) {
        std::println("    Face {} with vertex indices: ({}, {}, {})",
                     f.index(), f.vertexIndex(0), f.vertexIndex(1), f.vertexIndex(2));
        ++fCount;
    }
    assert(fCount == 2);
}

inline void testNavigation()
{
    std::println("--- Testing Navigation & Iteration ---");
    MyMesh mesh = createSampleQuadMesh();

    testMeshCreationAndColors(mesh);
    testFaceToVertexNavigation(mesh);
    testMeshElementIteration(mesh);

    std::println("Topological navigation and iteration verified successfully!\n");
}

#endif // TEST_NAVIGATION_H
