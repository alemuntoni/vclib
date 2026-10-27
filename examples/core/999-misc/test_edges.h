// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_EDGES_H
#define TEST_EDGES_H

#include <cassert>
#include <print>
#include <ranges>
#include <vector>

#include <vclib/mesh/views/elements/edge.h>

#include "types.h"

inline void testEdgeConcepts()
{
    // Element concepts
    static_assert(ElementConcept<EdgeMesh::Edge>);
    static_assert(ElementConcept<EdgeMesh::ConstEdge>);
    static_assert(ElementConcept<GraphMesh::Edge>);
    static_assert(ElementConcept<GraphMesh::ConstEdge>);

    static_assert(EdgeConcept<EdgeMesh::Edge>);
    static_assert(EdgeConcept<EdgeMesh::ConstEdge>);
    static_assert(EdgeConcept<GraphMesh::Edge>);
    static_assert(EdgeConcept<GraphMesh::ConstEdge>);

    // Strong typing: Edge is not Vertex nor Face
    static_assert(!VertexConcept<EdgeMesh::Edge>);
    static_assert(!FaceConcept<EdgeMesh::Edge>);
    static_assert(!EdgeConcept<EdgeMesh::Vertex>);
    static_assert(!EdgeConcept<GraphMesh::Face>);

    // Component concepts
    static_assert(HasRequiredEdgeComponents<MyEdgeComponents>);
    static_assert(HasRequiredEdgeComponents<CCEdgeComponents>);
    static_assert(edge::HasEdgeVertexReferences<MyEdgeComponents>);
    static_assert(edge::HasEdgeVertexReferences<EdgeMesh::Edge>);
    static_assert(edge::HasColor<MyEdgeComponents>);
    static_assert(edge::HasColor<EdgeMesh::Edge>);
    static_assert(edge::HasOptionalQuality<MyEdgeComponents>);
    static_assert(edge::HasOptionalQuality<EdgeMesh::Edge>);

    // Mesh concepts
    static_assert(HasEdges<EdgeMesh>);
    static_assert(!HasFaces<EdgeMesh>);

    static_assert(HasFaces<GraphMesh>);
    static_assert(HasEdges<GraphMesh>);

    static_assert(!HasEdges<MyMesh>);

    static_assert(HasEdgeCustomComponents<EdgeMeshWithCC>);
    static_assert(!HasEdgeCustomComponents<EdgeMesh>);

    // Sentinels
    static_assert(EdgeMesh::NULL_INDEX == EdgeMesh::Edge::NULL_INDEX);
    static_assert(EdgeMesh::NULL_INDEX == EdgeMesh::EdgeContainer::NULL_INDEX);
    static_assert(GraphMesh::NULL_INDEX == GraphMesh::Edge::NULL_INDEX);

    // Iterators and Ranges
    static_assert(std::random_access_iterator<EdgeMesh::EdgeContainer::Iterator>);
    static_assert(std::random_access_iterator<EdgeMesh::EdgeContainer::ConstIterator>);
    static_assert(EdgeRangeConcept<decltype(std::declval<EdgeMesh>().edges())>);
    static_assert(EdgeRangeConcept<decltype(std::declval<const EdgeMesh>().edges())>);
}

inline void testEdgeCreationAndProperties()
{
    EdgeMesh m = createSampleEdgeMesh();

    assert(m.vertexCount() == 4);
    assert(m.edgeCount() == 4);
    assert(m.edgeContainerSize() == 4);
    assert(m.deletedEdgeCount() == 0);

    // Check vertex indices for each edge
    assert(m.edge(0).vertexIndex(0) == 0 && m.edge(0).vertexIndex(1) == 1);
    assert(m.edge(1).vertexIndex(0) == 1 && m.edge(1).vertexIndex(1) == 3);
    assert(m.edge(2).vertexIndex(0) == 3 && m.edge(2).vertexIndex(1) == 2);
    assert(m.edge(3).vertexIndex(0) == 2 && m.edge(3).vertexIndex(1) == 0);

    // Check navigating to Vertex proxies
    assert(m.edge(0).vertex(0).index() == 0);
    assert(m.edge(0).vertex(1).index() == 1);
    assert(m.edge(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    assert(m.edge(0).vertex(1).position() == Point3f(1.0f, 0.0f, 0.0f));

    // Test modifying vertices via setVertex and setVertices
    m.edge(0).setVertex(1, 2);
    assert(m.edge(0).vertexIndex(1) == 2);

    m.edge(0).setVertices(m.vertex(0), m.vertex(1));
    assert(m.edge(0).vertexIndex(0) == 0 && m.edge(0).vertexIndex(1) == 1);

    // Test color component
    m.edge(0).color() = Color::Red;
    assert(m.edge(0).color() == Color::Red);

    // Test optional quality
    assert((!m.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
    m.enableElementComponent<ElemId::EDGE, CompId::QUALITY>();
    assert((m.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
    m.edge(0).quality() = 42.0f;
    assert(m.edge(0).quality() == 42.0f);
    m.disableElementComponent<ElemId::EDGE, CompId::QUALITY>();
    assert((!m.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
}

inline void testEdgeContainerMethods()
{
    EdgeMesh m;
    m.addVertices(4);

    // addEdge()
    uint e0 = m.addEdge();
    assert(e0 == 0);
    assert(m.edgeCount() == 1);
    m.edge(e0).setVertices(0, 1);

    // addEdge(v0, v1)
    uint e1 = m.addEdge(1, 2);
    assert(e1 == 1);
    assert(m.edge(e1).vertexIndex(0) == 1 && m.edge(e1).vertexIndex(1) == 2);

    // addEdge(rng)
    std::array<uint, 2> edgeVerts = {2, 3};
    uint e2 = m.addEdge(edgeVerts);
    assert(e2 == 2);
    assert(m.edge(e2).vertexIndex(0) == 2 && m.edge(e2).vertexIndex(1) == 3);

    // addEdges(n)
    uint e3 = m.addEdges(2);
    assert(e3 == 3);
    assert(m.edgeCount() == 5);
    assert(m.edgeContainerSize() == 5);

    // addEdges(rng of ranges)
    std::vector<std::array<uint, 2>> batch = {{0, 2}, {1, 3}};
    uint e5 = m.addEdges(batch);
    assert(e5 == 5);
    assert(m.edgeCount() == 7);
    assert(m.edge(5).vertexIndex(0) == 0 && m.edge(5).vertexIndex(1) == 2);
    assert(m.edge(6).vertexIndex(0) == 1 && m.edge(6).vertexIndex(1) == 3);

    // reserve & resize
    m.reserveEdges(20);
    m.resizeEdges(4);
    assert(m.edgeCount() == 4);
    assert(m.edgeContainerSize() == 4);

    // clearEdges()
    m.clearEdges();
    assert(m.edgeCount() == 0);
    assert(m.edgeContainerSize() == 0);
}

inline void testEdgeViewsAndRanges()
{
    EdgeMesh m = createSampleEdgeMesh();

    // Loop with m.edges()
    uint count = 0;
    for (auto e : m.edges()) {
        assert(e.index() == count);
        count++;
    }
    assert(count == 4);

    // Views pipe syntax
    count = 0;
    for (auto e : m | views::edges) {
        assert(e.index() == count);
        count++;
    }
    assert(count == 4);

    // Views function syntax
    count = 0;
    for (auto e : views::edges(m)) {
        assert(e.index() == count);
        count++;
    }
    assert(count == 4);

    // Generic elements<ElemId::EDGE>()
    count = 0;
    for (auto e : m.elements<ElemId::EDGE>()) {
        assert(e.index() == count);
        count++;
    }
    assert(count == 4);

    // Slice range
    count = 0;
    for (auto e : m.edges(1, 3)) {
        assert(e.index() == count + 1);
        count++;
    }
    assert(count == 2);

    // Column access
    auto colors = m.edges().colors();
    assert(colors.size() == 4);
    colors[0] = Color::Red;
    assert(m.edge(0).color() == Color::Red);

    auto viCol = m.edges().vertexIndices();
    assert(viCol.size() == 4);
    assert(viCol[0][0] == 0 && viCol[0][1] == 1);
}

inline void testEdgeDeletionAndCompaction()
{
    EdgeMesh m = createSampleEdgeMesh();

    assert(m.edgeCount() == 4);
    assert(m.isCompact());

    // Delete edge 1
    m.deleteEdge(1);
    assert(!m.isCompact());
    assert(m.edgeCount() == 3);
    assert(m.edgeContainerSize() == 4);
    assert(m.deletedEdgeCount() == 1);

    // Iteration skips deleted element
    std::vector<uint> visited;
    for (auto e : m.edges()) {
        visited.push_back(e.index());
    }
    assert((visited == std::vector<uint>{0, 2, 3}));

    // Iteration without jumpDeleted
    visited.clear();
    for (auto e : m.edges(false)) {
        visited.push_back(e.index());
    }
    assert((visited == std::vector<uint>{0, 1, 2, 3}));

    // edgeIndexIfCompact
    assert(m.edgeIndexIfCompact(0) == 0);
    assert(m.edgeIndexIfCompact(2) == 1);
    assert(m.edgeIndexIfCompact(3) == 2);

    // Compact
    auto newIndices = m.compactEdges();
    assert(m.isCompact());
    assert(m.edgeCount() == 3);
    assert(m.edgeContainerSize() == 3);
    assert(m.deletedEdgeCount() == 0);

    assert(newIndices[0] == 0);
    assert(newIndices[1] == EdgeMesh::NULL_INDEX);
    assert(newIndices[2] == 1);
    assert(newIndices[3] == 2);

    assert(m.edge(0).vertexIndex(0) == 0 && m.edge(0).vertexIndex(1) == 1);
    assert(m.edge(1).vertexIndex(0) == 3 && m.edge(1).vertexIndex(1) == 2);
    assert(m.edge(2).vertexIndex(0) == 2 && m.edge(2).vertexIndex(1) == 0);
}

inline void testEdgeCustomComponents()
{
    EdgeMeshWithCC m;
    m.addVertices(4);
    m.addEdge(0, 1);
    m.addEdge(1, 2);

    assert(!m.hasEdgeCustomComponent("weight"));
    m.addEdgeCustomComponent<float>("weight");
    assert(m.hasEdgeCustomComponent("weight"));
    assert(m.isEdgeCustomComponentOfType<float>("weight"));

    auto names = m.edgeCustomComponentNames();
    assert(names.size() == 1 && names[0] == "weight");

    auto handle = m.edgeCustomComponentVectorHandle<float>("weight");
    handle[0] = 1.5f;
    handle[1] = 2.5f;

    assert(m.edge(0).customComponent<float>("weight") == 1.5f);
    assert(m.edge(1).customComponent<float>("weight") == 2.5f);

    m.deleteEdgeCustomComponent("weight");
    assert(!m.hasEdgeCustomComponent("weight"));
}

inline void testEdgeAppend()
{
    EdgeMesh m1;
    m1.addVertices(2);
    m1.addEdge(0, 1);

    EdgeMesh m2;
    m2.addVertices(3);
    m2.addEdge(0, 1);
    m2.addEdge(1, 2);

    m1.append(m2);

    assert(m1.vertexCount() == 5);
    assert(m1.edgeCount() == 3);

    // Initial edge from m1
    assert(m1.edge(0).vertexIndex(0) == 0 && m1.edge(0).vertexIndex(1) == 1);

    // Appended edges from m2: vertex references must have been shifted by 2!
    assert(m1.edge(1).vertexIndex(0) == 2 && m1.edge(1).vertexIndex(1) == 3);
    assert(m1.edge(2).vertexIndex(0) == 3 && m1.edge(2).vertexIndex(1) == 4);
}

inline void testEdges()
{
    std::println("--- Testing Edge and EdgeContainer ---");
    testEdgeConcepts();
    testEdgeCreationAndProperties();
    testEdgeContainerMethods();
    testEdgeViewsAndRanges();
    testEdgeDeletionAndCompaction();
    testEdgeCustomComponents();
    testEdgeAppend();
    std::println("Edge and EdgeContainer tests passed successfully!");
}

#endif // TEST_EDGES_H
