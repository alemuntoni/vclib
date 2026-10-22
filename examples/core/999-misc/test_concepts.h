// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CONCEPTS_H
#define TEST_CONCEPTS_H

#include "types.h"

inline void testComponentConcepts()
{
    // Component lists satisfy component concepts
    static_assert(meshv2::HasRequiredVertexComponents<MyVertexComponents>);
    static_assert(meshv2::HasRequiredFaceComponents<MyFaceComponents>);

    // Color Component Concepts
    static_assert(meshv2::mesh::HasColor<MyMesh>);
    static_assert(meshv2::mesh::HasColor<meshv2::mesh::Color>);
    static_assert(!meshv2::mesh::HasColor<SimpleMesh>);

    static_assert(meshv2::vert::HasColor<MyMesh::Vertex>);
    static_assert(meshv2::vert::HasColor<MyMesh::ConstVertex>);
    static_assert(meshv2::vert::HasColor<MyVertexComponents>);
    static_assert(!meshv2::vert::HasColor<SimpleMesh::Vertex>);

    static_assert(meshv2::face::HasColor<MyMesh::Face>);
    static_assert(meshv2::face::HasColor<MyMesh::ConstFace>);
    static_assert(meshv2::face::HasColor<MyFaceComponents>);

    // Optional vs Non-Optional Concepts
    static_assert(meshv2::vert::HasOptionalColor<MyMesh::Vertex>);
    static_assert(!meshv2::vert::HasOptionalColor<FullMesh::Vertex>);
    static_assert(meshv2::vert::HasOptionalNormal<MyMesh::Vertex>);
    static_assert(!meshv2::vert::HasOptionalNormal<FullMesh::Vertex>);
    static_assert(meshv2::face::HasOptionalColor<MyMesh::Face>);
    static_assert(!meshv2::face::HasOptionalColor<FullMesh::Face>);

    // Container and Component Concepts
    static_assert(meshv2::ElementContainerConcept<
                  MyMesh::ContainerType<meshv2::ElemId::VERTEX>>);
    static_assert(meshv2::ElementContainerConcept<
                  MyMesh::ContainerType<meshv2::ElemId::FACE>>);
    static_assert(!meshv2::ElementContainerConcept<meshv2::mesh::Color>);
    static_assert(meshv2::ComponentConcept<meshv2::mesh::Color>);
    static_assert(!meshv2::ComponentConcept<
                  MyMesh::ContainerType<meshv2::ElemId::VERTEX>>);
    static_assert(!meshv2::ComponentConcept<
                  MyMesh::ContainerType<meshv2::ElemId::FACE>>);
    static_assert(std::is_same_v<MyMesh::Components, vcl::TypeWrapper<meshv2::mesh::Color>>);
}

inline void testElementConcepts()
{
    // The single element classes (Vertex and Face) satisfy concepts
    static_assert(meshv2::ElementConcept<MyMesh::Vertex>);
    static_assert(meshv2::ElementConcept<MyMesh::ConstVertex>);
    static_assert(meshv2::ElementConcept<MyMesh::Face>);
    static_assert(meshv2::ElementConcept<MyMesh::ConstFace>);

    static_assert(meshv2::VertexConcept<MyMesh::Vertex>);
    static_assert(meshv2::VertexConcept<MyMesh::ConstVertex>);
    static_assert(meshv2::FaceConcept<MyMesh::Face>);
    static_assert(meshv2::FaceConcept<MyMesh::ConstFace>);
    static_assert(meshv2::TriangleFaceConcept<MyMesh::Face>);
    static_assert(meshv2::TriangleFaceConcept<MyMesh::ConstFace>);
    static_assert(meshv2::face::HasTriangleVertexReferences<MyMesh::Face>);
    static_assert(meshv2::face::HasTriangleVertexReferences<MyMesh::ConstFace>);
    static_assert(meshv2::face::HasTriangleVertexReferences<MyFaceComponents>);

    // Polygon element concepts
    static_assert(meshv2::face::HasPolygonBitFlags<PolyMesh::Face>);
    static_assert(meshv2::face::HasPolygonBitFlags<PolyMesh::ConstFace>);
    static_assert(meshv2::face::HasPolygonVertexReferences<PolyMesh::Face>);
    static_assert(meshv2::face::HasPolygonVertexReferences<PolyMesh::ConstFace>);
    static_assert(meshv2::face::HasPolygonVertexReferences<PolyFaceComponents>);

    // Strong typing: Vertex and Face cannot be confused
    static_assert(!meshv2::VertexConcept<MyMesh::Face>);
    static_assert(!meshv2::FaceConcept<MyMesh::Vertex>);
}

inline void testMeshConcepts()
{
    // Mesh Concepts
    static_assert(meshv2::HasVertices<MyMesh>);
    static_assert(meshv2::HasFaces<MyMesh>);
    static_assert(meshv2::HasTriangles<MyMesh>);
    static_assert(meshv2::MeshConcept<MyMesh>);

    static_assert(meshv2::HasVertices<PolyMesh>);
    static_assert(meshv2::HasFaces<PolyMesh>);
    static_assert(meshv2::MeshConcept<PolyMesh>);
}

inline void testConcepts()
{
    std::println("--- Testing Concepts & Strong Typing ---");
    testComponentConcepts();
    testElementConcepts();
    testMeshConcepts();
    std::println("Compile-time concepts verified successfully!\n");
}

#endif // TEST_CONCEPTS_H
