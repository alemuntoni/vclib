// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CONST_CORRECTNESS_H
#define TEST_CONST_CORRECTNESS_H

#include "types.h"

inline void testConstMeshColors(const MyMesh& constMesh)
{
    // Mesh color const correctness
    assert(constMesh.color() == Color::Red);
    static_assert(std::is_same_v<decltype(constMesh.color()), const vcl::Color&>);

    // Vertex & Face color const correctness
    assert(constMesh.vertex(0).color() == Color::Blue);
    static_assert(std::is_same_v<decltype(constMesh.vertex(0).color()), const vcl::Color&>);
    assert(constMesh.face(0).color() == Color::DarkRed);
    static_assert(std::is_same_v<decltype(constMesh.face(0).color()), const vcl::Color&>);
}

inline void testConstComponentViews(const MyMesh& constMesh)
{
    auto constPosView = constMesh.vertices().positions();
    assert(constPosView.size() == 4);
    assert(constPosView[0] == Point3f(0.0f, 0.0f, 1.0f));
    static_assert(std::is_same_v<decltype(constPosView[0]), const Point3f&>);
    static_assert(std::is_same_v<decltype(constPosView.data()), const Point3f*>);

    for (const Point3f& p : constMesh.vertices().positions()) {
        static_assert(std::is_const_v<std::remove_reference_t<decltype(p)>>);
        (void)p;
    }

    for (const auto& vi : constMesh.face(0).vertexIndices()) {
        static_assert(std::is_const_v<std::remove_reference_t<decltype(vi)>>);
        (void)vi;
    }
}

inline void testConstElementProxies(MyMesh& mesh, const MyMesh& constMesh)
{
    // Access returns ConstVertex / ConstFace proxies
    MyMesh::ConstVertex cv0 = constMesh.vertex(0);
    assert(cv0.position() == Point3f(0.0f, 0.0f, 1.0f));

    for (MyMesh::ConstVertex v : constMesh.face(0).vertices()) {
        assert(v.isValid());
    }

    // Conversion from mutable Vertex to ConstVertex
    MyMesh::ConstVertex cvFromMutable = mesh.vertex(2);
    assert(cvFromMutable.index() == 2);

    // Test vertexIndices() view (mutable vs const)
    auto f0 = mesh.face(0);
    f0.vertexIndices()[1] = 1; // Mutate via view operator[]
    for (auto& vi : f0.vertexIndices()) {
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(vi)>>);
        vi = vi; // Mutate via reference in range-for
    }
}

inline void testConstCorrectness()
{
    std::println("--- Testing Const Correctness ---");
    MyMesh mesh = createSampleQuadMesh();
    const MyMesh& constMesh = mesh;

    testConstMeshColors(constMesh);
    testConstComponentViews(constMesh);
    testConstElementProxies(mesh, constMesh);

    std::println("Const correctness verified successfully!\n");
}

#endif // TEST_CONST_CORRECTNESS_H
