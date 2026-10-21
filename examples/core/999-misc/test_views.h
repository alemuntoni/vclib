// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_VIEWS_H
#define TEST_VIEWS_H

#include "types.h"

#include <vclib/mesh_v2/views.h>

#include <cassert>
#include <print>
#include <ranges>
#include <vector>

inline void testElementViews()
{
    namespace views = meshv2::views;

    MyMesh m = createSampleQuadMesh(); // 4 vertices, 2 faces

    // --- 1. Range Concepts ---
    static_assert(meshv2::VertexRangeConcept<decltype(m.vertices())>);
    static_assert(meshv2::VertexRangeConcept<decltype(m | views::vertices)>);
    static_assert(meshv2::VertexRangeConcept<decltype(views::vertices(m))>);

    static_assert(meshv2::FaceRangeConcept<decltype(m.faces())>);
    static_assert(meshv2::FaceRangeConcept<decltype(m | views::faces)>);
    static_assert(meshv2::FaceRangeConcept<decltype(views::faces(m))>);

    // --- 2. Pipe Syntax on Mesh (m | views::vertices / m | views::faces) ---
    std::size_t vCount = 0;
    for (auto v : m | views::vertices) {
        assert(v.isValid());
        ++vCount;
    }
    assert(vCount == 4);

    std::size_t fCount = 0;
    for (auto f : m | views::faces) {
        assert(f.isValid());
        ++fCount;
    }
    assert(fCount == 2);

    // --- 3. Function Call Syntax (views::vertices(m) / views::faces(m)) ---
    vCount = 0;
    for (auto v : views::vertices(m)) {
        ++vCount;
    }
    assert(vCount == 4);

    fCount = 0;
    for (auto f : views::faces(m)) {
        ++fCount;
    }
    assert(fCount == 2);

    // --- 4. Generic elements<ELEM_ID> Adaptor ---
    vCount = 0;
    for (auto v : m | views::elements<meshv2::ElemId::VERTEX>) {
        ++vCount;
    }
    assert(vCount == 4);

    fCount = 0;
    for (auto f : m | views::elements<meshv2::ElemId::FACE>) {
        ++fCount;
    }
    assert(fCount == 2);

    // --- 5. Topology Navigation (face | views::vertices) ---
    auto f0 = m.face(0);
    std::vector<uint> face0VertexIndices;
    for (auto v : f0 | views::vertices) {
        face0VertexIndices.push_back(v.index());
    }
    assert(face0VertexIndices.size() == 3);
    assert(face0VertexIndices[0] == f0.vertexIndex(0));
    assert(face0VertexIndices[1] == f0.vertexIndex(1));
    assert(face0VertexIndices[2] == f0.vertexIndex(2));

    // Function call syntax on face: views::vertices(f0)
    face0VertexIndices.clear();
    for (auto v : views::vertices(f0)) {
        face0VertexIndices.push_back(v.index());
    }
    assert(face0VertexIndices.size() == 3);
    assert(face0VertexIndices[0] == f0.vertexIndex(0));

    // --- 6. views::indices ---
    std::vector<uint> vIndices;
    for (uint idx : m.vertices() | views::indices) {
        vIndices.push_back(idx);
    }
    assert((vIndices == std::vector<uint>{0, 1, 2, 3}));

    // Pipeline composition: m | views::vertices | views::indices
    vIndices.clear();
    for (uint idx : m | views::vertices | views::indices) {
        vIndices.push_back(idx);
    }
    assert((vIndices == std::vector<uint>{0, 1, 2, 3}));

    // Function syntax: views::indices(m.vertices())
    vIndices.clear();
    for (uint idx : views::indices(m.vertices())) {
        vIndices.push_back(idx);
    }
    assert((vIndices == std::vector<uint>{0, 1, 2, 3}));

    // Face indices
    std::vector<uint> fIndices;
    for (uint idx : m | views::faces | views::indices) {
        fIndices.push_back(idx);
    }
    assert((fIndices == std::vector<uint>{0, 1}));

    // Face vertices piped directly to indices: f0 | views::vertices | views::indices
    std::vector<uint> f0Indices;
    for (uint idx : f0 | views::vertices | views::indices) {
        f0Indices.push_back(idx);
    }
    assert(f0Indices.size() == 3);
    assert(f0Indices[0] == 0 && f0Indices[1] == 1 && f0Indices[2] == 2);

    // --- 7. Element Views skipping deleted elements ---
    m.deleteVertex(1);
    vIndices.clear();
    for (uint idx : m | views::vertices | views::indices) {
        vIndices.push_back(idx);
    }
    assert((vIndices == std::vector<uint>{0, 2, 3}));
}

inline void testSelectionViews()
{
    namespace views = meshv2::views;

    MyMesh m = createSampleQuadMesh(); // 4 vertices (0, 1, 2, 3), 2 faces (0, 1)

    // --- 1. Concepts ---
    static_assert(meshv2::SelectableRangeConcept<decltype(m.vertices())>);
    static_assert(meshv2::SelectableRangeConcept<decltype(m | views::vertices)>);
    static_assert(meshv2::SelectableRangeConcept<decltype(m.faces())>);
    static_assert(meshv2::SelectableRangeConcept<decltype(m | views::faces)>);
    static_assert(meshv2::SelectableRangeConcept<decltype(m.vertices() | views::selected)>);
    static_assert(meshv2::SelectableRangeConcept<decltype(m.vertices() | views::notSelected)>);

    // --- 2. Initial state: nothing selected ---
    std::size_t selCount = 0;
    for (auto v : m.vertices() | views::selected) {
        (void)v;
        ++selCount;
    }
    assert(selCount == 0);

    std::size_t notSelCount = 0;
    for (auto v : m.vertices() | views::notSelected) {
        (void)v;
        ++notSelCount;
    }
    assert(notSelCount == 4);

    // --- 3. Selecting vertices 0 and 2 ---
    m.vertex(0).selected() = true;
    m.vertex(2).selected() = true;

    // Pipe syntax: m.vertices() | views::selected
    std::vector<uint> selIndices;
    for (uint idx : m.vertices() | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    assert((selIndices == std::vector<uint>{0, 2}));

    // Pipeline composition: m | views::vertices | views::selected | views::indices
    selIndices.clear();
    for (uint idx : m | views::vertices | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    assert((selIndices == std::vector<uint>{0, 2}));

    // Function call syntax: views::selected(m.vertices())
    selIndices.clear();
    for (uint idx : views::selected(m.vertices()) | views::indices) {
        selIndices.push_back(idx);
    }
    assert((selIndices == std::vector<uint>{0, 2}));

    // --- 4. views::notSelected ---
    std::vector<uint> notSelIndices;
    for (uint idx : m.vertices() | views::notSelected | views::indices) {
        notSelIndices.push_back(idx);
    }
    assert((notSelIndices == std::vector<uint>{1, 3}));

    // Function call syntax: views::notSelected(m.vertices())
    notSelIndices.clear();
    for (uint idx : views::notSelected(m.vertices()) | views::indices) {
        notSelIndices.push_back(idx);
    }
    assert((notSelIndices == std::vector<uint>{1, 3}));

    // --- 5. views::selection (reading bit values) ---
    std::vector<bool> selValues;
    for (bool s : m.vertices() | views::selection) {
        selValues.push_back(s);
    }
    assert((selValues == std::vector<bool>{true, false, true, false}));

    // Writing through views::selection (BitProxy assignment)
    for (auto s : m.vertices() | views::selection) {
        s = true;
    }
    selIndices.clear();
    for (uint idx : m.vertices() | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    assert((selIndices == std::vector<uint>{0, 1, 2, 3}));

    // Resetting selection
    for (auto s : m.vertices() | views::selection) {
        s = false;
    }
    selIndices.clear();
    for (uint idx : m.vertices() | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    assert(selIndices.empty());

    // --- 6. Face Selection ---
    m.face(1).selected() = true;

    std::vector<uint> selFaces;
    for (uint idx : m | views::faces | views::selected | views::indices) {
        selFaces.push_back(idx);
    }
    assert((selFaces == std::vector<uint>{1}));

    std::vector<uint> notSelFaces;
    for (uint idx : m | views::faces | views::notSelected | views::indices) {
        notSelFaces.push_back(idx);
    }
    assert((notSelFaces == std::vector<uint>{0}));

    // --- 7. Selection combined with Deleted Elements ---
    m.vertex(0).selected() = true;
    m.vertex(1).selected() = true;
    m.vertex(2).selected() = true;
    m.deleteVertex(0); // Vertex 0 is deleted, so it must be skipped

    selIndices.clear();
    for (uint idx : m | views::vertices | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    assert((selIndices == std::vector<uint>{1, 2}));

    // --- 8. Modifying ElementProxy through views::selected iteration ---
    for (auto v : m.vertices() | views::selected) {
        v.selected() = false;
    }
    selIndices.clear();
    for (uint idx : m | views::vertices | views::selected | views::indices) {
        selIndices.push_back(idx);
    }
    assert(selIndices.empty());
}

inline void testComponentViews()
{
    namespace views = meshv2::views;

    MyMesh m = createSampleQuadMesh(); // 4 vertices, 2 faces

    // --- 1. Concepts ---
    // Input ranges satisfy PositionRangeConcept
    static_assert(meshv2::PositionRangeConcept<decltype(m.vertices())>);
    static_assert(meshv2::PositionRangeConcept<decltype(m | views::vertices)>);
    static_assert(meshv2::PositionRangeConcept<decltype(m.vertices() | views::selected)>);

    // Output ranges satisfy std::ranges::range and yield Point3f
    static_assert(std::ranges::range<decltype(m.vertices() | views::positions)>);
    static_assert(std::ranges::range<decltype(m.vertices() | views::selected | views::positions)>);
    static_assert(std::same_as<std::ranges::range_value_t<decltype(m.vertices() | views::positions)>, Point3f>);

    static_assert(meshv2::NormalRangeConcept<decltype(m.vertices())>);
    static_assert(meshv2::NormalRangeConcept<decltype(m | views::vertices)>);
    static_assert(meshv2::NormalRangeConcept<decltype(m.vertices() | views::selected)>);
    static_assert(std::ranges::range<decltype(m.vertices() | views::normals)>);

    // MyFace does not have Normal, so NormalRangeConcept is false for m.faces()
    static_assert(!meshv2::NormalRangeConcept<decltype(m.faces())>);
    static_assert(!meshv2::NormalRangeConcept<decltype(m | views::faces)>);

    static_assert(meshv2::ColorRangeConcept<decltype(m.vertices())>);
    static_assert(meshv2::ColorRangeConcept<decltype(m | views::vertices)>);
    static_assert(meshv2::ColorRangeConcept<decltype(m.faces())>);
    static_assert(meshv2::ColorRangeConcept<decltype(m | views::faces)>);
    static_assert(std::ranges::range<decltype(m.vertices() | views::colors)>);
    static_assert(std::ranges::range<decltype(m.faces() | views::colors)>);

    // --- 2. views::positions ---
    // Fast path: m.vertices() | views::positions returns ComponentView
    std::size_t posCount = 0;
    for (const Point3f& p : m.vertices() | views::positions) {
        (void)p;
        ++posCount;
    }
    assert(posCount == 4);

    // Function call syntax: views::positions(m.vertices())
    posCount = 0;
    for (const Point3f& p : views::positions(m.vertices())) {
        (void)p;
        ++posCount;
    }
    assert(posCount == 4);

    // Pipeline composition: m | views::vertices | views::positions
    posCount = 0;
    for (const Point3f& p : m | views::vertices | views::positions) {
        (void)p;
        ++posCount;
    }
    assert(posCount == 4);

    // Mutation through ComponentView: shift all vertices by (1, 0, 0)
    for (Point3f& p : m.vertices() | views::positions) {
        p.x() += 1.0f;
    }
    assert(m.vertex(0).position().x() == 1.0f); // Was 0.0f
    assert(m.vertex(1).position().x() == 2.0f); // Was 1.0f

    // Transform path: filtered view m.vertices() | views::selected | views::positions
    m.vertex(0).selected() = true;
    m.vertex(2).selected() = true;
    std::size_t selPosCount = 0;
    for (Point3f& p : m.vertices() | views::selected | views::positions) {
        p.y() += 5.0f;
        ++selPosCount;
    }
    assert(selPosCount == 2);
    assert(m.vertex(0).position().y() == 5.0f); // Was 0.0f
    assert(m.vertex(1).position().y() == 0.0f); // Not selected, unchanged
    assert(m.vertex(2).position().y() == 6.0f); // Was 1.0f
    assert(m.vertex(3).position().y() == 1.0f); // Not selected, unchanged

    // Topological traversal: face | views::vertices | views::positions
    auto f0 = m.face(0);
    std::vector<Point3f> f0Positions;
    for (const Point3f& p : f0 | views::vertices | views::positions) {
        f0Positions.push_back(p);
    }
    assert(f0Positions.size() == 3);
    assert(f0Positions[0] == m.vertex(f0.vertexIndex(0)).position());
    assert(f0Positions[1] == m.vertex(f0.vertexIndex(1)).position());
    assert(f0Positions[2] == m.vertex(f0.vertexIndex(2)).position());

    // --- 3. views::normals ---
    // Vertex normals
    std::ranges::fill(m.vertices() | views::normals, Point3f(0.0f, 0.0f, 1.0f));
    for (const Point3f& n : m.vertices() | views::normals) {
        assert((n == Point3f(0.0f, 0.0f, 1.0f)));
    }

    // Filtered vertex normals
    m.vertex(0).selected() = true;
    m.vertex(1).selected() = false;
    for (Point3f& n : m.vertices() | views::selected | views::normals) {
        n = Point3f(1.0f, 0.0f, 0.0f);
    }
    assert((m.vertex(0).normal() == Point3f(1.0f, 0.0f, 0.0f)));
    assert((m.vertex(1).normal() == Point3f(0.0f, 0.0f, 1.0f)));

    // Face normals on a mesh type with face normals
    using FaceWithNormals = meshv2::FaceComponents<
        meshv2::face::BitFlags,
        meshv2::face::TriangleVertexReferences<vcl::uint>,
        meshv2::face::Normal3f,
        meshv2::face::Color>;
    using MeshWithFaceNormals = meshv2::Mesh<
        meshv2::VertexContainer<MyVertexComponents>,
        meshv2::FaceContainer<FaceWithNormals>>;

    MeshWithFaceNormals mfn;
    mfn.addVertex();
    mfn.addVertex();
    mfn.addVertex();
    mfn.addFace(0, 1, 2);

    static_assert(meshv2::NormalRangeConcept<decltype(mfn.faces())>);
    static_assert(meshv2::NormalRangeConcept<decltype(mfn | views::faces)>);

    std::ranges::fill(mfn.faces() | views::normals, Point3f(0.0f, 1.0f, 0.0f));
    for (const Point3f& n : mfn.faces() | views::normals) {
        assert((n == Point3f(0.0f, 1.0f, 0.0f)));
    }

    // --- 4. views::colors ---
    // Fill all vertex colors
    std::ranges::fill(m.vertices() | views::colors, Color::Red);
    for (const Color& c : m.vertices() | views::colors) {
        assert(c == Color::Red);
    }

    // Fill selected vertex colors
    m.vertex(0).selected() = false;
    m.vertex(1).selected() = true;
    m.vertex(2).selected() = false;
    m.vertex(3).selected() = true;
    std::ranges::fill(m.vertices() | views::selected | views::colors, Color::Green);
    assert(m.vertex(0).color() == Color::Red);
    assert(m.vertex(1).color() == Color::Green);
    assert(m.vertex(2).color() == Color::Red);
    assert(m.vertex(3).color() == Color::Green);

    // Fill face colors
    std::ranges::fill(m.faces() | views::colors, Color::Blue);
    for (const Color& c : m.faces() | views::colors) {
        assert(c == Color::Blue);
    }

    // --- 5. Generic views::component<COMP_ID> ---
    std::size_t cPosCount = 0;
    for (const auto& p : m.vertices() | views::component<meshv2::CompId::POSITION>) {
        (void)p;
        ++cPosCount;
    }
    assert(cPosCount == 4);

    std::size_t cNormCount = 0;
    for (const auto& n : m.vertices() | views::component<meshv2::CompId::NORMAL>) {
        (void)n;
        ++cNormCount;
    }
    assert(cNormCount == 4);

    std::size_t cColCount = 0;
    for (const auto& c : m.faces() | views::component<meshv2::CompId::COLOR>) {
        (void)c;
        ++cColCount;
    }
    assert(cColCount == 2);

    // --- 6. Composite adaptors ---
    auto sel_pos = views::selected | views::positions;
    std::size_t compCount = 0;
    for (const auto& p : m.vertices() | sel_pos) {
        (void)p;
        ++compCount;
    }
    assert(compCount == 2); // Vertices 1 and 3 are selected
}

inline void testViews()
{
    std::println("--- Testing Views (Phase 1: Element Views) ---");
    testElementViews();
    std::println("Views (Phase 1) verified successfully!\n");

    std::println("--- Testing Views (Phase 2: Selection Views) ---");
    testSelectionViews();
    std::println("Views (Phase 2) verified successfully!\n");

    std::println("--- Testing Views (Phase 3: Component Views) ---");
    testComponentViews();
    std::println("Views (Phase 3) verified successfully!\n");
}

#endif // TEST_VIEWS_H
