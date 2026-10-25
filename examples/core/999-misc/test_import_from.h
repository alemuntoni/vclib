// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_IMPORT_FROM_H
#define TEST_IMPORT_FROM_H

#include "types.h"

inline void testVertexImportFrom()
{
    // 1. Position importFrom with cross-scalar cast (double to float)
    MeshDouble mDouble;
    mDouble.addVertex(Point3d(1.5, 2.5, 3.5));

    MyMesh mFloat;
    mFloat.enableAllOptionalComponents();
    mFloat.addVertex(Point3f(0.0f, 0.0f, 0.0f));

    // 2. ElementProxy::importFrom on Vertex:
    // mFloat has Position, BitFlags, Normal (optional), Color (optional).
    // mSource has Position, BitFlags, Normal, Color.
    MyMesh mSource;
    mSource.enableAllOptionalComponents();
    mSource.addVertex(Point3f(1.5f, 2.5f, 3.5f));
    mSource.vertex(0).normal() = Point3f(1.0f, 0.0f, 0.0f);
    mSource.vertex(0).color() = vcl::Color::Green;
    mSource.vertex(0).selected() = true;
    mSource.vertex(0).visited() = true;
    mSource.vertex(0).onBorder() = true;
    mSource.vertex(0).userBit(3) = true;

    mFloat.vertex(0).position() = Point3f(0, 0, 0);
    mFloat.vertex(0).normal() = Point3f(0, 0, 0);
    mFloat.vertex(0).importFrom(mSource.vertex(0));
    assert(mFloat.vertex(0).position() == Point3f(1.5f, 2.5f, 3.5f));
    assert(mFloat.vertex(0).normal() == Point3f(1.0f, 0.0f, 0.0f));
    assert(mFloat.vertex(0).color() == vcl::Color::Green);
    assert(mFloat.vertex(0).selected() == true);
    assert(mFloat.vertex(0).visited() == true);
    assert(mFloat.vertex(0).onBorder() == true);
    assert(mFloat.vertex(0).userBit(3) == true);
    assert(mFloat.vertex(0).userBit(2) == false);

    // 3. PrincipalCurvature importFrom on Vertex
    CurvatureMesh cm1;
    cm1.addVertices(1);
    cm1.vertex(0).principalCurvature().maxDir() = Point3f(1.0f, 0.0f, 0.0f);
    cm1.vertex(0).principalCurvature().minDir() = Point3f(0.0f, 1.0f, 0.0f);
    cm1.vertex(0).principalCurvature().maxValue() = 3.5f;
    cm1.vertex(0).principalCurvature().minValue() = 1.2f;

    CurvatureMesh cm2;
    cm2.addVertices(1);
    cm2.vertex(0).importFrom(cm1.vertex(0));
    assert(cm2.vertex(0).principalCurvature().maxDir() == Point3f(1.0f, 0.0f, 0.0f));
    assert(cm2.vertex(0).principalCurvature().minDir() == Point3f(0.0f, 1.0f, 0.0f));
    assert(cm2.vertex(0).principalCurvature().maxValue() == 3.5f);
    assert(cm2.vertex(0).principalCurvature().minValue() == 1.2f);

    // 4. PrincipalCurvature importFrom with cross-precision cast (float to double) into optional component
    OptionalCurvatureMesh ocm;
    ocm.addVertices(1);
    ocm.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::PRINCIPAL_CURVATURE>();
    ocm.vertex(0).importFrom(cm1.vertex(0));
    assert(ocm.vertex(0).principalCurvature().maxDir() == Point3d(1.0, 0.0, 0.0));
    assert(ocm.vertex(0).principalCurvature().minDir() == Point3d(0.0, 1.0, 0.0));
    assert(ocm.vertex(0).principalCurvature().maxValue() == 3.5);
    assert(ocm.vertex(0).principalCurvature().minValue() == 1.2);

    // 5. Quality importFrom on Vertex
    QualityMesh qm1;
    qm1.addVertices(1);
    qm1.vertex(0).quality() = 42.5f;

    QualityMesh qm2;
    qm2.addVertices(1);
    qm2.vertex(0).importFrom(qm1.vertex(0));
    assert(qm2.vertex(0).quality() == 42.5f);

    // 6. Quality importFrom with cross-precision cast (float to double) into optional component
    OptionalQualityMesh oqm;
    oqm.addVertices(1);
    oqm.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::QUALITY>();
    oqm.vertex(0).importFrom(qm1.vertex(0));
    assert(oqm.vertex(0).quality() == 42.5);

    // 7. Tangent importFrom on Vertex
    TangentMesh tm1;
    tm1.addVertices(1);
    tm1.vertex(0).tangent() = Point3f(1.0f, 0.0f, 0.0f);
    tm1.vertex(0).tangentRightHanded() = false;

    TangentMesh tm2;
    tm2.addVertices(1);
    tm2.vertex(0).importFrom(tm1.vertex(0));
    assert(tm2.vertex(0).tangent() == Point3f(1.0f, 0.0f, 0.0f));
    assert(tm2.vertex(0).tangentRightHanded() == false);

    // 8. Tangent importFrom with cross-precision cast (float to double) into optional component
    OptionalTangentMesh otm;
    otm.addVertices(1);
    otm.enablePerElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::TANGENT>();
    otm.vertex(0).importFrom(tm1.vertex(0));
    assert(otm.vertex(0).tangent() == Point3d(1.0, 0.0, 0.0));
    assert(otm.vertex(0).tangentRightHanded() == false);
}

inline void testFaceImportFrom()
{
    // 1. Triangle to Poly Face import
    TriBitMesh mSourceTri;
    mSourceTri.addVertices(3);
    mSourceTri.addFace(0, 1, 2);
    mSourceTri.face(0).edgeOnBorder(1) = true;
    mSourceTri.face(0).edgeSelected(2) = true;
    mSourceTri.face(0).edgeFaux(0) = true;

    PolyMesh polyMesh;
    polyMesh.addVertices(3);
    polyMesh.addFace(0, 1, 2);

    polyMesh.face(0).importFrom(mSourceTri.face(0));
    assert(polyMesh.face(0).vertexCount() == 3);
    assert(polyMesh.face(0).vertexIndex(0) == 0);
    assert(polyMesh.face(0).vertexIndex(1) == 1);
    assert(polyMesh.face(0).vertexIndex(2) == 2);
    assert(polyMesh.face(0).edgeOnBorder(1) == true);
    assert(polyMesh.face(0).edgeSelected(2) == true);
    assert(polyMesh.face(0).edgeFaux(0) == true);

    // 2. Poly (3 vertices) to Triangle Face import
    TriBitMesh mDestTri;
    mDestTri.addVertices(3);
    mDestTri.addFace(9, 9, 9);
    mDestTri.face(0).importFrom(polyMesh.face(0));
    assert(mDestTri.face(0).vertexIndex(0) == 0);
    assert(mDestTri.face(0).vertexIndex(1) == 1);
    assert(mDestTri.face(0).vertexIndex(2) == 2);
    assert(mDestTri.face(0).edgeOnBorder(1) == true);
    assert(mDestTri.face(0).edgeSelected(2) == true);
    assert(mDestTri.face(0).edgeFaux(0) == true);
    assert(mDestTri.face(0).edgeOnBorder(0) == false);

    // 3. importRefs = false on Face
    mDestTri.face(0).setVertices(7, 8, 9);
    mDestTri.face(0).importFrom(mSourceTri.face(0), false);
    assert(mDestTri.face(0).vertexIndex(0) == 7);
    assert(mDestTri.face(0).vertexIndex(1) == 8);
    assert(mDestTri.face(0).vertexIndex(2) == 9);

    // 4. Quality importFrom on Face
    QualityMesh fqm1;
    fqm1.addVertices(3);
    fqm1.addFace(0, 1, 2);
    fqm1.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::QUALITY>();
    fqm1.face(0).quality() = 12.5f;

    QualityMesh fqm2;
    fqm2.addVertices(3);
    fqm2.addFace(0, 1, 2);
    fqm2.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::QUALITY>();
    fqm2.face(0).importFrom(fqm1.face(0));
    assert(fqm2.face(0).quality() == 12.5f);

    // 5. Tangent importFrom on Face
    TangentMesh ftm1;
    ftm1.addVertices(3);
    ftm1.addFace(0, 1, 2);
    ftm1.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::TANGENT>();
    ftm1.face(0).tangent() = Point3f(0.0f, 1.0f, 0.0f);
    ftm1.face(0).tangentRightHanded() = false;

    TangentMesh ftm2;
    ftm2.addVertices(3);
    ftm2.addFace(0, 1, 2);
    ftm2.enablePerElementComponent<meshv2::ElemId::FACE, meshv2::CompId::TANGENT>();
    ftm2.face(0).importFrom(ftm1.face(0));
    assert(ftm2.face(0).tangent() == Point3f(0.0f, 1.0f, 0.0f));
    assert(ftm2.face(0).tangentRightHanded() == false);
}

inline void testMeshImportFromTriToTri()
{
    TriBitMesh mTriSource;
    mTriSource.addVertices(3);
    mTriSource.vertex(0).position() = Point3f(1, 2, 3);
    mTriSource.vertex(1).position() = Point3f(4, 5, 6);
    mTriSource.vertex(2).position() = Point3f(7, 8, 9);
    mTriSource.vertex(1).selected() = true;
    mTriSource.addFace(0, 1, 2);
    mTriSource.face(0).edgeSelected(1) = true;

    TriBitMesh mTriDest;
    mTriDest.importFrom(mTriSource);
    assert(mTriDest.vertexContainerSize() == 3);
    assert(mTriDest.vertex(0).position() == Point3f(1, 2, 3));
    assert(mTriDest.vertex(1).position() == Point3f(4, 5, 6));
    assert(mTriDest.vertex(1).selected() == true);
    assert(mTriDest.vertex(0).selected() == false);

    assert(mTriDest.faceContainerSize() == 1);
    assert(mTriDest.face(0).vertexIndex(0) == 0);
    assert(mTriDest.face(0).vertexIndex(1) == 1);
    assert(mTriDest.face(0).vertexIndex(2) == 2);
    assert(mTriDest.face(0).edgeSelected(1) == true);
}

inline void testMeshImportFromPolyToTri()
{
    // 1. Pentagon (5 vertices) -> should produce 3 triangles
    PolyMesh polySource;
    polySource.addVertices(5);
    polySource.vertex(0).position() = Point3f(0, 0, 0);
    polySource.vertex(1).position() = Point3f(2, 0, 0);
    polySource.vertex(2).position() = Point3f(2, 2, 0);
    polySource.vertex(3).position() = Point3f(1, 3, 0);
    polySource.vertex(4).position() = Point3f(0, 2, 0);

    polySource.addFace(0, 1, 2, 3, 4);

    TriBitMesh triDest;
    triDest.importFrom(polySource);
    assert(triDest.vertexCount() == 5);
    assert(triDest.faceCount() == 3);
    assert(triDest.faceContainerSize() == 3);

    // Each triangle must have 3 valid vertex indices from {0, 1, 2, 3, 4}
    for (vcl::uint fi = 0; fi < 3; ++fi) {
        assert(triDest.face(fi).vertexCount() == 3);
        for (vcl::uint vi = 0; vi < 3; ++vi) {
            assert(triDest.face(fi).vertexIndex(vi) <= 4);
        }
    }

    // 2. Mix: 1 triangle and 1 quad (4 vertices) -> 1 + 2 = 3 triangles
    PolyMesh polyMixed;
    polyMixed.addVertices(7);
    polyMixed.vertex(0).position() = Point3f(0, 0, 0);
    polyMixed.vertex(1).position() = Point3f(1, 0, 0);
    polyMixed.vertex(2).position() = Point3f(0, 1, 0);
    polyMixed.addFace(0, 1, 2); // triangle (3 vertices)

    polyMixed.vertex(3).position() = Point3f(2, 0, 0);
    polyMixed.vertex(4).position() = Point3f(3, 0, 0);
    polyMixed.vertex(5).position() = Point3f(3, 1, 0);
    polyMixed.vertex(6).position() = Point3f(2, 1, 0);
    polyMixed.addFace(3, 4, 5, 6); // quad (4 vertices)

    TriBitMesh triMixed;
    triMixed.importFrom(polyMixed);
    assert(triMixed.faceCount() == 3);
    assert(triMixed.faceContainerSize() == 3);
    assert(triMixed.face(0).vertexIndex(0) == 0);
    assert(triMixed.face(0).vertexIndex(1) == 1);
    assert(triMixed.face(0).vertexIndex(2) == 2);
}

inline void testMeshImportFromMeshComponents()
{
    MyMesh m1;
    m1.color() = Color::Red;
    m1.addVertices(3);
    m1.addFace(0, 1, 2);

    MyMesh m2;
    m2.importFrom(m1);
    assert(m2.color() == Color::Red);
    assert(m2.vertexCount() == 3);
    assert(m2.faceCount() == 1);

    // TransformMatrix importFrom test
    TransformedMesh tm1;
    tm1.color() = Color::Blue;
    Matrix44f mat = Matrix44f::Identity();
    mat(0, 3) = 99.0f;
    tm1.transformMatrix() = mat;
    tm1.addVertices(3);
    tm1.addFace(0, 1, 2);

    TransformedMesh tm2;
    tm2.importFrom(tm1);
    assert(tm2.color() == Color::Blue);
    assert(tm2.transformMatrix() == mat);
    assert(tm2.vertexCount() == 3);
    assert(tm2.faceCount() == 1);

    // BoundingBox importFrom test
    BoundingBoxMesh bbm1;
    bbm1.boundingBox().min() = Point3f(-1.0f, -2.0f, -3.0f);
    bbm1.boundingBox().max() = Point3f(1.0f, 2.0f, 3.0f);
    bbm1.addVertices(3);
    bbm1.addFace(0, 1, 2);

    BoundingBoxMesh bbm2;
    bbm2.importFrom(bbm1);
    assert(bbm2.boundingBox().min() == Point3f(-1.0f, -2.0f, -3.0f));
    assert(bbm2.boundingBox().max() == Point3f(1.0f, 2.0f, 3.0f));
    assert(bbm2.vertexCount() == 3);
    assert(bbm2.faceCount() == 1);

    // transformedBoundingBox test
    TransformedBoundingBoxMesh tbbm;
    tbbm.boundingBox().min() = Point3f(0.0f, 0.0f, 0.0f);
    tbbm.boundingBox().max() = Point3f(1.0f, 1.0f, 1.0f);
    assert(tbbm.transformedBoundingBox().min() == Point3f(0.0f, 0.0f, 0.0f));
    assert(tbbm.transformedBoundingBox().max() == Point3f(1.0f, 1.0f, 1.0f));

    Matrix44f tmat = Matrix44f::Identity();
    tmat(0, 3) = 10.0f; // translate X by +10
    tbbm.transformMatrix() = tmat;

    assert(tbbm.boundingBox().min() == Point3f(0.0f, 0.0f, 0.0f));
    assert(tbbm.transformedBoundingBox().min() == Point3f(10.0f, 0.0f, 0.0f));
    assert(tbbm.transformedBoundingBox().max() == Point3f(11.0f, 1.0f, 1.0f));

    // Name importFrom test
    NamedMesh nm1;
    nm1.name() = "SampleMesh";
    nm1.addVertices(3);
    nm1.addFace(0, 1, 2);

    NamedMesh nm2;
    nm2.importFrom(nm1);
    assert(nm2.name() == "SampleMesh");
    assert(nm2.vertexCount() == 3);
    assert(nm2.faceCount() == 1);
}

inline void testImportFrom()
{
    std::println("--- Testing importFrom System ---");
    testVertexImportFrom();
    testFaceImportFrom();
    testMeshImportFromTriToTri();
    testMeshImportFromPolyToTri();
    testMeshImportFromMeshComponents();
    std::println("ElementProxy and Mesh-level importFrom verified successfully!\n");
}

#endif // TEST_IMPORT_FROM_H
