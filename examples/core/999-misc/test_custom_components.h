// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CUSTOM_COMPONENTS_H
#define TEST_CUSTOM_COMPONENTS_H

#include "types.h"

#include <cassert>

inline void testMeshCustomComponents()
{
    // Concepts check
    static_assert(meshv2::mesh::HasCustomComponents<CCMesh>);
    static_assert(!meshv2::mesh::HasCustomComponents<MyMesh>);

    CCMesh m;

    // Initially no components
    assert(!m.hasCustomComponent("int_val"));
    assert(m.customComponentNames().empty());

    // Add custom components
    m.addCustomComponent<int>("int_val", 42);
    m.addCustomComponent<std::string>("str_val", std::string("vclib"));
    m.addCustomComponent<double>("scale", 3.14159);

    assert(m.hasCustomComponent("int_val"));
    assert(m.hasCustomComponent("str_val"));
    assert(m.hasCustomComponent("scale"));
    assert(!m.hasCustomComponent("non_existent"));

    // Type checking
    assert(m.isCustomComponentOfType<int>("int_val"));
    assert(!m.isCustomComponentOfType<float>("int_val"));
    assert(m.isCustomComponentOfType<std::string>("str_val"));
    assert(m.customComponentType("int_val") == typeid(int));
    assert(m.customComponentType("str_val") == typeid(std::string));

    // Names
    auto names = m.customComponentNames();
    assert(names.size() == 3);
    auto intNames = m.customComponentNamesOfType<int>();
    assert(intNames.size() == 1 && intNames[0] == "int_val");

    // Read and modify
    assert(m.customComponent<int>("int_val") == 42);
    m.customComponent<int>("int_val") = 100;
    assert(m.customComponent<int>("int_val") == 100);

    // Const access
    const CCMesh& cm = m;
    assert(cm.customComponent<int>("int_val") == 100);
    assert(cm.customComponent<std::string>("str_val") == "vclib");

    // Exceptions
    bool caughtNotFound = false;
    try {
        (void) m.customComponent<int>("missing");
    }
    catch (const std::out_of_range&) {
        caughtNotFound = true;
    }
    assert(caughtNotFound);

    bool caughtWrongType = false;
    try {
        (void) m.customComponent<float>("int_val");
    }
    catch (const vcl::meshv2::BadCustomComponentTypeException&) {
        caughtWrongType = true;
    }
    assert(caughtWrongType);

    // Deletion
    m.deleteCustomComponent("scale");
    assert(!m.hasCustomComponent("scale"));
    assert(m.customComponentNames().size() == 2);

    // Clear
    m.clearCustomComponents();
    assert(!m.hasCustomComponent("int_val"));
    assert(m.customComponentNames().empty());
}

inline void testElementCustomComponents()
{
    // Concepts check
    static_assert(meshv2::HasVertexCustomComponents<CCMesh>);
    static_assert(meshv2::HasFaceCustomComponents<CCMesh>);
    static_assert(!meshv2::HasVertexCustomComponents<MyMesh>);
    static_assert(!meshv2::HasFaceCustomComponents<MyMesh>);

    static_assert(meshv2::vert::HasCustomComponents<CCMesh::Vertex>);
    static_assert(meshv2::face::HasCustomComponents<CCMesh::Face>);

    CCMesh m;
    m.resizeVertices(4);
    m.resizeFaces(2);

    m.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    m.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    m.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    m.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    m.face(0).setVertices(0, 1, 2);
    m.face(1).setVertices(1, 3, 2);

    // Add vertex and face custom components
    m.addVertexCustomComponent<double>("curvature");
    m.addFaceCustomComponent<int>("mat_id");

    // Container / Mesh level queries
    assert(m.hasVertexCustomComponent("curvature"));
    assert(!m.hasVertexCustomComponent("non_existent"));
    assert(m.hasFaceCustomComponent("mat_id"));
    assert(m.isVertexCustomComponentOfType<double>("curvature"));
    assert(m.vertexCustomComponentType("curvature") == typeid(double));
    assert(m.faceCustomComponentType("mat_id") == typeid(int));

    // Generic element queries
    assert(m.hasElementCustomComponent<meshv2::ElemId::VERTEX>("curvature"));
    assert(m.hasElementCustomComponent<meshv2::ElemId::FACE>("mat_id"));
    assert((m.isElementCustomComponentOfType<meshv2::ElemId::VERTEX, double>(
        "curvature")));
    assert(
        m.elementCustomComponentType<meshv2::ElemId::VERTEX>("curvature") ==
        typeid(double));

    // Proxy access: write and read
    for (uint i = 0; i < 4; ++i) {
        assert(m.vertex(i).hasCustomComponent("curvature"));
        assert(m.vertex(i).isCustomComponentOfType<double>("curvature"));
        m.vertex(i).customComponent<double>("curvature") = 1.0 + i * 0.5;
    }

    m.face(0).customComponent<int>("mat_id") = 10;
    m.face(1).customComponent<int>("mat_id") = 20;

    for (uint i = 0; i < 4; ++i) {
        assert(m.vertex(i).customComponent<double>("curvature") == 1.0 + i * 0.5);
    }
    assert(m.face(0).customComponent<int>("mat_id") == 10);
    assert(m.face(1).customComponent<int>("mat_id") == 20);

    // Const proxy access
    const CCMesh& cm = m;
    assert(cm.vertex(2).customComponent<double>("curvature") == 2.0);
    assert(cm.face(1).customComponent<int>("mat_id") == 20);
}

inline void testCustomComponentHandles()
{
    CCMesh m;
    m.resizeVertices(3);

    m.addVertexCustomComponent<int>("weight");
    auto h = m.vertexCustomComponentVectorHandle<int>("weight");

    assert(h.size() == 3);
    assert(!h.empty());

    // Write via handle operator[]
    for (uint i = 0; i < 3; ++i) {
        h[i] = static_cast<int>(10 * (i + 1));
    }

    // Verify reflection on proxies
    assert(m.vertex(0).customComponent<int>("weight") == 10);
    assert(m.vertex(1).customComponent<int>("weight") == 20);
    assert(m.vertex(2).customComponent<int>("weight") == 30);

    // Range-based for loop over handle
    int sum = 0;
    for (int val : h) {
        sum += val;
    }
    assert(sum == 60);

    // front, back, at
    assert(h.front() == 10);
    assert(h.back() == 30);
    assert(h.at(1) == 20);

    // Const handle
    const CCMesh& cm = m;
    auto          ch = cm.vertexCustomComponentVectorHandle<int>("weight");
    assert(ch.size() == 3);
    assert(ch[1] == 20);

    // Converting constructor from handle to const handle
    meshv2::ConstCustomComponentVectorHandle<int> ch2 = h;
    assert(ch2[2] == 30);
}

inline void testCustomComponentsLifecycle()
{
    // 1. Lazy initialization on resize
    {
        CCMesh m;
        m.resizeVertices(2);
        m.addVertexCustomComponent<float>("scalar");

        m.vertex(0).customComponent<float>("scalar") = 4.5f;
        m.vertex(1).customComponent<float>("scalar") = 5.5f;

        // Resize larger: new elements are uninitialized until accessed
        m.addVertices(2); // total 4
        assert(m.vertex(0).customComponent<float>("scalar") == 4.5f);
        assert(m.vertex(1).customComponent<float>("scalar") == 5.5f);
        assert(m.vertex(2).customComponent<float>("scalar") == 0.0f);
        assert(m.vertex(3).customComponent<float>("scalar") == 0.0f);
    }

    // 2. Compaction
    {
        CCMesh m;
        m.resizeVertices(4);
        m.addVertexCustomComponent<int>("id");
        for (uint i = 0; i < 4; ++i) {
            m.vertex(i).customComponent<int>("id") = static_cast<int>(i * 10);
        }

        m.deleteVertex(1); // delete element at index 1
        assert(m.vertexCount() == 3);
        assert(m.vertexContainerSize() == 4);

        m.compactVertices();
        assert(m.vertexCount() == 3);
        assert(m.vertexContainerSize() == 3);

        // Remaining elements should be 0, 2, 3 -> compacted to 0, 1, 2
        assert(m.vertex(0).customComponent<int>("id") == 0);
        assert(m.vertex(1).customComponent<int>("id") == 20);
        assert(m.vertex(2).customComponent<int>("id") == 30);
    }

    // 3. Append
    {
        CCMesh m1;
        m1.resizeVertices(2);
        m1.addVertexCustomComponent<int>("tag");
        m1.vertex(0).customComponent<int>("tag") = 1;
        m1.vertex(1).customComponent<int>("tag") = 2;

        CCMesh m2;
        m2.resizeVertices(2);
        m2.addVertexCustomComponent<int>("tag");
        m2.vertex(0).customComponent<int>("tag") = 10;
        m2.vertex(1).customComponent<int>("tag") = 20;

        m1.append(m2);

        assert(m1.vertexCount() == 4);
        assert(m1.vertex(0).customComponent<int>("tag") == 1);
        assert(m1.vertex(1).customComponent<int>("tag") == 2);
        assert(m1.vertex(2).customComponent<int>("tag") == 10);
        assert(m1.vertex(3).customComponent<int>("tag") == 20);
    }

    // 4. Mesh importFrom
    {
        CCMesh src;
        src.resizeVertices(2);
        src.addCustomComponent<std::string>("mesh_tag", "imported");
        src.addVertexCustomComponent<double>("attr");
        src.vertex(0).customComponent<double>("attr") = 9.9;
        src.vertex(1).customComponent<double>("attr") = 8.8;

        CCMesh dst;
        dst.importFrom(src);

        assert(dst.hasCustomComponent("mesh_tag"));
        assert(dst.customComponent<std::string>("mesh_tag") == "imported");
        assert(dst.vertexCount() == 2);
        assert(dst.hasVertexCustomComponent("attr"));
        assert(dst.vertex(0).customComponent<double>("attr") == 9.9);
        assert(dst.vertex(1).customComponent<double>("attr") == 8.8);
    }
}

inline void testCustomComponents()
{
    std::println("--- Testing Custom Components ---");
    testMeshCustomComponents();
    testElementCustomComponents();
    testCustomComponentHandles();
    testCustomComponentsLifecycle();
    std::println("Custom components verified successfully!\n");
}

#endif // TEST_CUSTOM_COMPONENTS_H
