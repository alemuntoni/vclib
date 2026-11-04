// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_UPDATE_SELECTION_H
#define TEST_UPDATE_SELECTION_H

#include "types.h"

#include <vclib/algorithms/mesh/create.h>
#include <vclib/algorithms/mesh/stat/selection.h>
#include <vclib/algorithms/mesh/update/selection.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/meshes.h>

#include <cassert>
#include <numbers>
#include <print>

namespace {

template<typename Func>
void assertThrowsMissingSelectionComponent(Func&& func)
{
    bool thrown = false;
    try {
        func();
    }
    catch (const vcl::MissingComponentException&) {
        thrown = true;
    }
    assert(thrown);
}

} // namespace

inline void testClearSelection()
{
    // --- 1. Vertex Selection ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        assert(vcl::vertexSelectionCount(tet) == 0);

        for (auto v : tet.vertices()) {
            v.selected() = true;
        }
        assert(vcl::vertexSelectionCount(tet) == tet.vertexCount());

        vcl::clearVertexSelection(tet);
        assert(vcl::vertexSelectionCount(tet) == 0);
        for (const auto v : tet.vertices()) {
            assert(!v.selected());
        }

        // Test clearElementSelection<ElemId::VERTEX>
        tet.vertex(0).selected() = true;
        tet.vertex(2).selected() = true;
        assert(vcl::vertexSelectionCount(tet) == 2);

        vcl::clearElementSelection<vcl::ElemId::VERTEX>(tet);
        assert(vcl::vertexSelectionCount(tet) == 0);
        for (const auto v : tet.vertices()) {
            assert(!v.selected());
        }
    }

    // --- 2. Face Selection ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        assert(vcl::faceSelectionCount(tet) == 0);

        for (auto f : tet.faces()) {
            f.selected() = true;
        }
        assert(vcl::faceSelectionCount(tet) == tet.faceCount());

        vcl::clearFaceSelection(tet);
        assert(vcl::faceSelectionCount(tet) == 0);
        for (const auto f : tet.faces()) {
            assert(!f.selected());
        }

        // Test clearElementSelection<ElemId::FACE>
        tet.face(1).selected() = true;
        tet.face(3).selected() = true;
        assert(vcl::faceSelectionCount(tet) == 2);

        vcl::clearElementSelection<vcl::ElemId::FACE>(tet);
        assert(vcl::faceSelectionCount(tet) == 0);
        for (const auto f : tet.faces()) {
            assert(!f.selected());
        }
    }

    // --- 3. Face Edges Selection ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        assert(vcl::faceEdgesSelectionCount(tet) == 0);

        tet.face(0).edgeSelected(0) = true;
        tet.face(0).edgeSelected(2) = true;
        tet.face(1).edgeSelected(1) = true;
        assert(vcl::faceEdgesSelectionCount(tet) == 3);

        vcl::clearFaceEdgesSelection(tet);
        assert(vcl::faceEdgesSelectionCount(tet) == 0);
        for (const auto f : tet.faces()) {
            for (vcl::uint i = 0; i < f.vertexCount(); ++i) {
                assert(!f.edgeSelected(i));
            }
        }
    }

    // --- 4. Edge Selection (EdgeMesh) ---
    {
        vcl::EdgeMesh em;
        em.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(2.0, 0.0, 0.0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        em.edge(0).selected() = true;
        em.edge(1).selected() = true;
        assert(vcl::edgeSelectionCount(em) == 2);

        vcl::clearEdgeSelection(em);
        assert(vcl::edgeSelectionCount(em) == 0);
        for (const auto e : em.edges()) {
            assert(!e.selected());
        }

        // Test clearElementSelection<ElemId::EDGE>
        em.edge(1).selected() = true;
        assert(vcl::edgeSelectionCount(em) == 1);

        vcl::clearElementSelection<vcl::ElemId::EDGE>(em);
        assert(vcl::edgeSelectionCount(em) == 0);
        for (const auto e : em.edges()) {
            assert(!e.selected());
        }
    }
}

inline void testSelectNonManifoldVertices()
{
    // --- 1. Manifold Mesh (Tetrahedron) ---
    {
        vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
        tet.enableFaceAdjacentFaces();
        vcl::updatePerFaceAdjacentFaces(tet);

        vcl::selectNonManifoldVertices(tet);
        assert(vcl::vertexSelectionCount(tet) == 0);
        for (const auto v : tet.vertices()) {
            assert(!v.selected());
        }
    }

    // --- 2. Non-Manifold Mesh (Pinch / Bowtie Vertex) ---
    {
        // Two triangles sharing only vertex 0:
        // Face 0: (0, 1, 2)
        // Face 1: (0, 3, 4)
        vcl::TriMesh m;
        m.enableFaceAdjacentFaces();
        m.addVertices(
            vcl::Point3d(0.0, 0.0, 0.0),
            vcl::Point3d(1.0, 0.0, 0.0),
            vcl::Point3d(0.0, 1.0, 0.0),
            vcl::Point3d(-1.0, 0.0, 0.0),
            vcl::Point3d(0.0, -1.0, 0.0));
        m.addFace(0, 1, 2);
        m.addFace(0, 3, 4);

        vcl::updatePerFaceAdjacentFaces(m);

        // A. Default: clearSelectionFirst = true
        vcl::selectNonManifoldVertices(m, true);
        assert(m.vertex(0).selected());
        assert(!m.vertex(1).selected());
        assert(!m.vertex(2).selected());
        assert(!m.vertex(3).selected());
        assert(!m.vertex(4).selected());
        assert(vcl::vertexSelectionCount(m) == 1);

        // B. clearSelectionFirst = false: previously selected vertices remain selected
        m.vertex(1).selected() = true;
        vcl::selectNonManifoldVertices(m, false);
        assert(m.vertex(0).selected());
        assert(m.vertex(1).selected());
        assert(!m.vertex(2).selected());
        assert(!m.vertex(3).selected());
        assert(!m.vertex(4).selected());
        assert(vcl::vertexSelectionCount(m) == 2);

        // C. clearSelectionFirst = true resets vertex 1
        vcl::selectNonManifoldVertices(m, true);
        assert(m.vertex(0).selected());
        assert(!m.vertex(1).selected());
        assert(vcl::vertexSelectionCount(m) == 1);
    }
}

inline void testSelectCreaseFaceEdges()
{
    // Two triangles at a 90-degree angle sharing edge (0, 1):
    // Face 0: (0, 1, 2) in XY plane (z = 0), normal is (0, 0, 1)
    // Face 1: (0, 3, 1) in XZ plane (y = 0), normal is (0, -1, 0)
    // Edge (0, 1) is internal; edges (1, 2), (2, 0), (0, 3), (3, 1) are on border.
    // Dihedral angle between face 0 and face 1 is pi / 2 (~1.5708 rad).
    vcl::TriMesh m;
    m.enableFaceAdjacentFaces();
    m.addVertices(
        vcl::Point3d(0.0, 0.0, 0.0),
        vcl::Point3d(1.0, 0.0, 0.0),
        vcl::Point3d(0.0, 1.0, 0.0),
        vcl::Point3d(0.0, 0.0, 1.0));
    m.addFace(0, 1, 2);
    m.addFace(0, 3, 1);

    vcl::updatePerFaceAdjacentFaces(m);

    // Face 0: edge 0 is (0, 1) -> internal edge
    // Face 1: edge 2 is (1, 0) -> internal edge

    // 1. Threshold range [0.1, 0.5] does NOT include pi/2 (~1.5708)
    // So edge (0, 1) is a crease edge!
    vcl::selectCreaseFaceEdges(m, 0.1, 0.5, false);
    assert(m.face(0).edgeSelected(0));
    assert(!m.face(0).edgeSelected(1));
    assert(!m.face(0).edgeSelected(2));
    assert(m.face(1).edgeSelected(2));
    assert(!m.face(1).edgeSelected(0));
    assert(!m.face(1).edgeSelected(1));
    assert(vcl::faceEdgesSelectionCount(m) == 2);

    // 2. alsoBorderEdges = true: internal crease edge + 4 border edges = 6 edges
    vcl::selectCreaseFaceEdges(m, 0.1, 0.5, true);
    assert(vcl::faceEdgesSelectionCount(m) == 6);

    // 3. Threshold range [-3.0, 3.0] includes pi/2, so internal edge is NOT a crease edge
    vcl::selectCreaseFaceEdges(m, -3.0, 3.0, false);
    assert(vcl::faceEdgesSelectionCount(m) == 0);

    // 4. Threshold range [-3.0, 3.0] with alsoBorderEdges = true: only 4 border edges selected
    vcl::selectCreaseFaceEdges(m, -3.0, 3.0, true);
    assert(vcl::faceEdgesSelectionCount(m) == 4);
    assert(!m.face(0).edgeSelected(0));
    assert(!m.face(1).edgeSelected(2));
}

inline void testSelectionExceptions()
{
    // Calling selectNonManifoldVertices or selectCreaseFaceEdges without FaceAdjacentFaces enabled
    vcl::TriMesh tet = vcl::createTetrahedron<vcl::TriMesh>();
    // FaceAdjacentFaces is optional and not enabled

    assertThrowsMissingSelectionComponent([&]() {
        vcl::selectNonManifoldVertices(tet);
    });

    assertThrowsMissingSelectionComponent([&]() {
        vcl::selectCreaseFaceEdges(tet, 0.0, 1.0);
    });
}

inline void testUpdateSelection()
{
    std::println("--- Testing Update Selection Algorithms ---");
    testClearSelection();
    testSelectNonManifoldVertices();
    testSelectCreaseFaceEdges();
    testSelectionExceptions();
    std::println("--- Update Selection Algorithms Tests Passed! ---");
}

#endif // TEST_UPDATE_SELECTION_H

