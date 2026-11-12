// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_MESH_INFO_H
#define TEST_MESH_INFO_H

#include "types.h"

#include <vclib/space/complex/mesh_info.h>

#include <cassert>
#include <print>

inline void testMeshInfoDefaultAndManual()
{
    MeshInfo info;
    assert(info.isEmpty());
    assert(info.isUnkownMesh());
    assert(!info.hasVertices());
    assert(!info.hasFaces());
    assert(!info.hasEdges());
    assert(!info.hasMaterials());

    // Manual setters
    info.setVertices(true);
    info.setVertexPosition(true, PrimitiveType::FLOAT);
    info.setVertexNormal(true, PrimitiveType::FLOAT);
    info.setVertexColor(true, PrimitiveType::UCHAR);
    info.setFaces(true);
    info.setFaceVertexReferences(true);
    info.setFaceNormal(true, PrimitiveType::DOUBLE);
    info.setTriangleMesh();

    assert(!info.isEmpty());
    assert(info.isTriangleMesh());
    assert(!info.isQuadMesh());
    assert(!info.isPolygonMesh());
    assert(info.hasVertices());
    assert(info.hasVertexPosition());
    assert(info.vertexPositionType() == PrimitiveType::FLOAT);
    assert(info.hasVertexNormal());
    assert(info.vertexNormalType() == PrimitiveType::FLOAT);
    assert(info.hasVertexColor());
    assert(info.vertexColorType() == PrimitiveType::UCHAR);
    assert(info.hasFaces());
    assert(info.hasFaceVertexReferences());
    assert(info.hasFaceNormal());
    assert(info.faceNormalType() == PrimitiveType::DOUBLE);
    assert(!info.hasEdges());

    // Update mesh type
    MeshInfo polyInfo;
    polyInfo.updateMeshType(5);
    assert(polyInfo.isPolygonMesh());

    MeshInfo quadInfo;
    quadInfo.updateMeshType(4);
    assert(quadInfo.isQuadMesh());

    MeshInfo triInfo;
    triInfo.updateMeshType(3);
    assert(triInfo.isTriangleMesh());

    // Clear
    info.clear();
    assert(info.isEmpty());
    assert(info.isUnkownMesh());
    assert(!info.hasVertices());
}

inline void testMeshInfoFromMesh()
{
    // 1. MyMesh with enabled optional normal and color
    {
        MyMesh mesh = createSampleQuadMesh();
        MeshInfo info(mesh);

        assert(info.hasVertices());
        assert(info.hasVertexPosition());
        assert(info.vertexPositionType() == PrimitiveType::FLOAT);
        assert(info.hasVertexBitFlags());
        assert(info.hasVertexNormal());
        assert(info.vertexNormalType() == PrimitiveType::FLOAT);
        assert(info.hasVertexColor());
        assert(info.vertexColorType() == PrimitiveType::UCHAR);

        assert(info.hasFaces());
        assert(info.hasFaceVertexReferences());
        assert(info.hasFaceBitFlags());
        assert(info.hasFaceColor());
        assert(info.faceColorType() == PrimitiveType::UCHAR);
        assert(info.isTriangleMesh());

        assert(!info.hasEdges());
        assert(!info.hasMaterials());
    }

    // 2. MyMesh with disabled optional components
    {
        MyMesh emptyMesh;
        MeshInfo info(emptyMesh);

        assert(info.hasVertices());
        assert(info.hasVertexPosition());
        assert(!info.hasVertexNormal());
        assert(!info.hasVertexColor());

        assert(info.hasFaces());
        assert(!info.hasFaceColor());
        assert(info.isTriangleMesh());
    }

    // 3. MeshDouble (scalar type is double)
    {
        MeshDouble dm;
        MeshInfo info(dm);

        assert(info.hasVertices());
        assert(info.hasVertexPosition());
        assert(info.vertexPositionType() == PrimitiveType::DOUBLE);
        assert(info.hasFaces());
        assert(info.isTriangleMesh());
    }

    // 4. QuadMesh and PolyMesh
    {
        SimpleQuadMesh qm;
        MeshInfo qInfo(qm);
        assert(qInfo.hasFaces());
        assert(qInfo.isQuadMesh());

        SimplePolyMesh pm;
        MeshInfo pInfo(pm);
        assert(pInfo.hasFaces());
        assert(pInfo.isPolygonMesh());
    }

    // 5. EdgeMesh
    {
        MyEdgeMesh em = createSampleEdgeMesh();
        MeshInfo info(em);

        assert(info.hasVertices());
        assert(info.hasVertexPosition());
        assert(info.hasEdges());
        assert(info.hasEdgeVertexReferences());
        assert(info.hasEdgeBitFlags());
        assert(!info.hasFaces());
    }

    // 6. MaterialMeshWithMaterials
    {
        MaterialMeshWithMaterials mm = createSampleMaterialMesh();
        MeshInfo info(mm);

        assert(info.hasMaterials());
        assert(info.hasFaces());
        assert(info.hasFaceMaterialIndex());
        assert(info.faceMaterialIndexType() == PrimitiveType::UINT);
    }

    // 7. CCMesh (Custom Components)
    {
        CCMesh ccm;
        ccm.addVertexCustomComponent<double>("curvature");
        ccm.addFaceCustomComponent<int>("cluster_id");

        MeshInfo info(ccm);

        assert(info.hasVertexCustomComponents());
        assert(info.vertexCustomComponents().size() == 1);
        assert(info.vertexCustomComponents()[0].name == "curvature");
        assert(info.vertexCustomComponents()[0].type == PrimitiveType::DOUBLE);

        assert(info.hasFaceCustomComponents());
        assert(info.faceCustomComponents().size() == 1);
        assert(info.faceCustomComponents()[0].name == "cluster_id");
        assert(info.faceCustomComponents()[0].type == PrimitiveType::INT);
    }
}

inline void testMeshInfoIntersect()
{
    MeshInfo info1;
    info1.setVertices(true);
    info1.setVertexPosition(true, PrimitiveType::FLOAT);
    info1.setVertexNormal(true, PrimitiveType::FLOAT);
    info1.setFaces(true);
    info1.setFaceColor(true, PrimitiveType::UCHAR);
    info1.setTriangleMesh();

    MeshInfo info2;
    info2.setVertices(true);
    info2.setVertexPosition(true, PrimitiveType::DOUBLE);
    info2.setVertexColor(true, PrimitiveType::UCHAR);
    info2.setFaces(true);
    info2.setFaceColor(true, PrimitiveType::UCHAR);
    info2.setTriangleMesh();

    MeshInfo inter = info1.intersect(info2);

    assert(inter.hasVertices());
    assert(inter.hasVertexPosition());
    assert(inter.vertexPositionType() == PrimitiveType::FLOAT); // Taken from info1
    assert(!inter.hasVertexNormal()); // Not in info2
    assert(!inter.hasVertexColor());  // Not in info1
    assert(inter.hasFaces());
    assert(inter.hasFaceColor());
    assert(inter.isTriangleMesh());
}

inline void testMeshInfoEnableOptionalComponents()
{
    // 1. Enable optional components on MyMesh
    {
        MyMesh mesh;
        assert(!mesh.isVertexNormalAvailable());
        assert(!mesh.isVertexColorAvailable());
        assert(!mesh.isFaceColorAvailable());

        MeshInfo reqInfo;
        reqInfo.setVertices(true);
        reqInfo.setVertexNormal(true);
        reqInfo.setVertexColor(true);
        reqInfo.setFaces(true);
        reqInfo.setFaceColor(true);

        enableOptionalComponentsFromInfo(reqInfo, mesh);

        assert(mesh.isVertexNormalAvailable());
        assert(mesh.isVertexColorAvailable());
        assert(mesh.isFaceColorAvailable());
        assert(reqInfo.hasVertexNormal());
        assert(reqInfo.hasVertexColor());
        assert(reqInfo.hasFaceColor());

        // Component not supported by MyMesh (e.g. vertex tangent)
        MeshInfo tangentInfo;
        tangentInfo.setVertices(true);
        tangentInfo.setVertexTangent(true);
        enableOptionalComponentsFromInfo(tangentInfo, mesh);
        // info should have tangent reset to false since mesh cannot support it
        assert(!tangentInfo.hasVertexTangent());
    }

    // 2. Custom components enablement on CCMesh
    {
        CCMesh ccm;
        assert(!ccm.hasVertexCustomComponent("weight"));
        assert(!ccm.hasFaceCustomComponent("label"));

        MeshInfo ccInfo;
        ccInfo.setVertices(true);
        ccInfo.addVertexCustomComponent("weight", PrimitiveType::FLOAT);
        ccInfo.setFaces(true);
        ccInfo.addFaceCustomComponent("label", PrimitiveType::INT);

        enableOptionalComponentsFromInfo(ccInfo, ccm);

        assert(ccm.hasVertexCustomComponent("weight"));
        assert(ccm.isVertexCustomComponentOfType<float>("weight"));
        assert(ccm.hasFaceCustomComponent("label"));
        assert(ccm.isFaceCustomComponentOfType<int>("label"));
    }
}

inline void testMeshInfo()
{
    std::println("--- Testing MeshInfo ---");
    testMeshInfoDefaultAndManual();
    testMeshInfoFromMesh();
    testMeshInfoIntersect();
    testMeshInfoEnableOptionalComponents();
    std::println("MeshInfo tests passed!");
}

#endif // TEST_MESH_INFO_H
