// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CONTAINER_PROXY_H
#define TEST_CONTAINER_PROXY_H

#include "types.h"

inline void testProxyIndexing(MyMesh& mesh)
{
    // Direct indexing on ElementContainerProxy
    assert(mesh.vertices().size() == 4);
    assert(mesh.vertices()[0].position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(mesh.faces().size() == 2);
    assert(mesh.faces()[0].vertexIndex(0) == 0);
}

inline void testPositionsComponentView(MyMesh& mesh)
{
    auto posView = mesh.vertices().positions();
    assert(posView.size() == 4);
    assert(posView.isEnabled());
    assert(posView[0] == Point3f(0.0f, 0.0f, 1.0f));
    assert(posView.data() != nullptr);
    assert(posView.data()[0] == Point3f(0.0f, 0.0f, 1.0f));

    // Test mutation via positions operator[]
    posView[0] = Point3f(0.1f, 0.2f, 1.3f);
    assert(mesh.vertex(0).position() == Point3f(0.1f, 0.2f, 1.3f));
    posView[0] = Point3f(0.0f, 0.0f, 1.0f); // restore

    // Iterate directly over positions
    std::size_t posCount = 0;
    for (Point3f& p : mesh.vertices().positions()) {
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(p)>>);
        ++posCount;
    }
    assert(posCount == 4);
}

inline void testNormalsAndColorsComponentViews(MyMesh& mesh)
{
    auto normView = mesh.vertices().normals();
    assert(normView.size() == 4);
    assert(normView[0] == Point3f(0.0f, 0.0f, 1.0f));

    auto colView = mesh.vertices().colors();
    assert(colView.size() == 4);
    assert(colView[0] == Color::Blue);
    assert(colView[1] == Color::Green);

    // Face colors
    auto faceColView = mesh.faces().colors();
    assert(faceColView.size() == 2);
    assert(faceColView[0] == Color::DarkRed);
}

inline void testFaceIndicesComponentView(MyMesh& mesh)
{
    auto faceIndicesView = mesh.faces().vertexIndices();
    assert(faceIndicesView.size() == 2);
    assert(faceIndicesView[0][0] == 0 && faceIndicesView[0][1] == 1 && faceIndicesView[0][2] == 2);
    assert(faceIndicesView.data() != nullptr);
}

inline void testProxyConcepts()
{
    MyMesh mesh;
    static_assert(!CanCallBitFlags<decltype(mesh.vertices())>);
    static_assert(!CanCallPositions<decltype(mesh.faces())>);
    static_assert(!CanCallNormals<decltype(SimpleMesh{}.vertices())>);
    static_assert(CanCallPositions<decltype(mesh.vertices())>);
    static_assert(CanCallNormals<decltype(mesh.vertices())>);
    static_assert(CanCallVertexIndices<decltype(mesh.faces())>);
    static_assert(!CanCallVertexIndices<decltype(PolyMesh{}.faces())>);
}

inline void testContainerProxy()
{
    std::println("--- Testing ElementContainerProxy & ComponentView ---");
    MyMesh mesh = createSampleQuadMesh();

    testProxyIndexing(mesh);
    testPositionsComponentView(mesh);
    testNormalsAndColorsComponentViews(mesh);
    testFaceIndicesComponentView(mesh);
    testProxyConcepts();

    std::println("ElementContainerProxy and ComponentView verified successfully!\n");
}

#endif // TEST_CONTAINER_PROXY_H
