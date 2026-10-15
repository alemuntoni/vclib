// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>
#include <vclib/mesh_v2/components/base/base.h>
#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/normal.h>
#include <vclib/mesh_v2/components/position.h>
#include <vclib/mesh_v2/components/vertex_references.h>
#include <vclib/mesh_v2/containers/base/element_container.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>
#include <vclib/space/core.h>

using namespace vcl;

int main()
{
    // TypeWrapper of components: BitFlags is MANDATORY!
    using MyComponents = TypeWrapper<
        meshv2::comp::BitFlags,
        meshv2::comp::Position3f,
        meshv2::comp::Normal3f>;

    // A generic element container
    using MyContainer = meshv2::ElementContainer<MyComponents>;

    MyContainer container;

    // enable components trough COMP_ID
    container.template enable<meshv2::CompId::POSITION>();
    container.template enable<meshv2::CompId::NORMAL>();

    container.resize(10);

    // ElementProxy to access the data of the container
    using MyElementProxy = meshv2::ElementProxy<MyContainer, MyComponents>;
    MyElementProxy element0(container, 0);

    // Use the proxy to access and modify the data of the container
    if (element0) {
        element0.position() = Point3f(1.0f, 2.0f, 3.0f);
        element0.normal()   = Point3f(0.0f, 1.0f, 0.0f);

        std::cout << "Position of element 0 read from proxy: "
                  << element0.position().x() << ", " << element0.position().y()
                  << ", " << element0.position().z() << "\n";
    }

    // ConstElementProxy to access the data of the container in a const way
    using MyConstProxy = meshv2::ElementProxy<const MyContainer, MyComponents>;
    MyConstProxy constElement0 = element0;

    const MyConstProxy& constElement01 = element0;

    // constElement0.position() = Point3f(0.0f, 0.0f, 0.0f); // build error
    // constElement01.position() = Point3f(0.0f, 0.0f, 0.0f); // build error

    std::cout << "Position 0 (from ConstProxy): "
              << constElement0.position().x() << ", "
              << constElement0.position().y() << ", "
              << constElement0.position().z() << "\n";

    std::cout << "Position 0 (from const Proxy&): "
              << constElement01.position().x() << ", "
              << constElement01.position().y() << ", "
              << constElement01.position().z() << "\n";

    std::cout << "Normal 0:   " << element0.normal().x() << ", "
              << element0.normal().y() << ", " 
              << element0.normal().z() << "\n";
              
    std::cout << "element0 is valid: " << (element0 ? "yes" : "no") << "\n";
    std::cout << "Container size: " << container.size() << "\n";

    // an invalid proxy (index out of bounds)
    MyElementProxy invalidElement;
    std::cout << "invalidElement is valid: " << (invalidElement ? "yes" : "no")
              << "\n";

    // Test IndexType deduction: default is uint
    static_assert(std::is_same_v<MyElementProxy::IndexType, uint>);

    // Test C++20 Concepts
    static_assert(meshv2::comp::HasBitFlags<MyComponents>);
    static_assert(meshv2::comp::HasBitFlags<MyContainer>);
    static_assert(meshv2::comp::HasBitFlags<MyElementProxy>);
    static_assert(meshv2::comp::HasPosition<MyComponents>);
    static_assert(meshv2::comp::HasPosition<MyContainer>);
    static_assert(meshv2::comp::HasPosition<MyElementProxy>);
    static_assert(meshv2::comp::HasNormal<MyComponents>);
    static_assert(meshv2::comp::HasNormal<MyContainer>);
    static_assert(meshv2::comp::HasNormal<MyElementProxy>);

    assert(meshv2::comp::isPositionAvailableOn(element0));
    assert(meshv2::comp::isNormalAvailableOn(element0));

    // Test customized 64-bit container
    using MyBigContainer = meshv2::ElementContainer<MyComponents, uint64_t>;
    using MyBigElementProxy = meshv2::ElementProxy<MyBigContainer, MyComponents>;
    static_assert(std::is_same_v<MyBigElementProxy::IndexType, uint64_t>);

    MyBigContainer bigContainer;
    bigContainer.template enable<meshv2::CompId::POSITION>();
    bigContainer.resize(1);
    MyBigElementProxy bigElement(bigContainer, 0);
    bigElement.position() = Point3f(4.0f, 5.0f, 6.0f);
    std::cout << "BigContainer (uint64_t index) Element 0 position: "
              << bigElement.position().x() << ", "
              << bigElement.position().y() << ", "
              << bigElement.position().z() << "\n";

    // ==========================================
    // Phase 4: BitFlags and Iterators with skip
    // ==========================================
    std::cout << "\n--- Testing Phase 4: BitFlags & Iterators ---\n";
    MyContainer testContainer;
    testContainer.template enable<meshv2::CompId::POSITION>();
    testContainer.resize(5);

    for (std::size_t i = 0; i < testContainer.size(); ++i) {
        testContainer[i].position() = Point3f(static_cast<float>(i), 0.0f, 0.0f);
    }

    // Delete elements 1 and 3
    testContainer[1].setDeleted(true);
    testContainer.deleteElement(3);

    std::cout << "Physical size: " << testContainer.size() << "\n";
    std::cout << "Non-deleted count: " << testContainer.elementCount() << "\n";
    std::cout << "Deleted count: " << testContainer.deletedElementCount() << "\n";

    // Range-based for loop: automatically skips deleted elements (1 and 3)
    std::cout << "Iterating over non-deleted elements (range-based for loop):\n";
    for (auto elem : testContainer) {
        std::cout << "  Visible element " << elem.index()
                  << " pos.x = " << elem.position().x()
                  << " deleted: " << (elem.deleted() ? "yes" : "no") << "\n";
    }

    // Iterator over ALL elements including deleted
    std::cout << "Iterating over ALL elements (jumpDeleted = false):\n";
    for (auto it = testContainer.begin(false); it != testContainer.end(); ++it) {
        std::cout << "  Element " << it->index()
                  << " pos.x = " << it->position().x()
                  << " deleted: " << (it->deleted() ? "yes" : "no") << "\n";
    }

    // Undelete element 1
    testContainer[1].setDeleted(false);
    std::cout << "After undeleting element 1, non-deleted count: "
              << testContainer.elementCount() << "\n";

    // ==========================================
    // Phase 5: TriangleVertexReferences & Mesh navigation
    // ==========================================
    std::cout << "\n--- Testing Phase 5: TriangleVertexReferences ---\n";

    using VertexComponents = TypeWrapper<
        meshv2::comp::BitFlags,
        meshv2::comp::Position3f,
        meshv2::comp::Normal3f>;

    using FaceComponents = TypeWrapper<
        meshv2::comp::BitFlags,
        meshv2::comp::TriangleVertexReferences<uint>>;

    static_assert(meshv2::comp::HasVertexReferences<FaceComponents>);

    struct ProtoMesh
    {
        using VertexContainer =
            meshv2::ElementContainer<VertexComponents, uint, ProtoMesh>;
        using FaceContainer =
            meshv2::ElementContainer<FaceComponents, uint, ProtoMesh>;

        VertexContainer verticesContainer;
        FaceContainer   facesContainer;

        ProtoMesh()
        {
            verticesContainer.setMesh(this);
            facesContainer.setMesh(this);
            verticesContainer.template enable<meshv2::CompId::POSITION>();
            facesContainer.template enable<meshv2::CompId::VERTEX_REFERENCES>();
        }

        auto vertex(uint i) { return verticesContainer[i]; }
        auto vertex(uint i) const { return verticesContainer[i]; }
        auto face(uint i) { return facesContainer[i]; }
        auto face(uint i) const { return facesContainer[i]; }
    };

    ProtoMesh protoMesh;
    protoMesh.verticesContainer.resize(3);
    protoMesh.facesContainer.resize(1);

    protoMesh.vertex(0).position() = Point3f(1.0f, 0.0f, 0.0f);
    protoMesh.vertex(1).position() = Point3f(0.0f, 1.0f, 0.0f);
    protoMesh.vertex(2).position() = Point3f(0.0f, 0.0f, 1.0f);

    auto f0 = protoMesh.face(0);
    f0.setVertexIndices(0, 1, 2);

    std::cout << "Face 0 vertex indices: "
              << f0.vertexIndex(0) << ", "
              << f0.vertexIndex(1) << ", "
              << f0.vertexIndex(2) << "\n";

    std::cout << "Face 0 vertex(0) position: "
              << f0.vertex(0).position().x() << ", "
              << f0.vertex(0).position().y() << ", "
              << f0.vertex(0).position().z() << "\n";

    std::cout << "Face 0 vertex(1) position: "
              << f0.vertex(1).position().x() << ", "
              << f0.vertex(1).position().y() << ", "
              << f0.vertex(1).position().z() << "\n";

    std::cout << "Iterating over face 0 vertices with face.vertices():\n";
    for (auto v : f0.vertices()) {
        std::cout << "  Vertex " << v.index()
                  << " pos = (" << v.position().x()
                  << ", " << v.position().y()
                  << ", " << v.position().z() << ")\n";
    }

    std::cout << "f0.vertices()[2] position: "
              << f0.vertices()[2].position().x() << ", "
              << f0.vertices()[2].position().y() << ", "
              << f0.vertices()[2].position().z() << "\n";

    // Test const correctness on face.vertices()
    const auto& constProtoMesh = protoMesh;
    auto constF0 = constProtoMesh.face(0);
    std::cout << "Const face 0 vertex(0) position: "
              << constF0.vertex(0).position().x() << "\n";
    for (auto v : constF0.vertices()) {
        std::cout << "  Const Vertex " << v.index()
                  << " pos.x = " << v.position().x() << "\n";
    }

    return 0;
}
