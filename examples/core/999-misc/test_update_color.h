// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_UPDATE_COLOR_H
#define TEST_UPDATE_COLOR_H

#include "types.h"

#include <vclib/algorithms/mesh/create.h>
#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/meshes.h>

#include <cassert>
#include <print>
#include <vector>

namespace {

template<typename Func>
void assertThrowsMissingComponent(Func&& func)
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

inline void testSetPerElementColor()
{
    // --- Vertex Color ---
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableVertexColor();

        // 1. Default (White)
        vcl::setPerVertexColor(m);
        for (const auto v : m.vertices()) {
            assert(v.color() == vcl::Color::White);
        }

        // 2. Custom color (Red)
        vcl::setPerVertexColor(m, vcl::Color::Red);
        for (const auto v : m.vertices()) {
            assert(v.color() == vcl::Color::Red);
        }

        // 3. onlySelected = true
        m.vertex(1).selected() = true;
        vcl::setPerVertexColor(m, vcl::Color::Green, true);
        assert(m.vertex(0).color() == vcl::Color::Red);
        assert(m.vertex(1).color() == vcl::Color::Green);
        assert(m.vertex(2).color() == vcl::Color::Red);
        assert(m.vertex(3).color() == vcl::Color::Red);
    }

    // --- Face Color ---
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableFaceColor();

        // 1. Default (White)
        vcl::setPerFaceColor(m);
        for (const auto f : m.faces()) {
            assert(f.color() == vcl::Color::White);
        }

        // 2. Custom color (Blue)
        vcl::setPerFaceColor(m, vcl::Color::Blue);
        for (const auto f : m.faces()) {
            assert(f.color() == vcl::Color::Blue);
        }

        // 3. onlySelected = true
        m.face(0).selected() = true;
        vcl::setPerFaceColor(m, vcl::Color::Yellow, true);
        assert(m.face(0).color() == vcl::Color::Yellow);
        assert(m.face(1).color() == vcl::Color::Blue);
        assert(m.face(2).color() == vcl::Color::Blue);
        assert(m.face(3).color() == vcl::Color::Blue);
    }

    // --- Edge Color ---
    {
        vcl::EdgeMesh em;
        em.enableEdgeColor();
        em.addVertices(
            vcl::Point3d(0, 0, 0),
            vcl::Point3d(1, 0, 0),
            vcl::Point3d(0, 1, 0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        // 1. Default (White)
        vcl::setPerEdgeColor(em);
        for (const auto e : em.edges()) {
            assert(e.color() == vcl::Color::White);
        }

        // 2. Custom color (Magenta)
        vcl::setPerEdgeColor(em, vcl::Color::Magenta);
        for (const auto e : em.edges()) {
            assert(e.color() == vcl::Color::Magenta);
        }

        // 3. onlySelected = true
        em.edge(0).selected() = true;
        vcl::setPerEdgeColor(em, vcl::Color::Cyan, true);
        assert(em.edge(0).color() == vcl::Color::Cyan);
        assert(em.edge(1).color() == vcl::Color::Magenta);
    }
}

inline void testColorTransferBetweenElements()
{
    // --- Vertex <-> Face ---
    {
        vcl::TriMesh m;
        m.enableVertexColor();
        m.enableFaceColor();
        m.addVertices(
            vcl::Point3d(0, 0, 0),
            vcl::Point3d(1, 0, 0),
            vcl::Point3d(0, 1, 0),
            vcl::Point3d(1, 1, 0));
        m.addFace(0, 1, 2);
        m.addFace(1, 3, 2);

        // 1. setPerFaceColorFromVertexColor
        m.vertex(0).color() = vcl::Color(30, 60, 90, 255);
        m.vertex(1).color() = vcl::Color(60, 90, 120, 255);
        m.vertex(2).color() = vcl::Color(90, 120, 150, 255);
        m.vertex(3).color() = vcl::Color(120, 150, 180, 255);

        vcl::setPerFaceColorFromVertexColor(m);
        // Face 0: (0, 1, 2) -> avg = (60, 90, 120, 255)
        assert(m.face(0).color() == vcl::Color(60, 90, 120, 255));
        // Face 1: (1, 3, 2) -> avg = (90, 120, 150, 255)
        assert(m.face(1).color() == vcl::Color(90, 120, 150, 255));

        // 2. setPerVertexColorFromFaceColor
        m.face(0).color() = vcl::Color(100, 0, 0, 255);
        m.face(1).color() = vcl::Color(0, 100, 0, 255);

        vcl::setPerVertexColorFromFaceColor(m);
        assert(m.vertex(0).color() == vcl::Color(100, 0, 0, 255));
        assert(m.vertex(3).color() == vcl::Color(0, 100, 0, 255));
        assert(m.vertex(1).color() == vcl::Color(50, 50, 0, 255));
        assert(m.vertex(2).color() == vcl::Color(50, 50, 0, 255));
    }

    // --- Vertex -> Edge ---
    {
        vcl::EdgeMesh em;
        em.enableVertexColor();
        em.enableEdgeColor();
        em.addVertices(
            vcl::Point3d(0, 0, 0),
            vcl::Point3d(1, 0, 0),
            vcl::Point3d(0, 1, 0));
        em.addEdge(0, 1);
        em.addEdge(1, 2);

        em.vertex(0).color() = vcl::Color(100, 0, 0, 255);
        em.vertex(1).color() = vcl::Color(0, 100, 0, 255);
        em.vertex(2).color() = vcl::Color(0, 0, 100, 255);

        vcl::setPerEdgeColorFromVertexColor(em);
        assert(em.edge(0).color() == vcl::Color(50, 50, 0, 255));
        assert(em.edge(1).color() == vcl::Color(0, 50, 50, 255));
    }
}

inline void testColorFromQuality()
{
    // --- Vertex Quality -> Vertex Color ---
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableVertexColor();
        m.enableVertexQuality();

        m.vertex(0).quality() = 0.0;
        m.vertex(1).quality() = 1.0;
        m.vertex(2).quality() = 0.5;
        m.vertex(3).quality() = 0.5;

        // Auto min/max range
        vcl::setPerVertexColorFromQuality(m, vcl::Color::ColorMap::RedBlue);
        assert(
            m.vertex(0).color() ==
            vcl::colorFromInterval(0.0, 1.0, 0.0, vcl::Color::ColorMap::RedBlue));
        assert(
            m.vertex(1).color() ==
            vcl::colorFromInterval(0.0, 1.0, 1.0, vcl::Color::ColorMap::RedBlue));

        // Explicit min/max range
        vcl::setPerVertexColorFromQuality(
            m, vcl::Color::ColorMap::Parula, -2.0, 2.0);
        assert(
            m.vertex(0).color() ==
            vcl::colorFromInterval(-2.0, 2.0, 0.0, vcl::Color::ColorMap::Parula));
        assert(
            m.vertex(1).color() ==
            vcl::colorFromInterval(-2.0, 2.0, 1.0, vcl::Color::ColorMap::Parula));
    }

    // --- Face Quality -> Face Color ---
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableFaceColor();
        m.enableFaceQuality();

        m.face(0).quality() = 10.0;
        m.face(1).quality() = 20.0;
        m.face(2).quality() = 15.0;
        m.face(3).quality() = 15.0;

        // Auto min/max range
        vcl::setPerFaceColorFromQuality(m, vcl::Color::ColorMap::GreyShade);
        assert(
            m.face(0).color() ==
            vcl::colorFromInterval(
                10.0, 20.0, 10.0, vcl::Color::ColorMap::GreyShade));
        assert(
            m.face(1).color() ==
            vcl::colorFromInterval(
                10.0, 20.0, 20.0, vcl::Color::ColorMap::GreyShade));

        // Explicit min/max range
        vcl::setPerFaceColorFromQuality(
            m, vcl::Color::ColorMap::RedBlue, 0.0, 100.0);
        assert(
            m.face(0).color() ==
            vcl::colorFromInterval(
                0.0, 100.0, 10.0, vcl::Color::ColorMap::RedBlue));
        assert(
            m.face(1).color() ==
            vcl::colorFromInterval(
                0.0, 100.0, 20.0, vcl::Color::ColorMap::RedBlue));
    }
}

inline void testColorFromMaterial()
{
    vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
    m.enableVertexColor();
    m.enableFaceColor();
    m.enableVertexMaterialIndex();
    m.enableFaceMaterialIndex();

    vcl::Material mat0;
    mat0.baseColor() = vcl::Color::Red;
    vcl::Material mat1;
    mat1.baseColor() = vcl::Color::Blue;

    m.pushMaterial(mat0);
    m.pushMaterial(mat1);

    // Vertex materials
    m.vertex(0).materialIndex() = 0;
    m.vertex(1).materialIndex() = 1;
    m.vertex(2).materialIndex() = 0;
    m.vertex(3).materialIndex() = 1;

    vcl::setPerVertexColorFromMaterial(m);
    assert(m.vertex(0).color() == vcl::Color::Red);
    assert(m.vertex(1).color() == vcl::Color::Blue);
    assert(m.vertex(2).color() == vcl::Color::Red);
    assert(m.vertex(3).color() == vcl::Color::Blue);

    // Face materials
    m.face(0).materialIndex() = 0;
    m.face(1).materialIndex() = 1;
    m.face(2).materialIndex() = 0;
    m.face(3).materialIndex() = 1;

    vcl::setPerFaceColorFromMaterial(m);
    assert(m.face(0).color() == vcl::Color::Red);
    assert(m.face(1).color() == vcl::Color::Blue);
    assert(m.face(2).color() == vcl::Color::Red);
    assert(m.face(3).color() == vcl::Color::Blue);
}

inline void testColorFromFaceBorderFlag()
{
    // Quad with 2 triangles:
    // Face 0: (0, 1, 2)
    // Face 1: (1, 3, 2)
    // Edge (1, 2) is internal, edges (0,1), (2,0), (1,3), (3,2) are on border.
    vcl::TriMesh m;
    m.enableVertexColor();
    m.addVertices(
        vcl::Point3d(0, 0, 0),
        vcl::Point3d(1, 0, 0),
        vcl::Point3d(0, 1, 0),
        vcl::Point3d(1, 1, 0));
    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    m.face(0).edgeOnBorder(0) = true;  // (0, 1) -> border
    m.face(0).edgeOnBorder(1) = false; // (1, 2) -> internal
    m.face(0).edgeOnBorder(2) = true;  // (2, 0) -> border

    m.face(1).edgeOnBorder(0) = true;  // (1, 3) -> border
    m.face(1).edgeOnBorder(1) = true;  // (3, 2) -> border
    m.face(1).edgeOnBorder(2) = false; // (2, 1) -> internal

    vcl::setPerVertexColorFromFaceBorderFlag(
        m, vcl::Color::Blue, vcl::Color::White, vcl::Color::Cyan);

    // v0: incident only to border edges -> Blue
    assert(m.vertex(0).color() == vcl::Color::Blue);
    // v3: incident only to border edges -> Blue
    assert(m.vertex(3).color() == vcl::Color::Blue);
    // v1 and v2: incident to both border and internal edges -> Cyan
    assert(m.vertex(1).color() == vcl::Color::Cyan);
    assert(m.vertex(2).color() == vcl::Color::Cyan);
}

inline void testColorFromConnectedComponents()
{
    // 1. With precomputed connected components
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableFaceColor();

        std::vector<std::vector<vcl::uint>> comps = {{0, 1}, {2, 3}};
        vcl::setPerFaceColorFromConnectedComponents(m, comps);

        assert(m.face(0).color() == m.face(1).color());
        assert(m.face(2).color() == m.face(3).color());
        assert(m.face(0).color() != m.face(2).color());
    }

    // 2. With automatic connected components computation
    {
        vcl::TriMesh m;
        m.enableFaceColor();
        m.enableFaceAdjacentFaces();

        // 2 disconnected triangles
        m.addVertices(
            vcl::Point3d(0, 0, 0),
            vcl::Point3d(1, 0, 0),
            vcl::Point3d(0, 1, 0),
            vcl::Point3d(10, 0, 0),
            vcl::Point3d(11, 0, 0),
            vcl::Point3d(10, 1, 0));
        m.addFace(0, 1, 2);
        m.addFace(3, 4, 5);

        vcl::updatePerFaceAdjacentFaces(m);
        vcl::setPerFaceColorFromConnectedComponents(m);

        assert(m.face(0).color() != m.face(1).color());
    }
}

inline void testFaceColorScattering()
{
    // 1. Without faux edges
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableFaceColor();

        vcl::setPerFaceColorScattering(m, 10, false);
        for (const auto f : m.faces()) {
            assert(f.color() != vcl::Color::Black);
            assert(f.color().alpha() == 255);
        }
    }

    // 2. With faux edges
    {
        vcl::TriMesh m;
        m.enableFaceColor();
        m.enableFaceAdjacentFaces();

        m.addVertices(
            vcl::Point3d(0, 0, 0),
            vcl::Point3d(1, 0, 0),
            vcl::Point3d(0, 1, 0),
            vcl::Point3d(1, 1, 0));
        m.addFace(0, 1, 2);
        m.addFace(1, 3, 2);

        vcl::updatePerFaceAdjacentFaces(m);

        // Mark shared edge between face 0 and face 1 as faux
        // In face 0, edge 1 is (1, 2), adjacent to face 1
        m.face(0).edgeFaux(1) = true;

        vcl::setPerFaceColorScattering(m, 50, true);
        assert(m.face(0).color() == m.face(1).color());
    }
}

inline void testPerlinColor()
{
    // 1. setPerVertexColorPerlinNoise
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableVertexColor();

        vcl::Point3d period(1.0, 1.0, 1.0);
        vcl::setPerVertexColorPerlinNoise(m, period);

        for (const auto v : m.vertices()) {
            assert(v.color().alpha() == 255);
        }

        // Test onlySelected
        m.vertex(0).selected() = true;
        const vcl::Color oldColor1 = m.vertex(1).color();
        vcl::Point3d     newPeriod(0.2, 0.2, 0.2);
        vcl::setPerVertexColorPerlinNoise(
            m, newPeriod, vcl::Point3d(1, 1, 1), true);

        assert(m.vertex(1).color() == oldColor1);
    }

    // 2. setPerVertexPerlinColor
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        m.enableVertexColor();

        vcl::setPerVertexPerlinColor(
            m, 2.0, vcl::Point3d(0, 0, 0), vcl::Color::Black, vcl::Color::White);

        for (const auto v : m.vertices()) {
            assert(v.color().alpha() == 255);
        }

        // Test onlySelected
        m.vertex(0).selected() = true;
        const vcl::Color oldColor1 = m.vertex(1).color();
        vcl::setPerVertexPerlinColor(
            m,
            0.5,
            vcl::Point3d(2, 2, 2),
            vcl::Color::Red,
            vcl::Color::Blue,
            true);

        assert(m.vertex(1).color() == oldColor1);
    }
}

inline void testColorExceptions()
{
    // Test that all algorithms throw MissingComponentException when required
    // components are not enabled.

    // setPerVertexColor
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingComponent([&]() { vcl::setPerVertexColor(m); });
    }

    // setPerFaceColor
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingComponent([&]() { vcl::setPerFaceColor(m); });
    }

    // setPerEdgeColor
    {
        vcl::EdgeMesh em;
        em.addVertices(vcl::Point3d(0, 0, 0), vcl::Point3d(1, 0, 0));
        em.addEdge(0, 1);
        assertThrowsMissingComponent([&]() { vcl::setPerEdgeColor(em); });
    }

    // setPerVertexColorFromFaceColor
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromFaceColor(m); });
        // Missing face colors
        m.enableVertexColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromFaceColor(m); });
    }

    // setPerFaceColorFromVertexColor
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromVertexColor(m); });
        // Missing vertex colors
        m.enableFaceColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromVertexColor(m); });
    }

    // setPerEdgeColorFromVertexColor
    {
        vcl::EdgeMesh em;
        em.addVertices(vcl::Point3d(0, 0, 0), vcl::Point3d(1, 0, 0));
        em.addEdge(0, 1);
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerEdgeColorFromVertexColor(em); });
        // Missing edge colors
        em.enableVertexColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerEdgeColorFromVertexColor(em); });
    }

    // setPerVertexColorFromQuality
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromQuality(m); });
        // Missing quality
        m.enableVertexColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromQuality(m); });
    }

    // setPerFaceColorFromQuality
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromQuality(m); });
        // Missing quality
        m.enableFaceColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromQuality(m); });
    }

    // setPerVertexColorFromMaterial
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromMaterial(m); });
        // Missing material index
        m.enableVertexColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromMaterial(m); });
    }

    // setPerFaceColorFromMaterial
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        // Missing both
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromMaterial(m); });
        // Missing material index
        m.enableFaceColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromMaterial(m); });
    }

    // setPerVertexColorFromFaceBorderFlag
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerVertexColorFromFaceBorderFlag(m); });
    }

    // setPerFaceColorFromConnectedComponents
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        std::vector<std::vector<vcl::uint>> comps = {{0, 1}, {2, 3}};
        assertThrowsMissingComponent([&]() {
            vcl::setPerFaceColorFromConnectedComponents(m, comps);
        });
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromConnectedComponents(m); });
        // Enable face colors but miss adjacent faces
        m.enableFaceColor();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorFromConnectedComponents(m); });
    }

    // setPerFaceColorScattering
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingComponent(
            [&]() { vcl::setPerFaceColorScattering(m); });
    }

    // setPerVertexColorPerlinNoise
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingComponent([&]() {
            vcl::setPerVertexColorPerlinNoise(m, vcl::Point3d(1, 1, 1));
        });
    }

    // setPerVertexPerlinColor
    {
        vcl::TriMesh m = vcl::createTetrahedron<vcl::TriMesh>();
        assertThrowsMissingComponent([&]() {
            vcl::setPerVertexPerlinColor(m, 1.0);
        });
    }
}

inline void testUpdateColor()
{
    std::println("Running update/color tests...");
    testSetPerElementColor();
    testColorTransferBetweenElements();
    testColorFromQuality();
    testColorFromMaterial();
    testColorFromFaceBorderFlag();
    testColorFromConnectedComponents();
    testFaceColorScattering();
    testPerlinColor();
    testColorExceptions();
    std::println("All update/color tests passed!");
}

#endif // TEST_UPDATE_COLOR_H