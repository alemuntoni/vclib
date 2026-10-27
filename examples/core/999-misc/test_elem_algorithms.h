// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ELEM_ALGORITHMS_H
#define TEST_ELEM_ALGORITHMS_H

#include "types.h"

#include <vclib/mesh_v2/elem_algorithms.h>

#include <cassert>
#include <cmath>
#include <print>
#include <vector>

inline void testEarCutConcepts()
{
    // FaceConcept checks on various proxy types and value categories
    static_assert(meshv2::FaceConcept<PolyMesh::Face>);
    static_assert(meshv2::FaceConcept<PolyMesh::Face&>);
    static_assert(meshv2::FaceConcept<const PolyMesh::Face&>);
    static_assert(meshv2::FaceConcept<PolyMesh::Face&&>);

    static_assert(meshv2::FaceConcept<PolyMesh::ConstFace>);
    static_assert(meshv2::FaceConcept<PolyMesh::ConstFace&>);
    static_assert(meshv2::FaceConcept<const PolyMesh::ConstFace&>);
    static_assert(meshv2::FaceConcept<PolyMesh::ConstFace&&>);

    static_assert(meshv2::FaceConcept<TriBitMesh::Face>);
    static_assert(meshv2::FaceConcept<TriBitMesh::ConstFace>);

    // Negative concept test: Vertex and Edge are not Faces
    static_assert(!meshv2::FaceConcept<PolyMesh::Vertex>);
    static_assert(!meshv2::FaceConcept<PolyMesh::ConstVertex>);
}

inline void testEarCutQuad()
{
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(1.0f, 1.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 1.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    // 1. Calling with mutable lvalue proxy
    auto f = poly.face(0);
    std::vector<vcl::uint> tris1 = meshv2::earCut(f);
    assert(tris1.size() == 6); // 2 triangles

    // 2. Calling with temporary rvalue proxy
    std::vector<vcl::uint> tris2 = meshv2::earCut(poly.face(0));
    assert(tris2 == tris1);

    // 3. Calling with const lvalue proxy
    const auto cf = poly.face(0);
    std::vector<vcl::uint> tris3 = meshv2::earCut(cf);
    assert(tris3 == tris1);

    // 4. Calling with ConstFace from a const mesh reference
    const auto& constPoly = poly;
    std::vector<vcl::uint> tris4 = meshv2::earCut(constPoly.face(0));
    assert(tris4 == tris1);

    // 5. Calling unqualified via ADL (since Face is in namespace vcl::meshv2)
    std::vector<vcl::uint> tris5 = earCut(poly.face(0));
    assert(tris5 == tris1);

    // Verify all generated triangle indices are in the valid range [0, 3]
    for (vcl::uint idx : tris1) {
        assert(idx < 4);
    }
}

inline void testEarCutPentagon()
{
    PolyMesh poly;
    poly.addVertices(5);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(1.0f, 3.0f, 0.0f);
    poly.vertex(4).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3, 4);

    std::vector<vcl::uint> tris = meshv2::earCut(poly.face(0));
    assert(tris.size() == 9); // 5-gon -> 3 triangles = 9 indices

    for (vcl::uint idx : tris) {
        assert(idx < 5);
    }
}

inline void testEarCutConcave()
{
    // L-shaped concave polygon with 6 vertices
    PolyMesh poly;
    poly.addVertices(6);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 1.0f, 0.0f);
    poly.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);
    poly.vertex(4).position() = Point3f(1.0f, 2.0f, 0.0f);
    poly.vertex(5).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3, 4, 5);

    std::vector<vcl::uint> tris = meshv2::earCut(poly.face(0));
    assert(tris.size() == 12); // 6-gon -> 4 triangles = 12 indices

    for (vcl::uint idx : tris) {
        assert(idx < 6);
    }
}

inline void testEarCutTriangle()
{
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    std::vector<vcl::uint> tris = meshv2::earCut(triMesh.face(0));
    assert(tris.size() == 3);
    assert(tris[0] == 1 && tris[1] == 2 && tris[2] == 0);
}

inline void testEarCut3DPlane()
{
    // Planar quad situated on the YZ plane (x = 5.0)
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(5.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(5.0f, 2.0f, 0.0f);
    poly.vertex(2).position() = Point3f(5.0f, 2.0f, 2.0f);
    poly.vertex(3).position() = Point3f(5.0f, 0.0f, 2.0f);
    poly.addFace(0, 1, 2, 3);

    std::vector<vcl::uint> tris = meshv2::earCut(poly.face(0));
    assert(tris.size() == 6); // 2 triangles
    for (vcl::uint idx : tris) {
        assert(idx < 4);
    }
}

inline void testPolygonGeometryTriangle()
{
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    triMesh.addFace(0, 1, 2);
    auto f = triMesh.face(0);

    // Normal: counter-clockwise (0,0,0) -> (1,0,0) -> (0,1,0) has cross-product (0,0,1)
    Point3f n = meshv2::faceNormal(f);
    assert(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    // Barycenter: (0+1+0)/3, (0+0+1)/3, 0
    Point3f bar = meshv2::faceBarycenter(f);
    assert(vcl::epsilonEquals(bar, Point3f(1.0f / 3.0f, 1.0f / 3.0f, 0.0f)));

    // Area: 1 * 1 / 2 = 0.5
    float a = meshv2::faceArea(f);
    assert(vcl::epsilonEquals(a, 0.5f));

    // Perimeter: 1 + 1 + sqrt(2)
    float p = meshv2::facePerimeter(f);
    float expectedP = 2.0f + std::sqrt(2.0f);
    assert(vcl::epsilonEquals(p, expectedP));

    // Internal angles
    float a0 = meshv2::faceAngleOnVertexRad(f, 0);
    assert(vcl::epsilonEquals(a0, float(M_PI / 2.0)));
    float a1 = meshv2::faceAngleOnVertexRad(f, 1);
    assert(vcl::epsilonEquals(a1, float(M_PI / 4.0)));
    float a2 = meshv2::faceAngleOnVertexRad(f, 2);
    assert(vcl::epsilonEquals(a2, float(M_PI / 4.0)));
}

inline void testPolygonGeometryPoly3()
{
    // Same triangle geometry as above, but on a dynamic-vertex-count PolyMesh (vertexCount() == 3)
    PolyMesh poly;
    poly.addVertices(3);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    poly.addFace(0, 1, 2);
    auto f = poly.face(0);

    Point3f n = meshv2::faceNormal(f);
    assert(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    Point3f bar = meshv2::faceBarycenter(f);
    assert(vcl::epsilonEquals(bar, Point3f(1.0f / 3.0f, 1.0f / 3.0f, 0.0f)));

    float a = meshv2::faceArea(f);
    assert(vcl::epsilonEquals(a, 0.5f));

    float p = meshv2::facePerimeter(f);
    float expectedP = 2.0f + std::sqrt(2.0f);
    assert(vcl::epsilonEquals(p, expectedP));

    float a0 = meshv2::faceAngleOnVertexRad(f, 0);
    assert(vcl::epsilonEquals(a0, float(M_PI / 2.0)));
    float a1 = meshv2::faceAngleOnVertexRad(f, 1);
    assert(vcl::epsilonEquals(a1, float(M_PI / 4.0)));
    float a2 = meshv2::faceAngleOnVertexRad(f, 2);
    assert(vcl::epsilonEquals(a2, float(M_PI / 4.0)));
}

inline void testPolygonGeometryQuad()
{
    // Square of side 3 on XY plane (normal = +Z)
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(3.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(3.0f, 3.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 3.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);
    auto f = poly.face(0);

    Point3f n = meshv2::faceNormal(f);
    assert(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    Point3f bar = meshv2::faceBarycenter(f);
    assert(vcl::epsilonEquals(bar, Point3f(1.5f, 1.5f, 0.0f)));

    float a = meshv2::faceArea(f);
    assert(vcl::epsilonEquals(a, 9.0f));

    float p = meshv2::facePerimeter(f);
    assert(vcl::epsilonEquals(p, 12.0f));

    for (uint i = 0; i < 4; ++i) {
        float angle = meshv2::faceAngleOnVertexRad(f, i);
        assert(vcl::epsilonEquals(angle, float(M_PI / 2.0)));
    }
}

inline void testPolygonGeometryConstAndADL()
{
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    // 1. Const proxy
    const auto cf = poly.face(0);
    assert(vcl::epsilonEquals(meshv2::faceArea(cf), 4.0f));
    assert(vcl::epsilonEquals(meshv2::facePerimeter(cf), 8.0f));

    // 2. Const mesh reference
    const auto& constPoly = poly;
    assert(vcl::epsilonEquals(meshv2::faceArea(constPoly.face(0)), 4.0f));

    // 3. Temporary rvalue proxy
    assert(vcl::epsilonEquals(meshv2::faceNormal(poly.face(0)), Point3f(0.0f, 0.0f, 1.0f)));

    // 4. ADL unqualified calls
    assert(vcl::epsilonEquals(faceArea(poly.face(0)), 4.0f));
    assert(vcl::epsilonEquals(facePerimeter(poly.face(0)), 8.0f));
    assert(vcl::epsilonEquals(faceBarycenter(poly.face(0)), Point3f(1.0f, 1.0f, 0.0f)));
    assert(vcl::epsilonEquals(faceNormal(poly.face(0)), Point3f(0.0f, 0.0f, 1.0f)));
    assert(vcl::epsilonEquals(faceAngleOnVertexRad(poly.face(0), 0), float(M_PI / 2.0)));
}

inline void testBoundingBox()
{
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 3.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 3.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    // 1. Vertex bounding box
    auto vBox = meshv2::boundingBox(poly.vertex(1));
    assert(vBox.min() == Point3f(2.0f, 0.0f, 0.0f));
    assert(vBox.max() == Point3f(2.0f, 0.0f, 0.0f));

    // 2. Face bounding box
    auto fBox = meshv2::boundingBox(poly.face(0));
    assert(fBox.min() == Point3f(0.0f, 0.0f, 0.0f));
    assert(fBox.max() == Point3f(2.0f, 3.0f, 0.0f));

    // 3. Edge bounding box
    EdgeMesh edgeMesh;
    edgeMesh.addVertices(2);
    edgeMesh.vertex(0).position() = Point3f(-1.0f, 5.0f, 2.0f);
    edgeMesh.vertex(1).position() = Point3f(4.0f, -2.0f, 1.0f);
    edgeMesh.addEdge(0, 1);

    auto eBox = meshv2::boundingBox(edgeMesh.edge(0));
    assert(eBox.min() == Point3f(-1.0f, -2.0f, 1.0f));
    assert(eBox.max() == Point3f(4.0f, 5.0f, 2.0f));

    // 4. Const proxy and const mesh
    const auto cf = poly.face(0);
    assert(meshv2::boundingBox(cf).min() == Point3f(0.0f, 0.0f, 0.0f));
    const auto& cpoly = poly;
    assert(meshv2::boundingBox(cpoly.face(0)).max() == Point3f(2.0f, 3.0f, 0.0f));

    // 5. ADL calls
    assert(boundingBox(poly.face(0)).min() == Point3f(0.0f, 0.0f, 0.0f));
    assert(boundingBox(edgeMesh.edge(0)).max() == Point3f(4.0f, 5.0f, 2.0f));
}

inline void testDistanceVertexPoint()
{
    PolyMesh poly;
    poly.addVertex(Point3f(1.0f, 2.0f, 3.0f));
    Point3f p(4.0f, 6.0f, 3.0f); // dx=3, dy=4 -> dist=5

    // 1. mutable proxy
    assert(vcl::epsilonEquals(meshv2::distance(poly.vertex(0), p), 5.0f));
    assert(vcl::epsilonEquals(meshv2::distance(p, poly.vertex(0)), 5.0f));

    // 2. const proxy
    const auto cv = poly.vertex(0);
    assert(vcl::epsilonEquals(meshv2::distance(cv, p), 5.0f));
    assert(vcl::epsilonEquals(meshv2::distance(p, cv), 5.0f));

    // 3. const mesh ref
    const auto& cpoly = poly;
    assert(vcl::epsilonEquals(meshv2::distance(cpoly.vertex(0), p), 5.0f));

    // 4. ADL unqualified
    assert(vcl::epsilonEquals(distance(poly.vertex(0), p), 5.0f));
    assert(vcl::epsilonEquals(distance(p, poly.vertex(0)), 5.0f));
}

inline void testDistanceVertexVertex()
{
    PolyMesh poly;
    poly.addVertices(2);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(3.0f, 4.0f, 0.0f);

    assert(vcl::epsilonEquals(meshv2::distance(poly.vertex(0), poly.vertex(1)), 5.0f));
    assert(vcl::epsilonEquals(meshv2::distance(poly.vertex(1), poly.vertex(0)), 5.0f));

    const auto cv0 = poly.vertex(0);
    const auto cv1 = poly.vertex(1);
    assert(vcl::epsilonEquals(meshv2::distance(cv0, cv1), 5.0f));

    // ADL
    assert(vcl::epsilonEquals(distance(poly.vertex(0), poly.vertex(1)), 5.0f));
}

inline void testDistanceTriangleFace()
{
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    auto f = triMesh.face(0);
    Point3f pAbove(0.5f, 0.5f, 3.0f);

    // 1. Basic distance
    assert(vcl::epsilonEquals(meshv2::distance(pAbove, f), 3.0f));
    assert(vcl::epsilonEquals(meshv2::distance(f, pAbove), 3.0f));

    // 2. Closest point
    Point3f closest;
    float d = meshv2::distance(pAbove, f, closest);
    assert(vcl::epsilonEquals(d, 3.0f));
    assert(vcl::epsilonEquals(closest, Point3f(0.5f, 0.5f, 0.0f)));

    // Symmetrical distance with closest point
    Point3f closestSym;
    float dSym = meshv2::distance(f, pAbove, closestSym);
    assert(vcl::epsilonEquals(dSym, 3.0f));
    assert(vcl::epsilonEquals(closestSym, Point3f(0.5f, 0.5f, 0.0f)));

    // 3. Signed distance
    Point3f pBelow(0.5f, 0.5f, -3.0f);
    float dSigned = meshv2::distance(pBelow, f, closest, true);
    assert(vcl::epsilonEquals(dSigned, -3.0f));
    assert(vcl::epsilonEquals(closest, Point3f(0.5f, 0.5f, 0.0f)));

    // 4. Point outside triangle
    Point3f pOutside(-1.0f, 0.0f, 0.0f);
    assert(vcl::epsilonEquals(meshv2::distance(pOutside, f, closest), 1.0f));
    assert(vcl::epsilonEquals(closest, Point3f(0.0f, 0.0f, 0.0f)));

    // 5. boundedDistance
    assert(meshv2::boundedDistance(pAbove, f, 1.0f) >= 1.0f);
    assert(vcl::epsilonEquals(meshv2::boundedDistance(pAbove, f, 5.0f, closest), 3.0f));
    assert(vcl::epsilonEquals(meshv2::boundedDistance(f, pAbove, 5.0f, closest), 3.0f));

    // 6. Const proxy & ADL
    const auto cf = triMesh.face(0);
    assert(vcl::epsilonEquals(distance(pAbove, cf), 3.0f));
    assert(vcl::epsilonEquals(distance(cf, pAbove), 3.0f));
}

inline void testDistancePolygonFace()
{
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    auto f = poly.face(0);
    Point3f p(1.0f, 1.0f, 4.0f);

    // 1. Point above planar polygon face
    assert(vcl::epsilonEquals(meshv2::distance(p, f), 4.0f));
    assert(vcl::epsilonEquals(meshv2::distance(f, p), 4.0f));

    Point3f closest;
    float d = meshv2::distance(p, f, closest);
    assert(vcl::epsilonEquals(d, 4.0f));
    assert(vcl::epsilonEquals(closest, Point3f(1.0f, 1.0f, 0.0f)));

    // 2. Point outside polygon face
    Point3f pOutside(3.0f, 1.0f, 0.0f);
    float dOut = meshv2::distance(pOutside, f, closest);
    assert(vcl::epsilonEquals(dOut, 1.0f));
    assert(vcl::epsilonEquals(closest, Point3f(2.0f, 1.0f, 0.0f)));

    // 3. Const mesh and ADL
    const auto& cpoly = poly;
    assert(vcl::epsilonEquals(distance(p, cpoly.face(0)), 4.0f));
    assert(vcl::epsilonEquals(distance(cpoly.face(0), p), 4.0f));
}

inline void testDistanceVertexFace()
{
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    PolyMesh queryMesh;
    queryMesh.addVertex(Point3f(1.0f, 1.0f, 2.5f));

    auto v = queryMesh.vertex(0);
    auto f = poly.face(0);

    assert(vcl::epsilonEquals(meshv2::distance(v, f), 2.5f));
    assert(vcl::epsilonEquals(meshv2::distance(f, v), 2.5f));

    const auto cv = queryMesh.vertex(0);
    const auto cf = poly.face(0);
    assert(vcl::epsilonEquals(meshv2::distance(cv, cf), 2.5f));
    assert(vcl::epsilonEquals(meshv2::distance(cf, cv), 2.5f));

    // ADL
    assert(vcl::epsilonEquals(distance(v, f), 2.5f));
    assert(vcl::epsilonEquals(distance(f, v), 2.5f));
}

inline void testElemAlgorithms()
{
    std::println("--- Testing Elem Algorithms ---");
    testEarCutConcepts();
    testEarCutQuad();
    testEarCutPentagon();
    testEarCutConcave();
    testEarCutTriangle();
    testEarCut3DPlane();

    testPolygonGeometryTriangle();
    testPolygonGeometryPoly3();
    testPolygonGeometryQuad();
    testPolygonGeometryConstAndADL();

    testBoundingBox();

    testDistanceVertexPoint();
    testDistanceVertexVertex();
    testDistanceTriangleFace();
    testDistancePolygonFace();
    testDistanceVertexFace();

    std::println("Elem Algorithms verified successfully!");
}

#endif // TEST_ELEM_ALGORITHMS_H

