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

inline void testViews()
{
    std::println("--- Testing Views (Phase 1: Element Views) ---");
    testElementViews();
    std::println("Views (Phase 1) verified successfully!\n");

    std::println("--- Testing Views (Phase 2: Selection Views) ---");
    testSelectionViews();
    std::println("Views (Phase 2) verified successfully!\n");
}

#endif // TEST_VIEWS_H
