// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_WEDGE_H
#define TEST_WEDGE_H

#include "types.h"
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/traits.h>

#include <vclib/mesh/elements/face_components.h>

// Wedge mesh types for tests:
using WedgeFaceComponents = FaceComponents<
    face::BitFlags,
    face::TriangleVertexReferences<vcl::uint>,
    face::TriangleWedgeColors,
    face::TriangleWedgeTexCoordsf,
    face::OptionalTriangleWedgeNormals3f>;

using WedgeMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<WedgeFaceComponents>>;

using PolyWedgeFaceComponents = FaceComponents<
    face::PolygonBitFlags,
    face::PolygonVertexReferences<vcl::uint>,
    face::PolygonWedgeColors,
    face::PolygonWedgeTexCoordsf>;

using PolyWedgeMesh = Mesh<
    mesh::VertexContainer<SimpleVertexComponents>,
    mesh::FaceContainer<PolyWedgeFaceComponents>>;

inline void testWedgeConcepts()
{
    // Wedge component concepts on Face
    static_assert(face::HasWedgeColors<WedgeMesh::Face>);
    static_assert(!face::HasOptionalWedgeColors<WedgeMesh::Face>);
    static_assert(face::HasWedgeTexCoords<WedgeMesh::Face>);
    static_assert(!face::HasOptionalWedgeTexCoords<WedgeMesh::Face>);
    static_assert(face::HasWedgeNormals<WedgeMesh::Face>);
    static_assert(face::HasOptionalWedgeNormals<WedgeMesh::Face>);
    static_assert(face::HasWedges<WedgeMesh::Face>);

    // Unified element_concepts on Face
    static_assert(HasWedgeColors<WedgeMesh::Face>);
    static_assert(HasWedgeTexCoords<WedgeMesh::Face>);
    static_assert(HasWedgeNormals<WedgeMesh::Face>);
    static_assert(HasWedges<WedgeMesh::Face>);

    // Mesh-level wedge concepts
    static_assert(HasFaceWedgeColors<WedgeMesh>);
    static_assert(HasFaceWedgeTexCoords<WedgeMesh>);
    static_assert(HasFaceWedgeNormals<WedgeMesh>);
    static_assert(HasOptionalFaceWedgeNormals<WedgeMesh>);
    static_assert(HasFaceWedges<WedgeMesh>);

    // Negative concepts on mesh without wedges
    static_assert(!face::HasWedgeColors<SimpleMesh::Face>);
    static_assert(!face::HasWedgeTexCoords<SimpleMesh::Face>);
    static_assert(!face::HasWedgeNormals<SimpleMesh::Face>);
    static_assert(!face::HasWedges<SimpleMesh::Face>);
    static_assert(!HasWedges<SimpleMesh::Face>);
    static_assert(!HasFaceWedges<SimpleMesh>);

    // Polygonal wedge mesh concepts
    static_assert(face::HasWedgeColors<PolyWedgeMesh::Face>);
    static_assert(face::HasWedgeTexCoords<PolyWedgeMesh::Face>);
    static_assert(!face::HasWedgeNormals<PolyWedgeMesh::Face>);
    static_assert(HasFaceWedges<PolyWedgeMesh>);
    static_assert(!HasFaceWedgeNormals<PolyWedgeMesh>);

    // ComponentConcept check
    static_assert(comp::ComponentConcept<face::TriangleWedgeColors>);
    static_assert(comp::ComponentConcept<face::TriangleWedgeTexCoordsf>);
    static_assert(comp::ComponentConcept<face::OptionalTriangleWedgeNormals3f>);
    static_assert(comp::IsTiedToVertexCount<face::PolygonWedgeColors>);
    static_assert(!comp::IsTiedToVertexCount<face::TriangleWedgeColors>);

    // Wedge traits
    static_assert(WedgeConcept<WedgeType<WedgeMesh::Face>>);
    static_assert(std::is_same_v<WedgeType<WedgeMesh::Face>, comp::WedgeProxy<WedgeMesh::Face>>);
    static_assert(std::is_same_v<VertexType<WedgeType<WedgeMesh::Face>>, WedgeMesh::Vertex>);
    static_assert(std::is_same_v<FaceType<WedgeType<WedgeMesh::Face>>, WedgeMesh::Face>);
    static_assert(std::is_same_v<NormalType<WedgeType<WedgeMesh::Face>>, Point3f>);
    static_assert(std::is_same_v<TexCoordType<WedgeType<WedgeMesh::Face>>, vcl::TexCoordf>);
}

inline void testTriangleWedges()
{
    WedgeMesh m;
    m.addVertices(3);
    vcl::uint fi = m.addFace(0, 1, 2);
    auto f = m.face(fi);

    // 1. Access via WedgeProxy: f.wedge(i)
    f.wedge(0).color() = vcl::Color::Red;
    f.wedge(1).color() = vcl::Color::Green;
    f.wedge(2).color() = vcl::Color::Blue;

    assert(f.wedge(0).color() == vcl::Color::Red);
    assert(f.wedge(1).color() == vcl::Color::Green);
    assert(f.wedge(2).color() == vcl::Color::Blue);
    assert(f.wedge(0).index() == 0);
    assert(f.wedge(1).index() == 1);
    assert(f.wedge(2).index() == 2);
    assert(f.wedge(0).vertexIndex() == 0);
    assert(f.wedge(1).vertexIndex() == 1);
    assert(f.wedge(2).vertexIndex() == 2);

    // Wedge texcoords via proxy
    f.wedge(0).texCoord() = vcl::TexCoordf(0.0f, 0.0f);
    f.wedge(1).texCoord() = vcl::TexCoordf(1.0f, 0.0f);
    f.wedge(2).texCoord() = vcl::TexCoordf(0.5f, 1.0f);

    assert(f.wedge(0).texCoord().u() == 0.0f && f.wedge(0).texCoord().v() == 0.0f);
    assert(f.wedge(1).texCoord().u() == 1.0f && f.wedge(1).texCoord().v() == 0.0f);
    assert(f.wedge(2).texCoord().u() == 0.5f && f.wedge(2).texCoord().v() == 1.0f);

    // Optional WedgeNormals
    assert(!f.wedge(0).isNormalAvailable());
    assert(!f.wedges().isNormalsAvailable());

    m.enableElementComponent<ElemId::FACE, CompId::WEDGE_NORMALS>();
    assert(f.wedge(0).isNormalAvailable());
    assert(f.wedges().isNormalsAvailable());

    f.wedge(0).normal() = Point3f(0.0f, 0.0f, 1.0f);
    f.wedge(1).normal() = Point3f(0.0f, 1.0f, 0.0f);
    f.wedge(2).normal() = Point3f(1.0f, 0.0f, 0.0f);

    assert(f.wedge(0).normal() == Point3f(0.0f, 0.0f, 1.0f));
    assert(f.wedge(1).normal() == Point3f(0.0f, 1.0f, 0.0f));
    assert(f.wedge(2).normal() == Point3f(1.0f, 0.0f, 0.0f));

    // 2. Access via WedgeContainerProxy: f.wedges()
    assert(f.wedges().size() == 3);
    assert(f.wedges().colors().size() == 3);
    assert(f.wedges().colors()[0] == vcl::Color::Red);
    assert(f.wedges().colors()[1] == vcl::Color::Green);
    assert(f.wedges().colors()[2] == vcl::Color::Blue);
    assert(f.wedges().normals()[0] == Point3f(0.0f, 0.0f, 1.0f));
    assert(f.wedges().texCoords()[1].u() == 1.0f);
    assert(f.wedges()[0].color() == vcl::Color::Red);

    // Range-for over WedgeProxy: for (auto w : f.wedges())
    uint count = 0;
    for (auto w : f.wedges()) {
        assert(w.index() == count);
        assert(w.vertexIndex() == count);
        ++count;
    }
    assert(count == 3);

    // Range-for over direct column vectors: for (auto& n : f.wedges().normals())
    for (auto& n : f.wedges().normals()) {
        assert(n.norm() > 0.0f);
    }

    // Const-correctness
    const auto cf = f;
    assert(cf.wedge(0).color() == vcl::Color::Red);
    assert(cf.wedges().colors().size() == 3);
    for (const auto& c : cf.wedges().colors()) {
        assert(c.red() > 0 || c.green() > 0 || c.blue() > 0);
    }

    // Test importFrom between faces
    WedgeMesh m2;
    m2.enableElementComponent<ElemId::FACE, CompId::WEDGE_NORMALS>();
    m2.addVertices(3);
    vcl::uint fi2 = m2.addFace(0, 1, 2);
    auto f2 = m2.face(fi2);

    f2.importFrom(f);
    assert(f2.wedge(0).color() == vcl::Color::Red);
    assert(f2.wedge(1).color() == vcl::Color::Green);
    assert(f2.wedge(2).color() == vcl::Color::Blue);
    assert(f2.wedge(1).texCoord().u() == 1.0f);
    assert(f2.wedge(0).normal() == Point3f(0.0f, 0.0f, 1.0f));
}

inline void testPolygonWedges()
{
    PolyWedgeMesh pm;
    pm.addVertices(6);
    vcl::uint pfi = pm.addFace(0, 1, 2);
    auto pf = pm.face(pfi);

    // Initial 3 vertices
    assert(pf.vertexCount() == 3);
    assert(pf.wedges().size() == 3);
    assert(pf.wedges().colors().size() == 3);
    assert(pf.wedges().texCoords().size() == 3);

    pf.wedge(0).color() = vcl::Color::Red;
    pf.wedge(1).color() = vcl::Color::Green;
    pf.wedge(2).color() = vcl::Color::Blue;

    pf.wedge(0).texCoord() = vcl::TexCoordf(0.0f, 0.0f);
    pf.wedge(1).texCoord() = vcl::TexCoordf(1.0f, 0.0f);
    pf.wedge(2).texCoord() = vcl::TexCoordf(0.5f, 1.0f);

    // Test pushVertex: dynamic polygon expansion resizes wedges container
    pf.pushVertex(3);
    assert(pf.vertexCount() == 4);
    assert(pf.wedges().size() == 4);
    assert(pf.wedges().colors().size() == 4);
    assert(pf.wedges().texCoords().size() == 4);
    assert(pf.wedge(0).color() == vcl::Color::Red);
    assert(pf.wedge(2).color() == vcl::Color::Blue);

    pf.wedge(3).color() = vcl::Color::Yellow;
    assert(pf.wedge(3).color() == vcl::Color::Yellow);

    // Test insertVertex
    pf.insertVertex(1, 4);
    assert(pf.vertexCount() == 5);
    assert(pf.wedges().size() == 5);
    assert(pf.wedges().colors().size() == 5);
    assert(pf.wedge(0).color() == vcl::Color::Red);
    assert(pf.wedge(2).color() == vcl::Color::Green);
    assert(pf.wedge(3).color() == vcl::Color::Blue);

    // Test eraseVertex
    pf.eraseVertex(1);
    assert(pf.vertexCount() == 4);
    assert(pf.wedges().size() == 4);
    assert(pf.wedges().colors().size() == 4);
    assert(pf.wedge(0).color() == vcl::Color::Red);
    assert(pf.wedge(1).color() == vcl::Color::Green);
    assert(pf.wedge(2).color() == vcl::Color::Blue);

    // Test clearVertices
    pf.clearVertices();
    assert(pf.vertexCount() == 0);
    assert(pf.wedges().empty());
    assert(pf.wedges().colors().empty());
    assert(pf.wedges().texCoords().empty());
}

inline void testWedge()
{
    std::println("--- Testing Wedges Component & Proxies (Phase 2 & 3) ---");
    testWedgeConcepts();
    testTriangleWedges();
    testPolygonWedges();
    std::println("Wedges Component & Proxies verified successfully!");
}

#endif // TEST_WEDGE_H
