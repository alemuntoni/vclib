// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_SERIALIZATION_H
#define TEST_SERIALIZATION_H

#include "types.h"

#include <vclib/space/core/parallel_vectors_tuple.h>

#include <cassert>
#include <print>
#include <sstream>
#include <string>
#include <vector>

inline void testParallelVectorsTupleSerialization()
{
    std::println("Testing ParallelVectorsTuple serialization...");

    using PVT = vcl::ParallelVectorsTuple<int, vcl::Point3d, std::string, std::vector<uint>>;
    static_assert(vcl::Serializable<PVT>);

    // 1. Empty tuple round trip
    {
        PVT pvt;
        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        vcl::serialize(ss, pvt);

        PVT pvtRead;
        pvtRead.enable<int>();
        pvtRead.resize(5); // dirty state before deserialize
        vcl::deserialize(ss, pvtRead);

        assert(pvtRead.size() == 0);
        assert(!pvtRead.isEnabled<int>());
        assert(!pvtRead.isEnabled<vcl::Point3d>());
        assert(!pvtRead.isEnabled<std::string>());
        assert(!pvtRead.isEnabled<std::vector<uint>>());
    }

    // 2. Populated tuple with mixed enabled/disabled and trivial/non-trivial types
    {
        PVT pvt;
        pvt.enable<int>();
        pvt.enable<vcl::Point3d>();
        pvt.enable<std::string>();
        // std::vector<uint> remains disabled!

        pvt.resize(3);
        pvt.get<int>()[0] = 42;
        pvt.get<int>()[1] = 100;
        pvt.get<int>()[2] = -7;

        pvt.get<vcl::Point3d>()[0] = vcl::Point3d(1.0, 2.0, 3.0);
        pvt.get<vcl::Point3d>()[1] = vcl::Point3d(4.0, 5.0, 6.0);
        pvt.get<vcl::Point3d>()[2] = vcl::Point3d(7.0, 8.0, 9.0);

        pvt.get<std::string>()[0] = "alpha";
        pvt.get<std::string>()[1] = "beta";
        pvt.get<std::string>()[2] = "gamma_with_longer_string_beyond_sso";

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        vcl::serialize(ss, pvt);

        PVT pvtRead;
        // set some opposite enables to check clean override
        pvtRead.enable<std::vector<uint>>();
        pvtRead.resize(10);
        vcl::deserialize(ss, pvtRead);

        assert(pvtRead.size() == 3);
        assert(pvtRead.isEnabled<int>());
        assert(pvtRead.isEnabled<vcl::Point3d>());
        assert(pvtRead.isEnabled<std::string>());
        assert(!pvtRead.isEnabled<std::vector<uint>>());

        assert(pvtRead.get<int>()[0] == 42);
        assert(pvtRead.get<int>()[1] == 100);
        assert(pvtRead.get<int>()[2] == -7);

        assert(pvtRead.get<vcl::Point3d>()[0] == vcl::Point3d(1.0, 2.0, 3.0));
        assert(pvtRead.get<vcl::Point3d>()[1] == vcl::Point3d(4.0, 5.0, 6.0));
        assert(pvtRead.get<vcl::Point3d>()[2] == vcl::Point3d(7.0, 8.0, 9.0));

        assert(pvtRead.get<std::string>()[0] == "alpha");
        assert(pvtRead.get<std::string>()[1] == "beta");
        assert(pvtRead.get<std::string>()[2] == "gamma_with_longer_string_beyond_sso");
    }

    std::println("ParallelVectorsTuple serialization tests passed!");
}

inline void testMeshSerialization()
{
    std::println("Testing Mesh serialization...");

    // 1. Compile-time concepts: Mesh is Serializable, while child containers are protected (not Serializable from outside)
    static_assert(vcl::Serializable<MyMesh>);
    static_assert(vcl::Serializable<CCMesh>);
    static_assert(!vcl::Serializable<MyMesh::VertexContainer>);
    static_assert(!vcl::Serializable<MyMesh::FaceContainer>);
    static_assert(!vcl::Serializable<CCMesh::VertexContainer>);
    static_assert(!vcl::Serializable<CCMesh::FaceContainer>);

    // 2. Full Mesh Round-Trip (with mesh color, optional components, deleted elements, face vertex references)
    {
        MyMesh m = createSampleQuadMesh();
        m.color() = vcl::Color::Cyan;
        m.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>();
        m.vertex(0).color() = vcl::Color::Red;
        m.vertex(1).color() = vcl::Color::Green;
        m.vertex(2).color() = vcl::Color::Blue;
        m.vertex(3).color() = vcl::Color::White;

        // Delete vertex 1 (a hole in the SoA buffer)
        m.deleteVertex(1);
        assert(m.vertexCount() == 3);
        assert(m.vertexContainerSize() == 4);
        assert(m.faceCount() == 2);

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        m.serialize(ss);

        MyMesh mRead;
        mRead.deserialize(ss);

        // Verify Mesh-level properties
        assert(mRead.color() == vcl::Color::Cyan);

        // Verify Vertices
        assert(mRead.vertexCount() == 3);
        assert(mRead.vertexContainerSize() == 4);
        assert(mRead.vertex(0).position() == m.vertex(0).position());
        assert(mRead.vertex(2).position() == m.vertex(2).position());
        assert(mRead.vertex(3).position() == m.vertex(3).position());

        // Verify Optional Components
        assert((mRead.isPerElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
        assert(mRead.vertex(0).color() == vcl::Color::Red);
        assert(mRead.vertex(2).color() == vcl::Color::Blue);
        assert(mRead.vertex(3).color() == vcl::Color::White);

        // Verify Deleted flags
        assert(mRead.vertex(1).deleted());
        assert(!mRead.vertex(0).deleted());
        assert(!mRead.vertex(2).deleted());
        assert(!mRead.vertex(3).deleted());

        // Verify Faces & Topological Rebinding
        assert(mRead.faceCount() == 2);
        assert(mRead.face(0).vertexIndex(0) == m.face(0).vertexIndex(0));
        assert(mRead.face(0).vertexIndex(1) == m.face(0).vertexIndex(1));
        assert(mRead.face(0).vertexIndex(2) == m.face(0).vertexIndex(2));

        // Rebound parent pointers check: navigation face -> vertex works!
        assert(mRead.face(0).vertex(0).position() == mRead.vertex(0).position());
        assert(mRead.face(0).vertex(2).position() == mRead.vertex(2).position());
    }

    // 3. Header & Version Fail-Fast Validation
    {
        MyMesh m = createSampleQuadMesh();
        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        m.serialize(ss);

        std::string serializedData = ss.str();

        // 3a. Corrupt Magic Header
        {
            std::string badMagicData = serializedData;
            badMagicData[0] = 'X';
            std::stringstream ssBad(badMagicData, std::ios::in | std::ios::out | std::ios::binary);
            bool caughtBadMagic = false;
            try {
                MyMesh mRead;
                mRead.deserialize(ssBad);
            }
            catch (const std::runtime_error& e) {
                caughtBadMagic = true;
            }
            assert(caughtBadMagic);
        }

        // 3b. Unsupported Version
        {
            std::string badVersionData = serializedData;
            // Version is uint32_t starting at offset 8
            uint32_t badVer = 999;
            std::memcpy(&badVersionData[8], &badVer, sizeof(badVer));
            std::stringstream ssBad(badVersionData, std::ios::in | std::ios::out | std::ios::binary);
            bool caughtBadVersion = false;
            try {
                MyMesh mRead;
                mRead.deserialize(ssBad);
            }
            catch (const std::runtime_error& e) {
                caughtBadVersion = true;
            }
            assert(caughtBadVersion);
        }
    }

    // 4. Custom Components Serialization on Mesh
    {
        CCMesh cm;
        cm.resizeVertices(3);
        cm.resizeFaces(1);

        cm.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
        cm.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
        cm.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
        cm.face(0).setVertices(0, 1, 2);

        // Add custom components: mesh-level, per-vertex, per-face
        cm.addCustomComponent<int>("dataset_id", 42);
        cm.addPerVertexCustomComponent<double>("weight");
        cm.vertex(0).customComponent<double>("weight") = 1.25;
        cm.vertex(1).customComponent<double>("weight") = 2.50;
        cm.vertex(2).customComponent<double>("weight") = 3.75;

        cm.addPerFaceCustomComponent<int>("mat_id");
        cm.face(0).customComponent<int>("mat_id") = 777;

        // 4a. Serialize standard mesh structure
        std::stringstream ssMesh(std::ios::in | std::ios::out | std::ios::binary);
        cm.serialize(ssMesh);

        // 4b. Serialize custom components streams
        // Mesh-level custom components
        std::stringstream ssMeshCC(std::ios::in | std::ios::out | std::ios::binary);
        cm.serializeCustomComponentsOfType<int>(ssMeshCC);

        // Per-element generic methods on Mesh
        std::stringstream ssVertCC(std::ios::in | std::ios::out | std::ios::binary);
        cm.serializePerElementCustomComponentsOfType<vcl::meshv2::ElemId::VERTEX, double>(ssVertCC);

        // Per-face method inherited from FaceContainer
        std::stringstream ssFaceCC(std::ios::in | std::ios::out | std::ios::binary);
        cm.serializePerFaceCustomComponentsOfType<int>(ssFaceCC);

        // Deserialize on new mesh
        CCMesh cmRead;
        cmRead.deserialize(ssMesh);

        cmRead.deserializeCustomComponentsOfType<int>(ssMeshCC);
        // Generic deserializePerElement on Mesh
        cmRead.deserializePerElementCustomComponentsOfType<vcl::meshv2::ElemId::VERTEX, double>(ssVertCC);
        // FaceContainer deserializePerFace
        cmRead.deserializePerFaceCustomComponentsOfType<int>(ssFaceCC);

        assert(cmRead.hasCustomComponent("dataset_id"));
        assert(cmRead.customComponent<int>("dataset_id") == 42);

        assert(cmRead.hasPerVertexCustomComponent("weight"));
        assert(cmRead.vertex(0).customComponent<double>("weight") == 1.25);
        assert(cmRead.vertex(1).customComponent<double>("weight") == 2.50);
        assert(cmRead.vertex(2).customComponent<double>("weight") == 3.75);

        assert(cmRead.hasPerFaceCustomComponent("mat_id"));
        assert(cmRead.face(0).customComponent<int>("mat_id") == 777);

        // 4c. Container-level serializePerVertex / deserializePerVertex roundtrip test
        std::stringstream ssVertCC2(std::ios::in | std::ios::out | std::ios::binary);
        cm.serializePerVertexCustomComponentsOfType<double>(ssVertCC2);
        CCMesh cmRead2;
        ssMesh.clear();
        ssMesh.seekg(0);
        cmRead2.deserialize(ssMesh);
        cmRead2.deserializePerVertexCustomComponentsOfType<double>(ssVertCC2);
        assert(cmRead2.vertex(1).customComponent<double>("weight") == 2.50);

        // 4d. Mismatch fail-fast test (float vs double)
        std::stringstream ssMismatch(std::ios::in | std::ios::out | std::ios::binary);
        cm.serializePerVertexCustomComponentsOfType<double>(ssMismatch);

        bool caughtMismatch = false;
        try {
            CCMesh cmErr = cmRead;
            cmErr.deserializePerElementCustomComponentsOfType<vcl::meshv2::ElemId::VERTEX, float>(ssMismatch);
        }
        catch (const vcl::meshv2::BadCustomComponentTypeException&) {
            caughtMismatch = true;
        }
        assert(caughtMismatch);
    }

    // 5. TransformedMesh serialization test
    {
        TransformedMesh tm = createSampleTransformedMesh();
        Matrix44f mat = Matrix44f::Identity();
        mat(0, 3) = 12.34f;
        mat(1, 3) = 56.78f;
        mat(2, 2) = -1.0f;
        tm.transformMatrix() = mat;

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        tm.serialize(ss);

        TransformedMesh tmRead;
        tmRead.deserialize(ss);

        assert(tmRead.vertexCount() == tm.vertexCount());
        assert(tmRead.faceCount() == tm.faceCount());
        assert(tmRead.transformMatrix() == mat);
    }

    // 6. MaterialMeshWithMaterials serialization test
    {
        MaterialMeshWithMaterials mm = createSampleMaterialMesh();
        mm.meshBasePath() = "/path/to/mesh";
        mm.pushTextureImage("textures/wood.png", Image());

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        mm.serialize(ss);

        MaterialMeshWithMaterials mmRead;
        mmRead.deserialize(ss);

        assert(mmRead.vertexCount() == mm.vertexCount());
        assert(mmRead.faceCount() == mm.faceCount());
        assert(mmRead.materialCount() == 2);
        assert(mmRead.material(0).name() == "Material_0");
        assert(mmRead.material(1).name() == "Material_1");
        assert(mmRead.meshBasePath() == "/path/to/mesh");
        assert(mmRead.textureImageCount() == 1);
        assert(mmRead.face(0).materialIndex() == 0);
        assert(mmRead.face(1).materialIndex() == 1);
    }

    // 7. BoundingBoxMesh serialization test
    {
        BoundingBoxMesh bbm;
        bbm.boundingBox().min() = Point3f(-10.0f, -20.0f, -30.0f);
        bbm.boundingBox().max() = Point3f(10.0f, 20.0f, 30.0f);

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        bbm.serialize(ss);

        BoundingBoxMesh bbmRead;
        bbmRead.deserialize(ss);

        assert(bbmRead.boundingBox().min() == Point3f(-10.0f, -20.0f, -30.0f));
        assert(bbmRead.boundingBox().max() == Point3f(10.0f, 20.0f, 30.0f));
    }

    // 8. NamedMesh serialization test
    {
        NamedMesh nm;
        nm.name() = "SerializedMeshName";
        nm.addVertices(2);

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        nm.serialize(ss);

        NamedMesh nmRead;
        nmRead.deserialize(ss);

        assert(nmRead.name() == "SerializedMeshName");
        assert(nmRead.vertexCount() == 2);
    }

    // 9. CurvatureMesh serialization test
    {
        CurvatureMesh cm;
        cm.name() = "CurvatureMeshTest";
        cm.addVertices(2);
        cm.vertex(0).principalCurvature().maxDir() = Point3f(1.0f, 0.0f, 0.0f);
        cm.vertex(0).principalCurvature().minDir() = Point3f(0.0f, 1.0f, 0.0f);
        cm.vertex(0).principalCurvature().maxValue() = 10.0f;
        cm.vertex(0).principalCurvature().minValue() = 2.0f;

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        cm.serialize(ss);

        CurvatureMesh cmRead;
        cmRead.deserialize(ss);

        assert(cmRead.name() == "CurvatureMeshTest");
        assert(cmRead.vertexCount() == 2);
        assert(cmRead.vertex(0).principalCurvature().maxDir() == Point3f(1.0f, 0.0f, 0.0f));
        assert(cmRead.vertex(0).principalCurvature().minDir() == Point3f(0.0f, 1.0f, 0.0f));
        assert(cmRead.vertex(0).principalCurvature().maxValue() == 10.0f);
        assert(cmRead.vertex(0).principalCurvature().minValue() == 2.0f);
    }

    // 10. QualityMesh serialization test
    {
        QualityMesh qm;
        qm.addVertices(2);
        qm.vertex(0).quality() = 12.34f;
        qm.vertex(1).quality() = 56.78f;

        std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
        qm.serialize(ss);

        QualityMesh qmRead;
        qmRead.deserialize(ss);

        assert(qmRead.vertexCount() == 2);
        assert(qmRead.vertex(0).quality() == 12.34f);
        assert(qmRead.vertex(1).quality() == 56.78f);
    }

    std::println("Mesh serialization tests passed!");
}

inline void testSerialization()
{
    testParallelVectorsTupleSerialization();
    testMeshSerialization();
}

#endif // TEST_SERIALIZATION_H
