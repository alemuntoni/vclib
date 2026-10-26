// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_ELEM_ALGORITHMS_H
#define TEST_ELEM_ALGORITHMS_H

#include "types.h"

#include <vclib/mesh_v2/elem_algorithms/polygon/ear_cut.h>

#include <cassert>
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

inline void testElemAlgorithms()
{
    std::println("--- Testing Elem Algorithms (earCut) ---");
    testEarCutConcepts();
    testEarCutQuad();
    testEarCutPentagon();
    testEarCutConcave();
    testEarCutTriangle();
    testEarCut3DPlane();
    std::println("Elem Algorithms (earCut) verified successfully!");
}

#endif // TEST_ELEM_ALGORITHMS_H

