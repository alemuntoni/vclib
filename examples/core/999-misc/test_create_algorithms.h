// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CREATE_ALGORITHMS_H
#define TEST_CREATE_ALGORITHMS_H

#include "types.h"

#include <vclib/algorithms/mesh/create.h>
#include <vclib/meshes.h>

#include <cassert>
#include <print>

inline void testCreateTetrahedron()
{
    // Default TriMesh tetrahedron
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    assert(tet.vertexCount() == 4);
    assert(tet.faceCount() == 4);

    // PolyMesh tetrahedron
    vcl::PolyMesh tetPoly = vcl::createTetrahedron<vcl::PolyMesh>();
    assert(tetPoly.vertexCount() == 4);
    assert(tetPoly.faceCount() == 4);

    // Custom points tetrahedron
    vcl::Point3d p0(0, 0, 0);
    vcl::Point3d p1(1, 0, 0);
    vcl::Point3d p2(0, 1, 0);
    vcl::Point3d p3(0, 0, 1);
    vcl::TriMesh tetCustom =
        vcl::createTetrahedron<vcl::TriMesh>(p0, p1, p2, p3);
    assert(tetCustom.vertexCount() == 4);
    assert(tetCustom.faceCount() == 4);
}

inline void testCreateHexahedron()
{
    // TriMesh hexahedron (triangulated, 12 faces)
    vcl::TriMesh hexTri = vcl::createHexahedron<vcl::TriMesh>();
    assert(hexTri.vertexCount() == 8);
    assert(hexTri.faceCount() == 12);

    // PolyMesh hexahedron (quads, 6 faces)
    vcl::PolyMesh hexPoly = vcl::createHexahedron<vcl::PolyMesh>();
    assert(hexPoly.vertexCount() == 8);
    assert(hexPoly.faceCount() == 6);

    // Custom min/max hexahedron
    vcl::Point3d min(-2, -2, -2);
    vcl::Point3d max(2, 2, 2);
    vcl::TriMesh hexCustom =
        vcl::createHexahedron<vcl::TriMesh>(min, max);
    assert(hexCustom.vertexCount() == 8);
    assert(hexCustom.faceCount() == 12);

    // Default Cube
    vcl::TriMesh cubeTri = vcl::createCube<vcl::TriMesh>();
    assert(cubeTri.vertexCount() == 8);
    assert(cubeTri.faceCount() == 12);

    vcl::PolyMesh cubePoly = vcl::createCube<vcl::PolyMesh>();
    assert(cubePoly.vertexCount() == 8);
    assert(cubePoly.faceCount() == 6);

    // Parametric Cube
    vcl::TriMesh customCube =
        vcl::createCube<vcl::TriMesh>(vcl::Point3d(0, 0, 0), 2.5);
    assert(customCube.vertexCount() == 8);
    assert(customCube.faceCount() == 12);
}

inline void testCreateDodecahedron()
{
    // TriMesh dodecahedron (triangulated pentagons)
    vcl::TriMesh dodTri = vcl::createDodecahedron<vcl::TriMesh>();
    assert(dodTri.vertexCount() == 20);
    assert(dodTri.faceCount() == 36);

    // PolyMesh dodecahedron (12 pentagons)
    vcl::PolyMesh dodPoly = vcl::createDodecahedron<vcl::PolyMesh>();
    assert(dodPoly.vertexCount() == 20);
    assert(dodPoly.faceCount() == 12);
}

inline void testCreateIcosahedron()
{
    // Default icosahedron
    vcl::TriMesh ico = vcl::createIcosahedron<vcl::TriMesh>();
    assert(ico.vertexCount() == 12);
    assert(ico.faceCount() == 20);

    // Normalized vertices icosahedron
    vcl::TriMesh icoNorm = vcl::createIcosahedron<vcl::TriMesh>(true);
    assert(icoNorm.vertexCount() == 12);
    assert(icoNorm.faceCount() == 20);

    // PolyMesh icosahedron
    vcl::PolyMesh icoPoly = vcl::createIcosahedron<vcl::PolyMesh>();
    assert(icoPoly.vertexCount() == 12);
    assert(icoPoly.faceCount() == 20);
}

inline void testCreateSphere()
{
    // Default sphere (UV mode)
    vcl::TriMesh sp = vcl::createSphere<vcl::TriMesh>();
    assert(sp.vertexCount() > 0);
    assert(sp.faceCount() > 0);

    vcl::Sphere<double> s(vcl::Point3d(0, 0, 0), 1.0);

    // UV sphere
    vcl::TriMesh spUV = vcl::createSphereUV<vcl::TriMesh>(s, 8, 16);
    assert(spUV.vertexCount() > 0);
    assert(spUV.faceCount() > 0);

    // Normalized cube sphere
    vcl::TriMesh spNormCube =
        vcl::createSphereNormalizedCube<vcl::TriMesh>(s, 4);
    assert(spNormCube.vertexCount() > 0);
    assert(spNormCube.faceCount() > 0);

    // Spherified cube sphere (TriMesh)
    vcl::TriMesh spCubeTri =
        vcl::createSphereSpherifiedCube<vcl::TriMesh>(s, 4);
    assert(spCubeTri.vertexCount() > 0);
    assert(spCubeTri.faceCount() > 0);

    // Spherified cube sphere (PolyMesh - quads)
    vcl::PolyMesh spCubePoly =
        vcl::createSphereSpherifiedCube<vcl::PolyMesh>(s, 4);
    assert(spCubePoly.vertexCount() > 0);
    assert(spCubePoly.faceCount() > 0);

    // Icosahedron sphere
    vcl::TriMesh spIco =
        vcl::createSphereIcosahedron<vcl::TriMesh>(s, 1);
    assert(spIco.vertexCount() > 0);
    assert(spIco.faceCount() > 0);
}

inline void testCreateConeAndCylinder()
{
    // Cone centered at origin
    vcl::TriMesh cone = vcl::createCone<vcl::TriMesh>(1.0, 0.0, 2.0, 16);
    assert(cone.vertexCount() == 18);
    assert(cone.faceCount() == 32);

    // Frustum cone
    vcl::TriMesh frustum = vcl::createCone<vcl::TriMesh>(1.0, 0.5, 2.0, 16);
    assert(frustum.vertexCount() == 34);
    assert(frustum.faceCount() == 64);

    // Cone between two points
    vcl::TriMesh conePts = vcl::createCone<vcl::TriMesh>(
        vcl::Point3d(0, 0, 0), vcl::Point3d(0, 2, 0), 1.0, 0.0, 16);
    assert(conePts.vertexCount() == 18);
    assert(conePts.faceCount() == 32);

    // Cylinder centered at origin
    vcl::TriMesh cyl = vcl::createCylinder<vcl::TriMesh>(1.0, 2.0, 16);
    assert(cyl.vertexCount() == 34);
    assert(cyl.faceCount() == 64);

    // Cylinder between two points
    vcl::TriMesh cylPts = vcl::createCylinder<vcl::TriMesh>(
        vcl::Point3d(0, 0, 0), vcl::Point3d(0, 2, 0), 1.0, 16);
    assert(cylPts.vertexCount() == 34);
    assert(cylPts.faceCount() == 64);
}

inline void testCreateAxis()
{
    // Axis mesh (combined)
    vcl::TriMesh axis = vcl::createAxis<vcl::TriMesh>(1.0, true);
    assert(axis.vertexCount() > 0);
    assert(axis.faceCount() > 0);

    // Axis disjoint
    auto [cyl, conesph] = vcl::createAxisDisjoint<vcl::TriMesh>(1.0, false);
    assert(cyl.vertexCount() > 0);
    assert(conesph.vertexCount() > 0);

    // PolyMesh axis
    vcl::PolyMesh axisPoly = vcl::createAxis<vcl::PolyMesh>(1.0, false);
    assert(axisPoly.vertexCount() > 0);
    assert(axisPoly.faceCount() > 0);
}

inline void testCreateWithCustomAndFloatMeshes()
{
    // Float scalar meshes
    vcl::TriMeshf tetF = vcl::createTetrahedron<vcl::TriMeshf>();
    assert(tetF.vertexCount() == 4);

    vcl::TriMeshf hexF = vcl::createHexahedron<vcl::TriMeshf>();
    assert(hexF.vertexCount() == 8);

    vcl::TriMeshf cubeF = vcl::createCube<vcl::TriMeshf>();
    assert(cubeF.vertexCount() == 8);

    vcl::TriMeshf coneF =
        vcl::createCone<vcl::TriMeshf>(1.0f, 0.0f, 2.0f, 16);
    assert(coneF.vertexCount() == 18);

    vcl::TriMeshf cylF =
        vcl::createCylinder<vcl::TriMeshf>(1.0f, 2.0f, 16);
    assert(cylF.vertexCount() == 34);

    vcl::TriMeshf spF = vcl::createSphere<vcl::TriMeshf>();
    assert(spF.vertexCount() > 0);

    vcl::TriMeshf axisF = vcl::createAxis<vcl::TriMeshf>();
    assert(axisF.vertexCount() > 0);

    // Custom mesh with optional components (MyMesh from types.h)
    MyMesh myTet = vcl::createTetrahedron<MyMesh>();
    assert(myTet.vertexCount() == 4);

    MyMesh myHex = vcl::createHexahedron<MyMesh>();
    assert(myHex.vertexCount() == 8);

    MyMesh myCube = vcl::createCube<MyMesh>();
    assert(myCube.vertexCount() == 8);

    MyMesh myAxis = vcl::createAxis<MyMesh>();
    assert(myAxis.vertexCount() > 0);
}

inline void testCreateAlgorithms()
{
    std::println("--- Testing Create Mesh Algorithms ---");
    testCreateTetrahedron();
    testCreateHexahedron();
    testCreateDodecahedron();
    testCreateIcosahedron();
    testCreateSphere();
    testCreateConeAndCylinder();
    testCreateAxis();
    testCreateWithCustomAndFloatMeshes();
    std::println("--- Create Mesh Algorithms Tests Passed! ---");
}

#endif // TEST_CREATE_ALGORITHMS_H
