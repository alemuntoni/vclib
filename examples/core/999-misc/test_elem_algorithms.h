// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ELEM_ALGORITHMS_H
#define TEST_ELEM_ALGORITHMS_H

#include "types.h"

#include <vclib/mesh/elem_algorithms.h>

#include <cassert>
#include <cmath>
#include <print>
#include <vector>

inline void testEarCutConcepts()
{
    // FaceConcept checks on various proxy types and value categories
    static_assert(FaceConcept<PolyMesh::Face>);
    static_assert(FaceConcept<PolyMesh::Face&>);
    static_assert(FaceConcept<const PolyMesh::Face&>);
    static_assert(FaceConcept<PolyMesh::Face&&>);

    static_assert(FaceConcept<PolyMesh::ConstFace>);
    static_assert(FaceConcept<PolyMesh::ConstFace&>);
    static_assert(FaceConcept<const PolyMesh::ConstFace&>);
    static_assert(FaceConcept<PolyMesh::ConstFace&&>);

    static_assert(FaceConcept<TriBitMesh::Face>);
    static_assert(FaceConcept<TriBitMesh::ConstFace>);

    // Negative concept test: Vertex and Edge are not Faces
    static_assert(!FaceConcept<PolyMesh::Vertex>);
    static_assert(!FaceConcept<PolyMesh::ConstVertex>);
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
    std::vector<vcl::uint> tris1 = earCut(f);
    assert(tris1.size() == 6); // 2 triangles

    // 2. Calling with temporary rvalue proxy
    std::vector<vcl::uint> tris2 = earCut(poly.face(0));
    assert(tris2 == tris1);

    // 3. Calling with const lvalue proxy
    const auto cf = poly.face(0);
    std::vector<vcl::uint> tris3 = earCut(cf);
    assert(tris3 == tris1);

    // 4. Calling with ConstFace from a const mesh reference
    const auto& constPoly = poly;
    std::vector<vcl::uint> tris4 = earCut(constPoly.face(0));
    assert(tris4 == tris1);

    // 5. Calling unqualified via ADL (since Face is in namespace vcl)
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

    std::vector<vcl::uint> tris = earCut(poly.face(0));
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

    std::vector<vcl::uint> tris = earCut(poly.face(0));
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

    std::vector<vcl::uint> tris = earCut(triMesh.face(0));
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

    std::vector<vcl::uint> tris = earCut(poly.face(0));
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
    Point3f n = faceNormal(f);
    assert(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    // Barycenter: (0+1+0)/3, (0+0+1)/3, 0
    Point3f bar = faceBarycenter(f);
    assert(vcl::epsilonEquals(bar, Point3f(1.0f / 3.0f, 1.0f / 3.0f, 0.0f)));

    // Area: 1 * 1 / 2 = 0.5
    float a = faceArea(f);
    assert(vcl::epsilonEquals(a, 0.5f));

    // Perimeter: 1 + 1 + sqrt(2)
    float p = facePerimeter(f);
    float expectedP = 2.0f + std::sqrt(2.0f);
    assert(vcl::epsilonEquals(p, expectedP));

    // Internal angles
    float a0 = faceAngleOnVertexRad(f, 0);
    assert(vcl::epsilonEquals(a0, float(M_PI / 2.0)));
    float a1 = faceAngleOnVertexRad(f, 1);
    assert(vcl::epsilonEquals(a1, float(M_PI / 4.0)));
    float a2 = faceAngleOnVertexRad(f, 2);
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

    Point3f n = faceNormal(f);
    assert(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    Point3f bar = faceBarycenter(f);
    assert(vcl::epsilonEquals(bar, Point3f(1.0f / 3.0f, 1.0f / 3.0f, 0.0f)));

    float a = faceArea(f);
    assert(vcl::epsilonEquals(a, 0.5f));

    float p = facePerimeter(f);
    float expectedP = 2.0f + std::sqrt(2.0f);
    assert(vcl::epsilonEquals(p, expectedP));

    float a0 = faceAngleOnVertexRad(f, 0);
    assert(vcl::epsilonEquals(a0, float(M_PI / 2.0)));
    float a1 = faceAngleOnVertexRad(f, 1);
    assert(vcl::epsilonEquals(a1, float(M_PI / 4.0)));
    float a2 = faceAngleOnVertexRad(f, 2);
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

    Point3f n = faceNormal(f);
    assert(vcl::epsilonEquals(n, Point3f(0.0f, 0.0f, 1.0f)));

    Point3f bar = faceBarycenter(f);
    assert(vcl::epsilonEquals(bar, Point3f(1.5f, 1.5f, 0.0f)));

    float a = faceArea(f);
    assert(vcl::epsilonEquals(a, 9.0f));

    float p = facePerimeter(f);
    assert(vcl::epsilonEquals(p, 12.0f));

    for (uint i = 0; i < 4; ++i) {
        float angle = faceAngleOnVertexRad(f, i);
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
    assert(vcl::epsilonEquals(faceArea(cf), 4.0f));
    assert(vcl::epsilonEquals(facePerimeter(cf), 8.0f));

    // 2. Const mesh reference
    const auto& constPoly = poly;
    assert(vcl::epsilonEquals(faceArea(constPoly.face(0)), 4.0f));

    // 3. Temporary rvalue proxy
    assert(vcl::epsilonEquals(faceNormal(poly.face(0)), Point3f(0.0f, 0.0f, 1.0f)));

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
    auto vBox = boundingBox(poly.vertex(1));
    assert(vBox.min() == Point3f(2.0f, 0.0f, 0.0f));
    assert(vBox.max() == Point3f(2.0f, 0.0f, 0.0f));

    // 2. Face bounding box
    auto fBox = boundingBox(poly.face(0));
    assert(fBox.min() == Point3f(0.0f, 0.0f, 0.0f));
    assert(fBox.max() == Point3f(2.0f, 3.0f, 0.0f));

    // 3. Edge bounding box
    EdgeMesh edgeMesh;
    edgeMesh.addVertices(2);
    edgeMesh.vertex(0).position() = Point3f(-1.0f, 5.0f, 2.0f);
    edgeMesh.vertex(1).position() = Point3f(4.0f, -2.0f, 1.0f);
    edgeMesh.addEdge(0, 1);

    auto eBox = boundingBox(edgeMesh.edge(0));
    assert(eBox.min() == Point3f(-1.0f, -2.0f, 1.0f));
    assert(eBox.max() == Point3f(4.0f, 5.0f, 2.0f));

    // 4. Const proxy and const mesh
    const auto cf = poly.face(0);
    assert(boundingBox(cf).min() == Point3f(0.0f, 0.0f, 0.0f));
    const auto& cpoly = poly;
    assert(boundingBox(cpoly.face(0)).max() == Point3f(2.0f, 3.0f, 0.0f));

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
    assert(vcl::epsilonEquals(distance(poly.vertex(0), p), 5.0f));
    assert(vcl::epsilonEquals(distance(p, poly.vertex(0)), 5.0f));

    // 2. const proxy
    const auto cv = poly.vertex(0);
    assert(vcl::epsilonEquals(distance(cv, p), 5.0f));
    assert(vcl::epsilonEquals(distance(p, cv), 5.0f));

    // 3. const mesh ref
    const auto& cpoly = poly;
    assert(vcl::epsilonEquals(distance(cpoly.vertex(0), p), 5.0f));

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

    assert(vcl::epsilonEquals(distance(poly.vertex(0), poly.vertex(1)), 5.0f));
    assert(vcl::epsilonEquals(distance(poly.vertex(1), poly.vertex(0)), 5.0f));

    const auto cv0 = poly.vertex(0);
    const auto cv1 = poly.vertex(1);
    assert(vcl::epsilonEquals(distance(cv0, cv1), 5.0f));

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
    assert(vcl::epsilonEquals(distance(pAbove, f), 3.0f));
    assert(vcl::epsilonEquals(distance(f, pAbove), 3.0f));

    // 2. Closest point
    Point3f closest;
    float d = distance(pAbove, f, closest);
    assert(vcl::epsilonEquals(d, 3.0f));
    assert(vcl::epsilonEquals(closest, Point3f(0.5f, 0.5f, 0.0f)));

    // Symmetrical distance with closest point
    Point3f closestSym;
    float dSym = distance(f, pAbove, closestSym);
    assert(vcl::epsilonEquals(dSym, 3.0f));
    assert(vcl::epsilonEquals(closestSym, Point3f(0.5f, 0.5f, 0.0f)));

    // 3. Signed distance
    Point3f pBelow(0.5f, 0.5f, -3.0f);
    float dSigned = distance(pBelow, f, closest, true);
    assert(vcl::epsilonEquals(dSigned, -3.0f));
    assert(vcl::epsilonEquals(closest, Point3f(0.5f, 0.5f, 0.0f)));

    // 4. Point outside triangle
    Point3f pOutside(-1.0f, 0.0f, 0.0f);
    assert(vcl::epsilonEquals(distance(pOutside, f, closest), 1.0f));
    assert(vcl::epsilonEquals(closest, Point3f(0.0f, 0.0f, 0.0f)));

    // 5. boundedDistance
    assert(boundedDistance(pAbove, f, 1.0f) >= 1.0f);
    assert(vcl::epsilonEquals(boundedDistance(pAbove, f, 5.0f, closest), 3.0f));
    assert(vcl::epsilonEquals(boundedDistance(f, pAbove, 5.0f, closest), 3.0f));

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
    assert(vcl::epsilonEquals(distance(p, f), 4.0f));
    assert(vcl::epsilonEquals(distance(f, p), 4.0f));

    Point3f closest;
    float d = distance(p, f, closest);
    assert(vcl::epsilonEquals(d, 4.0f));
    assert(vcl::epsilonEquals(closest, Point3f(1.0f, 1.0f, 0.0f)));

    // 2. Point outside polygon face
    Point3f pOutside(3.0f, 1.0f, 0.0f);
    float dOut = distance(pOutside, f, closest);
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

    assert(vcl::epsilonEquals(distance(v, f), 2.5f));
    assert(vcl::epsilonEquals(distance(f, v), 2.5f));

    const auto cv = queryMesh.vertex(0);
    const auto cf = poly.face(0);
    assert(vcl::epsilonEquals(distance(cv, cf), 2.5f));
    assert(vcl::epsilonEquals(distance(cf, cv), 2.5f));

    // ADL
    assert(vcl::epsilonEquals(distance(v, f), 2.5f));
    assert(vcl::epsilonEquals(distance(f, v), 2.5f));
}

inline void testIntersectionFaceBox()
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Box3f boxHit(Point3f(-0.5f, -0.5f, -0.5f), Point3f(1.5f, 1.5f, 0.5f));
    Box3f boxMiss(Point3f(10.0f, 10.0f, 10.0f), Point3f(12.0f, 12.0f, 12.0f));

    assert(intersect(triMesh.face(0), boxHit));
    assert(intersect(boxHit, triMesh.face(0))); // symmetry

    assert(!intersect(triMesh.face(0), boxMiss));
    assert(!intersect(boxMiss, triMesh.face(0)));

    // 2. Polygonal Quad Face
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    Box3f quadBoxHit(Point3f(1.0f, 1.0f, -1.0f), Point3f(3.0f, 3.0f, 1.0f));
    Box3f quadBoxMiss(Point3f(5.0f, 5.0f, 0.0f), Point3f(6.0f, 6.0f, 1.0f));

    assert(intersect(poly.face(0), quadBoxHit));
    assert(intersect(quadBoxHit, poly.face(0)));

    assert(!intersect(poly.face(0), quadBoxMiss));
    assert(!intersect(quadBoxMiss, poly.face(0)));

    // 3. 3-vertex polygon face
    PolyMesh poly3;
    poly3.addVertices(3);
    poly3.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly3.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly3.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly3.addFace(0, 1, 2);

    assert(intersect(poly3.face(0), boxHit));
    assert(!intersect(poly3.face(0), boxMiss));

    // 4. Const proxy and ADL
    const auto cf = poly.face(0);
    assert(intersect(cf, quadBoxHit));
    assert(intersect(quadBoxHit, cf));
}

inline void testIntersectionFaceSphere()
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Sphere<float> sphereHit(Point3f(0.5f, 0.5f, 0.5f), 1.0f);
    Sphere<float> sphereMiss(Point3f(0.5f, 0.5f, 5.0f), 1.0f);

    assert(intersect(triMesh.face(0), sphereHit));
    assert(intersect(sphereHit, triMesh.face(0)));

    assert(!intersect(triMesh.face(0), sphereMiss));
    assert(!intersect(sphereMiss, triMesh.face(0)));

    // Witness and res
    Point3f witness;
    std::pair<float, float> res;
    assert(intersect(triMesh.face(0), sphereHit, witness, res));
    assert(vcl::epsilonEquals(witness, Point3f(0.5f, 0.5f, 0.0f)));

    Point3f witnessSym;
    std::pair<float, float> resSym;
    assert(intersect(sphereHit, triMesh.face(0), witnessSym, resSym));
    assert(vcl::epsilonEquals(witnessSym, Point3f(0.5f, 0.5f, 0.0f)));

    // 2. Polygonal Quad Face
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    Sphere<float> quadSphereHit(Point3f(1.0f, 1.0f, 0.5f), 1.0f);
    assert(intersect(poly.face(0), quadSphereHit));
    assert(intersect(quadSphereHit, poly.face(0)));

    assert(!intersect(poly.face(0), sphereMiss));

    // 3. Const proxy and ADL
    const auto cf = poly.face(0);
    assert(intersect(cf, quadSphereHit));
    assert(intersect(quadSphereHit, cf));
}

inline void testIntersectionFaceRay()
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Ray3f rayHit(Point3f(0.5f, 0.5f, 5.0f), Point3f(0.0f, 0.0f, -1.0f));
    Ray3f rayAway(Point3f(0.5f, 0.5f, 5.0f), Point3f(0.0f, 0.0f, 1.0f));
    Ray3f rayMiss(Point3f(5.0f, 5.0f, 5.0f), Point3f(0.0f, 0.0f, -1.0f));

    float t = 0.0f;
    auto pt = intersection(rayHit, triMesh.face(0), std::ref(t));
    assert(pt.has_value());
    assert(vcl::epsilonEquals(*pt, Point3f(0.5f, 0.5f, 0.0f)));
    assert(vcl::epsilonEquals(t, 5.0f));

    // Symmetrical call
    float tSym = 0.0f;
    auto ptSym = intersection(triMesh.face(0), rayHit, std::ref(tSym));
    assert(ptSym.has_value());
    assert(vcl::epsilonEquals(*ptSym, Point3f(0.5f, 0.5f, 0.0f)));
    assert(vcl::epsilonEquals(tSym, 5.0f));

    assert(intersect(rayHit, triMesh.face(0)));
    assert(intersect(triMesh.face(0), rayHit));

    assert(!intersection(rayAway, triMesh.face(0)).has_value());
    assert(!intersect(rayAway, triMesh.face(0)));

    assert(!intersection(rayMiss, triMesh.face(0)).has_value());
    assert(!intersect(rayMiss, triMesh.face(0)));

    // 2. Polygonal Quad Face
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    Ray3f rayQuad(Point3f(1.0f, 1.5f, 3.0f), Point3f(0.0f, 0.0f, -1.0f));
    float tQuad = 0.0f;
    auto ptQuad = intersection(rayQuad, poly.face(0), std::ref(tQuad));
    assert(ptQuad.has_value());
    assert(vcl::epsilonEquals(*ptQuad, Point3f(1.0f, 1.5f, 0.0f)));
    assert(vcl::epsilonEquals(tQuad, 3.0f));

    assert(intersect(rayQuad, poly.face(0)));
    assert(intersect(poly.face(0), rayQuad));

    assert(!intersect(rayMiss, poly.face(0)));

    // 3. Const proxy and ADL
    const auto cf = poly.face(0);
    assert(intersection(rayQuad, cf).has_value());
    assert(intersection(cf, rayQuad).has_value());
    assert(intersect(rayQuad, cf));
    assert(intersect(cf, rayQuad));
}

inline void testVisibility()
{
    // 1. Triangle Face
    TriBitMesh triMesh;
    triMesh.addVertices(3);
    triMesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    triMesh.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    triMesh.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    triMesh.addFace(0, 1, 2);

    Point3f pAbove(0.5f, 0.5f, 2.0f);
    Point3f pBelow(0.5f, 0.5f, -2.0f);
    Point3f pOnPlane(0.5f, 0.5f, 0.0f);

    assert(halfSpaceDeterminant(triMesh.face(0), pAbove) > 0.0f);
    assert(halfSpaceDeterminant(triMesh.face(0), pBelow) < 0.0f);
    assert(vcl::epsilonEquals(halfSpaceDeterminant(triMesh.face(0), pOnPlane), 0.0f));

    assert(facePointVisibility(triMesh.face(0), pAbove));
    assert(!facePointVisibility(triMesh.face(0), pBelow));
    assert(!facePointVisibility(triMesh.face(0), pOnPlane));

    // Symmetrical facePointVisibility
    assert(facePointVisibility(pAbove, triMesh.face(0)));
    assert(!facePointVisibility(pBelow, triMesh.face(0)));

    // 2. 3-vertex polygon face
    PolyMesh poly3;
    poly3.addVertices(3);
    poly3.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly3.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly3.vertex(2).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly3.addFace(0, 1, 2);

    assert(halfSpaceDeterminant(poly3.face(0), pAbove) > 0.0f);
    assert(halfSpaceDeterminant(poly3.face(0), pBelow) < 0.0f);
    assert(facePointVisibility(poly3.face(0), pAbove));
    assert(!facePointVisibility(poly3.face(0), pBelow));

    // 3. Polygonal Quad Face
    PolyMesh poly;
    poly.addVertices(4);
    poly.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    poly.vertex(1).position() = Point3f(2.0f, 0.0f, 0.0f);
    poly.vertex(2).position() = Point3f(2.0f, 2.0f, 0.0f);
    poly.vertex(3).position() = Point3f(0.0f, 2.0f, 0.0f);
    poly.addFace(0, 1, 2, 3);

    assert(halfSpaceDeterminant(poly.face(0), pAbove) > 0.0f);
    assert(halfSpaceDeterminant(poly.face(0), pBelow) < 0.0f);
    assert(facePointVisibility(poly.face(0), pAbove));
    assert(!facePointVisibility(poly.face(0), pBelow));

    // 4. Const proxy and ADL
    const auto cf = poly.face(0);
    assert(halfSpaceDeterminant(cf, pAbove) > 0.0f);
    assert(facePointVisibility(cf, pAbove));
    assert(facePointVisibility(pAbove, cf));
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

    testIntersectionFaceBox();
    testIntersectionFaceSphere();
    testIntersectionFaceRay();

    testVisibility();

    std::println("Elem Algorithms verified successfully!");
}

#endif // TEST_ELEM_ALGORITHMS_H

