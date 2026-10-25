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

    // TransformMatrix Component Concepts
    static_assert(meshv2::mesh::HasTransformMatrix<TransformedMesh>);
    static_assert(meshv2::mesh::HasTransformMatrix<meshv2::mesh::TransformMatrixf>);
    static_assert(!meshv2::mesh::HasTransformMatrix<MyMesh>);
    static_assert(meshv2::HasTransformMatrix<TransformedMesh>);
    static_assert(!meshv2::HasTransformMatrix<MyMesh>);
    static_assert(!meshv2::ElementContainerConcept<meshv2::mesh::TransformMatrixf>);
    static_assert(meshv2::ComponentConcept<meshv2::mesh::TransformMatrixf>);
    static_assert(std::is_same_v<
                  TransformedMesh::Components,
                  vcl::TypeWrapper<meshv2::mesh::Color, meshv2::mesh::TransformMatrixf>>);

    // Materials Component Concepts
    static_assert(meshv2::mesh::HasMaterials<MaterialMeshWithMaterials>);
    static_assert(meshv2::mesh::HasMaterials<meshv2::mesh::Materials>);
    static_assert(!meshv2::mesh::HasMaterials<MyMesh>);
    static_assert(meshv2::HasMaterials<MaterialMeshWithMaterials>);
    static_assert(!meshv2::HasMaterials<MyMesh>);
    static_assert(!meshv2::ElementContainerConcept<meshv2::mesh::Materials>);
    static_assert(meshv2::ComponentConcept<meshv2::mesh::Materials>);
    static_assert(std::is_same_v<
                  MaterialMeshWithMaterials::Components,
                  vcl::TypeWrapper<meshv2::mesh::Materials>>);

    // MaterialIndex Component Concepts
    static_assert(meshv2::face::HasMaterialIndex<MaterialFaceComponents>);
    static_assert(meshv2::face::HasMaterialIndex<MaterialMesh::Face>);
    static_assert(!meshv2::face::HasMaterialIndex<MyFaceComponents>);
    static_assert(!meshv2::face::HasOptionalMaterialIndex<MaterialFaceComponents>);
    static_assert(meshv2::face::HasOptionalMaterialIndex<OptionalMaterialFaceComponents>);
    static_assert(meshv2::face::HasOptionalMaterialIndex<OptionalMaterialMesh::Face>);

    // MaterialIndex Type and Sentinel Concepts
    static_assert(std::is_same_v<MaterialMesh::Face::MaterialIndexType, ushort>);
    static_assert(MaterialMesh::Face::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<ushort>);
    static_assert(meshv2::comp::MaterialIndex<>::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<ushort>);
    static_assert(meshv2::face::MaterialIndex::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<ushort>);
    static_assert(meshv2::face::MaterialIndexu::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<uint>);

    // Default Value Concepts
    static_assert(meshv2::comp::HasDefaultValue<meshv2::comp::MaterialIndex<>>);
    static_assert(meshv2::comp::defaultValue<meshv2::comp::MaterialIndex<>>() == vcl::NULL_INDEX<ushort>);
    static_assert(meshv2::comp::HasDefaultValue<meshv2::face::MaterialIndex>);
    static_assert(meshv2::comp::defaultValue<meshv2::face::MaterialIndex>() == vcl::NULL_INDEX<ushort>);
    static_assert(meshv2::comp::defaultValue<meshv2::face::MaterialIndexu>() == vcl::NULL_INDEX<uint>);
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

    // NULL_INDEX exposure and consistency across all levels
    static_assert(MyMesh::NULL_INDEX == vcl::NULL_INDEX<MyMesh::IndexType>);
    static_assert(MyMesh::NULL_INDEX == MyMesh::Vertex::NULL_INDEX);
    static_assert(MyMesh::NULL_INDEX == MyMesh::Face::NULL_INDEX);
    static_assert(MyMesh::NULL_INDEX == MyMesh::VertexContainer::NULL_INDEX);
    static_assert(MyMesh::NULL_INDEX == MyMesh::FaceContainer::NULL_INDEX);
    static_assert(vcl::meshv2::comp::VertexReferences<3>::NULL_INDEX == vcl::NULL_INDEX<uint>);

    // 64-bit Mesh NULL_INDEX verification
    static_assert(std::is_same_v<Mesh64::IndexType, uint64_t>);
    static_assert(Mesh64::NULL_INDEX == 0xFFFFFFFFFFFFFFFFULL);
    static_assert(Mesh64::NULL_INDEX == vcl::NULL_INDEX<uint64_t>);
    static_assert(Mesh64::NULL_INDEX == Mesh64::Vertex::NULL_INDEX);
    static_assert(Mesh64::NULL_INDEX == Mesh64::Face::NULL_INDEX);
    static_assert(Mesh64::NULL_INDEX == Mesh64::VertexContainer::NULL_INDEX);
    static_assert(Mesh64::NULL_INDEX == Mesh64::FaceContainer::NULL_INDEX);
    static_assert(Mesh64::NULL_INDEX != MyMesh::NULL_INDEX);
}

inline void testIteratorConcepts()
{
    // Random access iterators
    static_assert(std::random_access_iterator<MyMesh::VertexContainer::Iterator>);
    static_assert(std::random_access_iterator<MyMesh::VertexContainer::ConstIterator>);
    static_assert(std::random_access_iterator<MyMesh::FaceContainer::Iterator>);
    static_assert(std::random_access_iterator<MyMesh::FaceContainer::ConstIterator>);

    // Component iterators
    static_assert(std::random_access_iterator<
        vcl::meshv2::ComponentIterator<vcl::meshv2::CompId::POSITION, MyMesh::VertexContainer>>);
    static_assert(std::random_access_iterator<
        vcl::meshv2::ComponentIterator<vcl::meshv2::CompId::NORMAL, MyMesh::VertexContainer>>);

    // Random access ranges
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().vertices())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().faces())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().vertices().positions())>);

    // Views over elements and components
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u))>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u) | vcl::meshv2::views::positions)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u) | vcl::meshv2::views::normals)>);
}

inline void testConcepts()
{
    std::println("--- Testing Concepts & Strong Typing ---");
    testComponentConcepts();
    testElementConcepts();
    testMeshConcepts();
    testIteratorConcepts();
    std::println("Compile-time concepts verified successfully!\n");
}

#endif // TEST_CONCEPTS_H
