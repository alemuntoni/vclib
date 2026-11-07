// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Import From - Vertex Import From")
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
    mSource.vertex(0).normal()   = Point3f(1.0f, 0.0f, 0.0f);
    mSource.vertex(0).color()    = vcl::Color::Green;
    mSource.vertex(0).selected() = true;
    mSource.vertex(0).visited()  = true;
    mSource.vertex(0).onBorder() = true;
    mSource.vertex(0).userBit(3) = true;

    mFloat.vertex(0).position() = Point3f(0, 0, 0);
    mFloat.vertex(0).normal()   = Point3f(0, 0, 0);
    mFloat.vertex(0).importFrom(mSource.vertex(0));
    REQUIRE(mFloat.vertex(0).position() == Point3f(1.5f, 2.5f, 3.5f));
    REQUIRE(mFloat.vertex(0).normal() == Point3f(1.0f, 0.0f, 0.0f));
    REQUIRE(mFloat.vertex(0).color() == vcl::Color::Green);
    REQUIRE(mFloat.vertex(0).selected() == true);
    REQUIRE(mFloat.vertex(0).visited() == true);
    REQUIRE(mFloat.vertex(0).onBorder() == true);
    REQUIRE(mFloat.vertex(0).userBit(3) == true);
    REQUIRE(mFloat.vertex(0).userBit(2) == false);

    // 3. PrincipalCurvature importFrom on Vertex
    CurvatureMesh cm1;
    cm1.addVertices(1);
    cm1.vertex(0).principalCurvature().maxDir()   = Point3f(1.0f, 0.0f, 0.0f);
    cm1.vertex(0).principalCurvature().minDir()   = Point3f(0.0f, 1.0f, 0.0f);
    cm1.vertex(0).principalCurvature().maxValue() = 3.5f;
    cm1.vertex(0).principalCurvature().minValue() = 1.25f;

    CurvatureMesh cm2;
    cm2.addVertices(1);
    cm2.vertex(0).importFrom(cm1.vertex(0));
    REQUIRE(
        cm2.vertex(0).principalCurvature().maxDir() ==
        Point3f(1.0f, 0.0f, 0.0f));
    REQUIRE(
        cm2.vertex(0).principalCurvature().minDir() ==
        Point3f(0.0f, 1.0f, 0.0f));
    REQUIRE(cm2.vertex(0).principalCurvature().maxValue() == 3.5f);
    REQUIRE(cm2.vertex(0).principalCurvature().minValue() == 1.25f);

    // 4. PrincipalCurvature importFrom with cross-precision cast (float to
    // double) into optional component
    OptionalCurvatureMesh ocm;
    ocm.addVertices(1);
    ocm.enableElementComponent<ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>();
    ocm.vertex(0).importFrom(cm1.vertex(0));
    REQUIRE(
        ocm.vertex(0).principalCurvature().maxDir() == Point3d(1.0, 0.0, 0.0));
    REQUIRE(
        ocm.vertex(0).principalCurvature().minDir() == Point3d(0.0, 1.0, 0.0));
    REQUIRE(ocm.vertex(0).principalCurvature().maxValue() == 3.5);
    REQUIRE(ocm.vertex(0).principalCurvature().minValue() == 1.25);

    // 5. Quality importFrom on Vertex
    QualityMesh qm1;
    qm1.addVertices(1);
    qm1.vertex(0).quality() = 42.5f;

    QualityMesh qm2;
    qm2.addVertices(1);
    qm2.vertex(0).importFrom(qm1.vertex(0));
    REQUIRE(qm2.vertex(0).quality() == 42.5f);

    // 6. Quality importFrom with cross-precision cast (float to double) into
    // optional component
    OptionalQualityMesh oqm;
    oqm.addVertices(1);
    oqm.enableElementComponent<ElemId::VERTEX, CompId::QUALITY>();
    oqm.vertex(0).importFrom(qm1.vertex(0));
    REQUIRE(oqm.vertex(0).quality() == 42.5);

    // 7. Tangent importFrom on Vertex
    TangentMesh tm1;
    tm1.addVertices(1);
    tm1.vertex(0).tangent()            = Point3f(1.0f, 0.0f, 0.0f);
    tm1.vertex(0).tangentRightHanded() = false;

    TangentMesh tm2;
    tm2.addVertices(1);
    tm2.vertex(0).importFrom(tm1.vertex(0));
    REQUIRE(tm2.vertex(0).tangent() == Point3f(1.0f, 0.0f, 0.0f));
    REQUIRE(tm2.vertex(0).tangentRightHanded() == false);

    // 8. Tangent importFrom with cross-precision cast (float to double) into
    // optional component
    OptionalTangentMesh otm;
    otm.addVertices(1);
    otm.enableElementComponent<ElemId::VERTEX, CompId::TANGENT>();
    otm.vertex(0).importFrom(tm1.vertex(0));
    REQUIRE(otm.vertex(0).tangent() == Point3d(1.0, 0.0, 0.0));
    REQUIRE(otm.vertex(0).tangentRightHanded() == false);
}

TEST_CASE("Import From - Face Import From")
{
    // 1. Triangle to Poly Face import
    TriBitMesh mSourceTri;
    mSourceTri.addVertices(3);
    mSourceTri.addFace(0, 1, 2);
    mSourceTri.face(0).edgeOnBorder(1) = true;
    mSourceTri.face(0).edgeSelected(2) = true;
    mSourceTri.face(0).edgeFaux(0)     = true;

    SimplePolyMesh polyMesh;
    polyMesh.addVertices(3);
    polyMesh.addFace(0, 1, 2);

    polyMesh.face(0).importFrom(mSourceTri.face(0));
    REQUIRE(polyMesh.face(0).vertexCount() == 3);
    REQUIRE(polyMesh.face(0).vertexIndex(0) == 0);
    REQUIRE(polyMesh.face(0).vertexIndex(1) == 1);
    REQUIRE(polyMesh.face(0).vertexIndex(2) == 2);
    REQUIRE(polyMesh.face(0).edgeOnBorder(1) == true);
    REQUIRE(polyMesh.face(0).edgeSelected(2) == true);
    REQUIRE(polyMesh.face(0).edgeFaux(0) == true);

    // 2. Poly (3 vertices) to Triangle Face import
    TriBitMesh mDestTri;
    mDestTri.addVertices(3);
    mDestTri.addFace(9, 9, 9);
    mDestTri.face(0).importFrom(polyMesh.face(0));
    REQUIRE(mDestTri.face(0).vertexIndex(0) == 0);
    REQUIRE(mDestTri.face(0).vertexIndex(1) == 1);
    REQUIRE(mDestTri.face(0).vertexIndex(2) == 2);
    REQUIRE(mDestTri.face(0).edgeOnBorder(1) == true);
    REQUIRE(mDestTri.face(0).edgeSelected(2) == true);
    REQUIRE(mDestTri.face(0).edgeFaux(0) == true);
    REQUIRE(mDestTri.face(0).edgeOnBorder(0) == false);

    // 3. importRefs = false on Face
    mDestTri.face(0).setVertices(7, 8, 9);
    mDestTri.face(0).importFrom(mSourceTri.face(0), false);
    REQUIRE(mDestTri.face(0).vertexIndex(0) == 7);
    REQUIRE(mDestTri.face(0).vertexIndex(1) == 8);
    REQUIRE(mDestTri.face(0).vertexIndex(2) == 9);

    // 4. Quality importFrom on Face
    QualityMesh fqm1;
    fqm1.addVertices(3);
    fqm1.addFace(0, 1, 2);
    fqm1.enableElementComponent<ElemId::FACE, CompId::QUALITY>();
    fqm1.face(0).quality() = 12.5f;

    QualityMesh fqm2;
    fqm2.addVertices(3);
    fqm2.addFace(0, 1, 2);
    fqm2.enableElementComponent<ElemId::FACE, CompId::QUALITY>();
    fqm2.face(0).importFrom(fqm1.face(0));
    REQUIRE(fqm2.face(0).quality() == 12.5f);

    // 5. Tangent importFrom on Face
    TangentMesh ftm1;
    ftm1.addVertices(3);
    ftm1.addFace(0, 1, 2);
    ftm1.enableElementComponent<ElemId::FACE, CompId::TANGENT>();
    ftm1.face(0).tangent()            = Point3f(0.0f, 1.0f, 0.0f);
    ftm1.face(0).tangentRightHanded() = false;

    TangentMesh ftm2;
    ftm2.addVertices(3);
    ftm2.addFace(0, 1, 2);
    ftm2.enableElementComponent<ElemId::FACE, CompId::TANGENT>();
    ftm2.face(0).importFrom(ftm1.face(0));
    REQUIRE(ftm2.face(0).tangent() == Point3f(0.0f, 1.0f, 0.0f));
    REQUIRE(ftm2.face(0).tangentRightHanded() == false);
}

TEST_CASE("Import From - Mesh Import From Tri To Tri")
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
    REQUIRE(mTriDest.vertexContainerSize() == 3);
    REQUIRE(mTriDest.vertex(0).position() == Point3f(1, 2, 3));
    REQUIRE(mTriDest.vertex(1).position() == Point3f(4, 5, 6));
    REQUIRE(mTriDest.vertex(1).selected() == true);
    REQUIRE(mTriDest.vertex(0).selected() == false);

    REQUIRE(mTriDest.faceContainerSize() == 1);
    REQUIRE(mTriDest.face(0).vertexIndex(0) == 0);
    REQUIRE(mTriDest.face(0).vertexIndex(1) == 1);
    REQUIRE(mTriDest.face(0).vertexIndex(2) == 2);
    REQUIRE(mTriDest.face(0).edgeSelected(1) == true);
}

TEST_CASE("Import From - Mesh Import From Poly To Tri")
{
    // 1. Pentagon (5 vertices) -> should produce 3 triangles
    SimplePolyMesh polySource;
    polySource.addVertices(5);
    polySource.vertex(0).position() = Point3f(0, 0, 0);
    polySource.vertex(1).position() = Point3f(2, 0, 0);
    polySource.vertex(2).position() = Point3f(2, 2, 0);
    polySource.vertex(3).position() = Point3f(1, 3, 0);
    polySource.vertex(4).position() = Point3f(0, 2, 0);

    polySource.addFace(0, 1, 2, 3, 4);

    TriBitMesh triDest;
    triDest.importFrom(polySource);
    REQUIRE(triDest.vertexCount() == 5);
    REQUIRE(triDest.faceCount() == 3);
    REQUIRE(triDest.faceContainerSize() == 3);

    // Each triangle must have 3 valid vertex indices from {0, 1, 2, 3, 4}
    for (vcl::uint fi = 0; fi < 3; ++fi) {
        REQUIRE(triDest.face(fi).vertexCount() == 3);
        for (vcl::uint vi = 0; vi < 3; ++vi) {
            REQUIRE(triDest.face(fi).vertexIndex(vi) <= 4);
        }
    }

    // 2. Mix: 1 triangle and 1 quad (4 vertices) -> 1 + 2 = 3 triangles
    SimplePolyMesh polyMixed;
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
    REQUIRE(triMixed.faceCount() == 3);
    REQUIRE(triMixed.faceContainerSize() == 3);
    REQUIRE(triMixed.face(0).vertexIndex(0) == 0);
    REQUIRE(triMixed.face(0).vertexIndex(1) == 1);
    REQUIRE(triMixed.face(0).vertexIndex(2) == 2);
}

TEST_CASE("Import From - Mesh Import From Mesh Components")
{
    MyMesh m1;
    m1.color() = Color::Red;
    m1.addVertices(3);
    m1.addFace(0, 1, 2);

    MyMesh m2;
    m2.importFrom(m1);
    REQUIRE(m2.color() == Color::Red);
    REQUIRE(m2.vertexCount() == 3);
    REQUIRE(m2.faceCount() == 1);

    // TransformMatrix importFrom test
    TransformedMesh tm1;
    tm1.color()           = Color::Blue;
    Matrix44f mat         = Matrix44f::Identity();
    mat(0, 3)             = 99.0f;
    tm1.transformMatrix() = mat;
    tm1.addVertices(3);
    tm1.addFace(0, 1, 2);

    TransformedMesh tm2;
    tm2.importFrom(tm1);
    REQUIRE(tm2.color() == Color::Blue);
    REQUIRE(tm2.transformMatrix() == mat);
    REQUIRE(tm2.vertexCount() == 3);
    REQUIRE(tm2.faceCount() == 1);

    // BoundingBox importFrom test
    BoundingBoxMesh bbm1;
    bbm1.boundingBox().min() = Point3f(-1.0f, -2.0f, -3.0f);
    bbm1.boundingBox().max() = Point3f(1.0f, 2.0f, 3.0f);
    bbm1.addVertices(3);
    bbm1.addFace(0, 1, 2);

    BoundingBoxMesh bbm2;
    bbm2.importFrom(bbm1);
    REQUIRE(bbm2.boundingBox().min() == Point3f(-1.0f, -2.0f, -3.0f));
    REQUIRE(bbm2.boundingBox().max() == Point3f(1.0f, 2.0f, 3.0f));
    REQUIRE(bbm2.vertexCount() == 3);
    REQUIRE(bbm2.faceCount() == 1);

    // transformedBoundingBox test
    TransformedBoundingBoxMesh tbbm;
    tbbm.boundingBox().min() = Point3f(0.0f, 0.0f, 0.0f);
    tbbm.boundingBox().max() = Point3f(1.0f, 1.0f, 1.0f);
    REQUIRE(tbbm.transformedBoundingBox().min() == Point3f(0.0f, 0.0f, 0.0f));
    REQUIRE(tbbm.transformedBoundingBox().max() == Point3f(1.0f, 1.0f, 1.0f));

    Matrix44f tmat         = Matrix44f::Identity();
    tmat(0, 3)             = 10.0f; // translate X by +10
    tbbm.transformMatrix() = tmat;

    REQUIRE(tbbm.boundingBox().min() == Point3f(0.0f, 0.0f, 0.0f));
    REQUIRE(tbbm.transformedBoundingBox().min() == Point3f(10.0f, 0.0f, 0.0f));
    REQUIRE(tbbm.transformedBoundingBox().max() == Point3f(11.0f, 1.0f, 1.0f));

    // Name importFrom test
    NamedMesh nm1;
    nm1.name() = "SampleMesh";
    nm1.addVertices(3);
    nm1.addFace(0, 1, 2);

    NamedMesh nm2;
    nm2.importFrom(nm1);
    REQUIRE(nm2.name() == "SampleMesh");
    REQUIRE(nm2.vertexCount() == 3);
    REQUIRE(nm2.faceCount() == 1);
}
