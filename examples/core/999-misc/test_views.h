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

inline void testViews()
{
    std::println("--- Testing Views (Phase 1: Element Views) ---");
    testElementViews();
    std::println("Views (Phase 1) verified successfully!\n");
}

#endif // TEST_VIEWS_H
