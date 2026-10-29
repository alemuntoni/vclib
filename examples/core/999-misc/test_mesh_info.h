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
    info.setPerVertexPosition(true, PrimitiveType::FLOAT);
    info.setPerVertexNormal(true, PrimitiveType::FLOAT);
    info.setPerVertexColor(true, PrimitiveType::UCHAR);
    info.setFaces(true);
    info.setPerFaceVertexReferences(true);
    info.setPerFaceNormal(true, PrimitiveType::DOUBLE);
    info.setTriangleMesh();

    assert(!info.isEmpty());
    assert(info.isTriangleMesh());
    assert(!info.isQuadMesh());
    assert(!info.isPolygonMesh());
    assert(info.hasVertices());
    assert(info.hasPerVertexPosition());
    assert(info.perVertexPositionType() == PrimitiveType::FLOAT);
    assert(info.hasPerVertexNormal());
    assert(info.perVertexNormalType() == PrimitiveType::FLOAT);
    assert(info.hasPerVertexColor());
    assert(info.perVertexColorType() == PrimitiveType::UCHAR);
    assert(info.hasFaces());
    assert(info.hasPerFaceVertexReferences());
    assert(info.hasPerFaceNormal());
    assert(info.perFaceNormalType() == PrimitiveType::DOUBLE);
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
        assert(info.hasPerVertexPosition());
        assert(info.perVertexPositionType() == PrimitiveType::FLOAT);
        assert(info.hasPerVertexBitFlags());
        assert(info.hasPerVertexNormal());
        assert(info.perVertexNormalType() == PrimitiveType::FLOAT);
        assert(info.hasPerVertexColor());
        assert(info.perVertexColorType() == PrimitiveType::UCHAR);

        assert(info.hasFaces());
        assert(info.hasPerFaceVertexReferences());
        assert(info.hasPerFaceBitFlags());
        assert(info.hasPerFaceColor());
        assert(info.perFaceColorType() == PrimitiveType::UCHAR);
        assert(info.isTriangleMesh());

        assert(!info.hasEdges());
        assert(!info.hasMaterials());
    }

    // 2. MyMesh with disabled optional components
    {
        MyMesh emptyMesh;
        MeshInfo info(emptyMesh);

        assert(info.hasVertices());
        assert(info.hasPerVertexPosition());
        assert(!info.hasPerVertexNormal());
        assert(!info.hasPerVertexColor());

        assert(info.hasFaces());
        assert(!info.hasPerFaceColor());
        assert(info.isTriangleMesh());
    }

    // 3. MeshDouble (scalar type is double)
    {
        MeshDouble dm;
        MeshInfo info(dm);

        assert(info.hasVertices());
        assert(info.hasPerVertexPosition());
        assert(info.perVertexPositionType() == PrimitiveType::DOUBLE);
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
        assert(info.hasPerVertexPosition());
        assert(info.hasEdges());
        assert(info.hasPerEdgeVertexReferences());
        assert(info.hasPerEdgeBitFlags());
        assert(!info.hasFaces());
    }

    // 6. MaterialMeshWithMaterials
    {
        MaterialMeshWithMaterials mm = createSampleMaterialMesh();
        MeshInfo info(mm);

        assert(info.hasMaterials());
        assert(info.hasFaces());
        assert(info.hasPerFaceMaterialIndex());
        assert(info.perFaceMaterialIndexType() == PrimitiveType::UINT);
    }

    // 7. CCMesh (Custom Components)
    {
        CCMesh ccm;
        ccm.addVertexCustomComponent<double>("curvature");
        ccm.addFaceCustomComponent<int>("cluster_id");

        MeshInfo info(ccm);

        assert(info.hasPerVertexCustomComponents());
        assert(info.perVertexCustomComponents().size() == 1);
        assert(info.perVertexCustomComponents()[0].name == "curvature");
        assert(info.perVertexCustomComponents()[0].type == PrimitiveType::DOUBLE);

        assert(info.hasPerFaceCustomComponents());
        assert(info.perFaceCustomComponents().size() == 1);
        assert(info.perFaceCustomComponents()[0].name == "cluster_id");
        assert(info.perFaceCustomComponents()[0].type == PrimitiveType::INT);
    }
}

inline void testMeshInfoIntersect()
{
    MeshInfo info1;
    info1.setVertices(true);
    info1.setPerVertexPosition(true, PrimitiveType::FLOAT);
    info1.setPerVertexNormal(true, PrimitiveType::FLOAT);
    info1.setFaces(true);
    info1.setPerFaceColor(true, PrimitiveType::UCHAR);
    info1.setTriangleMesh();

    MeshInfo info2;
    info2.setVertices(true);
    info2.setPerVertexPosition(true, PrimitiveType::DOUBLE);
    info2.setPerVertexColor(true, PrimitiveType::UCHAR);
    info2.setFaces(true);
    info2.setPerFaceColor(true, PrimitiveType::UCHAR);
    info2.setTriangleMesh();

    MeshInfo inter = info1.intersect(info2);

    assert(inter.hasVertices());
    assert(inter.hasPerVertexPosition());
    assert(inter.perVertexPositionType() == PrimitiveType::FLOAT); // Taken from info1
    assert(!inter.hasPerVertexNormal()); // Not in info2
    assert(!inter.hasPerVertexColor());  // Not in info1
    assert(inter.hasFaces());
    assert(inter.hasPerFaceColor());
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
        reqInfo.setPerVertexNormal(true);
        reqInfo.setPerVertexColor(true);
        reqInfo.setFaces(true);
        reqInfo.setPerFaceColor(true);

        enableOptionalComponentsFromInfo(reqInfo, mesh);

        assert(mesh.isVertexNormalAvailable());
        assert(mesh.isVertexColorAvailable());
        assert(mesh.isFaceColorAvailable());
        assert(reqInfo.hasPerVertexNormal());
        assert(reqInfo.hasPerVertexColor());
        assert(reqInfo.hasPerFaceColor());

        // Component not supported by MyMesh (e.g. vertex tangent)
        MeshInfo tangentInfo;
        tangentInfo.setVertices(true);
        tangentInfo.setPerVertexTangent(true);
        enableOptionalComponentsFromInfo(tangentInfo, mesh);
        // info should have tangent reset to false since mesh cannot support it
        assert(!tangentInfo.hasPerVertexTangent());
    }

    // 2. Custom components enablement on CCMesh
    {
        CCMesh ccm;
        assert(!ccm.hasVertexCustomComponent("weight"));
        assert(!ccm.hasFaceCustomComponent("label"));

        MeshInfo ccInfo;
        ccInfo.setVertices(true);
        ccInfo.addPerVertexCustomComponent("weight", PrimitiveType::FLOAT);
        ccInfo.setFaces(true);
        ccInfo.addPerFaceCustomComponent("label", PrimitiveType::INT);

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
