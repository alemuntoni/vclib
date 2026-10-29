// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CONCEPTS_H
#define TEST_CONCEPTS_H

#include "types.h"
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/traits.h>

inline void testComponentConcepts()
{
    // Component lists satisfy component concepts
    static_assert(HasRequiredVertexComponents<MyVertexComponents>);
    static_assert(HasRequiredFaceComponents<MyFaceComponents>);

    // Color Component Concepts
    static_assert(mesh::HasColor<MyMesh>);
    static_assert(mesh::HasColor<mesh::Color>);
    static_assert(!mesh::HasColor<SimpleMesh>);

    static_assert(vert::HasColor<MyMesh::Vertex>);
    static_assert(vert::HasColor<MyMesh::ConstVertex>);
    static_assert(vert::HasColor<MyVertexComponents>);
    static_assert(!vert::HasColor<SimpleMesh::Vertex>);

    static_assert(face::HasColor<MyMesh::Face>);
    static_assert(face::HasColor<MyMesh::ConstFace>);
    static_assert(face::HasColor<MyFaceComponents>);

    // Optional vs Non-Optional Concepts
    static_assert(vert::HasOptionalColor<MyMesh::Vertex>);
    static_assert(!vert::HasOptionalColor<FullMesh::Vertex>);
    static_assert(vert::HasOptionalNormal<MyMesh::Vertex>);
    static_assert(!vert::HasOptionalNormal<FullMesh::Vertex>);
    static_assert(face::HasOptionalColor<MyMesh::Face>);
    static_assert(!face::HasOptionalColor<FullMesh::Face>);

    // Container and Component Concepts
    static_assert(mesh::ElementContainerConcept<
                  MyMesh::ContainerType<ElemId::VERTEX>>);
    static_assert(mesh::ElementContainerConcept<
                  MyMesh::ContainerType<ElemId::FACE>>);
    static_assert(!mesh::ElementContainerConcept<mesh::Color>);
    static_assert(comp::ComponentConcept<mesh::Color>);
    static_assert(!comp::ComponentConcept<
                  MyMesh::ContainerType<ElemId::VERTEX>>);
    static_assert(!comp::ComponentConcept<
                  MyMesh::ContainerType<ElemId::FACE>>);
    static_assert(std::is_same_v<MyMesh::Components, vcl::TypeWrapper<mesh::Color>>);

    // TransformMatrix Component Concepts
    static_assert(mesh::HasTransformMatrix<TransformedMesh>);
    static_assert(mesh::HasTransformMatrix<mesh::TransformMatrixf>);
    static_assert(!mesh::HasTransformMatrix<MyMesh>);
    static_assert(HasTransformMatrix<TransformedMesh>);
    static_assert(!HasTransformMatrix<MyMesh>);
    static_assert(!mesh::ElementContainerConcept<mesh::TransformMatrixf>);
    static_assert(comp::ComponentConcept<mesh::TransformMatrixf>);
    static_assert(std::is_same_v<
                  TransformedMesh::Components,
                  vcl::TypeWrapper<mesh::Color, mesh::TransformMatrixf>>);

    // Materials Component Concepts
    static_assert(mesh::HasMaterials<MaterialMeshWithMaterials>);
    static_assert(mesh::HasMaterials<mesh::Materials>);
    static_assert(!mesh::HasMaterials<MyMesh>);
    static_assert(HasMaterials<MaterialMeshWithMaterials>);
    static_assert(!HasMaterials<MyMesh>);
    static_assert(!mesh::ElementContainerConcept<mesh::Materials>);
    static_assert(comp::ComponentConcept<mesh::Materials>);
    static_assert(std::is_same_v<
                  MaterialMeshWithMaterials::Components,
                  vcl::TypeWrapper<mesh::Materials>>);

    // BoundingBox Component Concepts
    static_assert(mesh::HasBoundingBox<BoundingBoxMesh>);
    static_assert(mesh::HasBoundingBox<mesh::BoundingBox3f>);
    static_assert(!mesh::HasBoundingBox<MyMesh>);
    static_assert(HasBoundingBox<BoundingBoxMesh>);
    static_assert(!HasBoundingBox<MyMesh>);
    static_assert(!mesh::ElementContainerConcept<mesh::BoundingBox3f>);
    static_assert(comp::ComponentConcept<mesh::BoundingBox3f>);
    static_assert(std::is_same_v<
                  BoundingBoxMesh::Components,
                  vcl::TypeWrapper<mesh::BoundingBox3f>>);

    // Name Component Concepts
    static_assert(mesh::HasName<NamedMesh>);
    static_assert(mesh::HasName<mesh::Name>);
    static_assert(!mesh::HasName<MyMesh>);
    static_assert(HasName<NamedMesh>);
    static_assert(!HasName<MyMesh>);
    static_assert(!mesh::ElementContainerConcept<mesh::Name>);
    static_assert(comp::ComponentConcept<mesh::Name>);
    static_assert(std::is_same_v<
                  NamedMesh::Components,
                  vcl::TypeWrapper<mesh::Name>>);

    // PrincipalCurvature Component Concepts
    static_assert(vert::HasPrincipalCurvature<CurvatureMesh::Vertex>);
    static_assert(vert::HasPrincipalCurvature<CurvatureMesh::ConstVertex>);
    static_assert(vert::HasPrincipalCurvature<CurvatureVertexComponents>);
    static_assert(!vert::HasPrincipalCurvature<MyMesh::Vertex>);
    static_assert(!vert::HasOptionalPrincipalCurvature<CurvatureVertexComponents>);
    static_assert(vert::HasOptionalPrincipalCurvature<OptionalCurvatureVertexComponents>);
    static_assert(vert::HasOptionalPrincipalCurvature<OptionalCurvatureMesh::Vertex>);
    static_assert(face::HasPrincipalCurvature<CurvatureMesh::Face>);
    static_assert(face::HasOptionalPrincipalCurvature<CurvatureMesh::Face>);
    static_assert(!face::HasPrincipalCurvature<MyMesh::Face>);
    static_assert(vcl::PrincipalCurvatureConcept<vcl::PrincipalCurvaturef>);
    static_assert(vcl::PrincipalCurvatureConcept<vcl::PrincipalCurvatured>);

    // Quality Component Concepts
    static_assert(vert::HasQuality<QualityMesh::Vertex>);
    static_assert(vert::HasQuality<QualityMesh::ConstVertex>);
    static_assert(vert::HasQuality<QualityVertexComponents>);
    static_assert(!vert::HasQuality<MyMesh::Vertex>);
    static_assert(!vert::HasOptionalQuality<QualityVertexComponents>);
    static_assert(vert::HasOptionalQuality<OptionalQualityVertexComponents>);
    static_assert(vert::HasOptionalQuality<OptionalQualityMesh::Vertex>);
    static_assert(face::HasQuality<QualityMesh::Face>);
    static_assert(face::HasOptionalQuality<QualityMesh::Face>);
    static_assert(!face::HasQuality<MyMesh::Face>);
    static_assert(std::is_same_v<QualityMesh::Vertex::QualityType, float>);
    static_assert(std::is_same_v<OptionalQualityMesh::Vertex::QualityType, double>);

    // Tangent Component Concepts
    static_assert(vert::HasTangent<TangentMesh::Vertex>);
    static_assert(vert::HasTangent<TangentMesh::ConstVertex>);
    static_assert(vert::HasTangent<TangentVertexComponents>);
    static_assert(!vert::HasTangent<MyMesh::Vertex>);
    static_assert(!vert::HasOptionalTangent<TangentVertexComponents>);
    static_assert(vert::HasOptionalTangent<OptionalTangentVertexComponents>);
    static_assert(vert::HasOptionalTangent<OptionalTangentMesh::Vertex>);
    static_assert(face::HasTangent<TangentMesh::Face>);
    static_assert(face::HasOptionalTangent<TangentMesh::Face>);
    static_assert(!face::HasTangent<MyMesh::Face>);
    static_assert(std::is_same_v<TangentMesh::Vertex::TangentType, Point3f>);
    static_assert(std::is_same_v<OptionalTangentMesh::Vertex::TangentType, Point3d>);
    static_assert(comp::HasDefaultValue<vert::Tangent3f>);

    // MaterialIndex Component Concepts
    static_assert(face::HasMaterialIndex<MaterialFaceComponents>);
    static_assert(face::HasMaterialIndex<MaterialMesh::Face>);
    static_assert(!face::HasMaterialIndex<MyFaceComponents>);
    static_assert(!face::HasOptionalMaterialIndex<MaterialFaceComponents>);
    static_assert(face::HasOptionalMaterialIndex<OptionalMaterialFaceComponents>);
    static_assert(face::HasOptionalMaterialIndex<OptionalMaterialMesh::Face>);

    // MaterialIndex Type and Sentinel Concepts
    static_assert(std::is_same_v<MaterialMesh::Face::MaterialIndexType, ushort>);
    static_assert(MaterialMesh::Face::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<ushort>);
    static_assert(comp::MaterialIndex<>::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<ushort>);
    static_assert(face::MaterialIndex::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<ushort>);
    static_assert(face::MaterialIndexu::NULL_MATERIAL_INDEX == vcl::NULL_INDEX<uint>);

    // Default Value Concepts
    static_assert(comp::HasDefaultValue<comp::MaterialIndex<>>);
    static_assert(comp::defaultValue<comp::MaterialIndex<>>() == vcl::NULL_INDEX<ushort>);
    static_assert(comp::HasDefaultValue<face::MaterialIndex>);
    static_assert(comp::defaultValue<face::MaterialIndex>() == vcl::NULL_INDEX<ushort>);
    static_assert(comp::defaultValue<face::MaterialIndexu>() == vcl::NULL_INDEX<uint>);

    // Adjacency Component Concepts
    static_assert(HasAdjacentFaces<AdjFaceComponents>);
    static_assert(HasAdjacentFaces<AdjMesh::Face>);
    static_assert(face::HasTriangleAdjacentFaces<AdjFaceComponents>);
    static_assert(face::HasTriangleAdjacentFaces<AdjMesh::Face>);
    static_assert(!face::HasPolygonAdjacentFaces<AdjMesh::Face>);
    static_assert(!face::HasOptionalAdjacentFaces<AdjMesh::Face>);

    static_assert(HasAdjacentFaces<AdjVertexComponents>);
    static_assert(HasAdjacentFaces<AdjMesh::Vertex>);
    static_assert(!comp::HasStaticAdjacentFaces<AdjMesh::Vertex>);

    static_assert(HasAdjacentVertices<AdjVertexComponents>);
    static_assert(HasAdjacentVertices<AdjMesh::Vertex>);

    static_assert(HasAdjacentEdges<AdjMesh::Vertex>);
    static_assert(vert::HasOptionalAdjacentEdges<AdjMesh::Vertex>);
    static_assert(HasAdjacentEdges<AdjMesh::Face>);
    static_assert(face::HasOptionalAdjacentEdges<AdjMesh::Face>);

    static_assert(HasVertexAdjacentFaces<AdjMesh>);
    static_assert(HasFaceAdjacentFaces<AdjMesh>);
    static_assert(HasVertexAdjacentVertices<AdjMesh>);
    static_assert(HasVertexAdjacentEdges<AdjMesh>);
    static_assert(HasFaceAdjacentEdges<AdjMesh>);
    static_assert(!HasVertexAdjacentFaces<MyMesh>);
}

inline void testElementConcepts()
{
    // Base element classes satisfy ElementConcept
    static_assert(ElementConcept<MyMesh::Vertex>);
    static_assert(ElementConcept<MyMesh::ConstVertex>);
    static_assert(ElementConcept<MyMesh::Face>);
    static_assert(ElementConcept<MyMesh::ConstFace>);
    static_assert(ElementConcept<MyEdgeMesh::Edge>);
    static_assert(ElementConcept<MyEdgeMesh::ConstEdge>);

    // VertexConcept
    static_assert(VertexConcept<MyMesh::Vertex>);
    static_assert(VertexConcept<MyMesh::ConstVertex>);
    static_assert(!VertexConcept<MyMesh::Face>);
    static_assert(!VertexConcept<MyEdgeMesh::Edge>);

    // FaceConcept
    static_assert(FaceConcept<MyMesh::Face>);
    static_assert(FaceConcept<MyMesh::ConstFace>);
    static_assert(FaceConcept<SimplePolyMesh::Face>);
    static_assert(FaceConcept<SimplePolyMesh::ConstFace>);
    static_assert(FaceConcept<SimpleQuadMesh::Face>);
    static_assert(FaceConcept<SimpleQuadMesh::ConstFace>);
    static_assert(!FaceConcept<MyMesh::Vertex>);
    static_assert(!FaceConcept<MyEdgeMesh::Edge>);

    // TriangleFaceConcept
    static_assert(TriangleFaceConcept<MyMesh::Face>);
    static_assert(TriangleFaceConcept<MyMesh::ConstFace>);
    static_assert(!TriangleFaceConcept<SimpleQuadMesh::Face>);
    static_assert(!TriangleFaceConcept<SimplePolyMesh::Face>);

    // QuadFaceConcept
    static_assert(QuadFaceConcept<SimpleQuadMesh::Face>);
    static_assert(QuadFaceConcept<SimpleQuadMesh::ConstFace>);
    static_assert(!QuadFaceConcept<MyMesh::Face>);
    static_assert(!QuadFaceConcept<SimplePolyMesh::Face>);

    // PolygonFaceConcept
    static_assert(PolygonFaceConcept<SimplePolyMesh::Face>);
    static_assert(PolygonFaceConcept<SimplePolyMesh::ConstFace>);
    static_assert(!PolygonFaceConcept<MyMesh::Face>);
    static_assert(!PolygonFaceConcept<SimpleQuadMesh::Face>);

    // EdgeConcept
    static_assert(EdgeConcept<MyEdgeMesh::Edge>);
    static_assert(EdgeConcept<MyEdgeMesh::ConstEdge>);
    static_assert(!EdgeConcept<MyMesh::Vertex>);
    static_assert(!EdgeConcept<MyMesh::Face>);

    // Component presence concepts on elements (HasPosition, HasNormal, HasColor, etc.)
    // HasPosition
    static_assert(HasPosition<MyMesh::Vertex>);
    static_assert(HasPosition<MyMesh::ConstVertex>);
    static_assert(HasPosition<MyVertexComponents>);
    static_assert(!HasPosition<MyMesh::Face>);

    // HasNormal
    static_assert(HasNormal<MyMesh::Vertex>);
    static_assert(HasNormal<MyMesh::ConstVertex>);
    static_assert(!HasNormal<SimpleMesh::Vertex>);
    static_assert(!HasNormal<MyMesh::Face>);

    // HasColor
    static_assert(HasColor<MyMesh::Vertex>);
    static_assert(HasColor<MyMesh::Face>);
    static_assert(!HasColor<SimpleMesh::Vertex>);

    // HasAdjacentFaces
    static_assert(HasAdjacentFaces<AdjMesh::Vertex>);
    static_assert(HasAdjacentFaces<AdjMesh::Face>);
    static_assert(!HasAdjacentFaces<MyMesh::Vertex>);

    // HasAdjacentEdges
    static_assert(HasAdjacentEdges<AdjMesh::Vertex>);
    static_assert(HasAdjacentEdges<AdjMesh::Face>);
    static_assert(!HasAdjacentEdges<MyMesh::Vertex>);

    // HasAdjacentVertices
    static_assert(HasAdjacentVertices<AdjMesh::Vertex>);
    static_assert(!HasAdjacentVertices<MyMesh::Vertex>);

    // HasBitFlags
    static_assert(HasBitFlags<MyMesh::Vertex>);
    static_assert(HasBitFlags<MyMesh::Face>);
    static_assert(HasBitFlags<MyEdgeMesh::Edge>);

    // HasQuality
    static_assert(HasQuality<QualityMesh::Vertex>);
    static_assert(HasQuality<QualityMesh::Face>);
    static_assert(!HasQuality<MyMesh::Vertex>);

    // HasPrincipalCurvature
    static_assert(HasPrincipalCurvature<CurvatureMesh::Vertex>);
    static_assert(HasPrincipalCurvature<CurvatureMesh::Face>);
    static_assert(!HasPrincipalCurvature<MyMesh::Vertex>);

    // HasTangent
    static_assert(HasTangent<TangentMesh::Vertex>);
    static_assert(HasTangent<TangentMesh::Face>);
    static_assert(!HasTangent<MyMesh::Vertex>);

    // HasMaterialIndex
    static_assert(HasMaterialIndex<MaterialMesh::Face>);
    static_assert(!HasMaterialIndex<MyMesh::Face>);

    // HasVertexReferences
    static_assert(HasVertexReferences<MyMesh::Face>);
    static_assert(HasVertexReferences<MyEdgeMesh::Edge>);
    static_assert(!HasVertexReferences<MyMesh::Vertex>);

    // HasCustomComponents
    static_assert(HasCustomComponents<CCMesh::Vertex>);
    static_assert(HasCustomComponents<CCMesh::Face>);
    static_assert(!HasCustomComponents<MyMesh::Vertex>);
}


inline void testMeshConcepts()
{
    // Base Mesh Structure & Container Presence
    static_assert(MeshConcept<MyMesh>);
    static_assert(HasContainerOf<MyMesh, ElemId::VERTEX>);
    static_assert(HasContainerOf<MyMesh, ElemId::FACE>);
    static_assert(!HasContainerOf<MyMesh, ElemId::EDGE>);
    static_assert(HasFaces<MyMesh>);
    static_assert(!HasEdges<MyMesh>);
    static_assert(FaceMeshConcept<MyMesh>);
    static_assert(!EdgeMeshConcept<MyMesh>);

    static_assert(!HasFaces<MyEdgeMesh>);
    static_assert(HasEdges<MyEdgeMesh>);
    static_assert(!FaceMeshConcept<MyEdgeMesh>);
    static_assert(EdgeMeshConcept<MyEdgeMesh>);

    static_assert(HasFaces<GraphMesh>);
    static_assert(HasEdges<GraphMesh>);
    static_assert(FaceMeshConcept<GraphMesh>);
    static_assert(EdgeMeshConcept<GraphMesh>);

    static_assert(MeshConcept<PointCloudMesh>);
    static_assert(!HasFaces<PointCloudMesh>);
    static_assert(!HasEdges<PointCloudMesh>);
    static_assert(!FaceMeshConcept<PointCloudMesh>);
    static_assert(!EdgeMeshConcept<PointCloudMesh>);

    // Face Topology Specializations
    static_assert(HasTriangles<MyMesh>);
    static_assert(!HasQuads<MyMesh>);
    static_assert(!HasPolygons<MyMesh>);
    static_assert(TriangleMeshConcept<MyMesh>);
    static_assert(!QuadMeshConcept<MyMesh>);
    static_assert(!PolygonMeshConcept<MyMesh>);

    static_assert(!HasTriangles<SimpleQuadMesh>);
    static_assert(HasQuads<SimpleQuadMesh>);
    static_assert(!HasPolygons<SimpleQuadMesh>);
    static_assert(!TriangleMeshConcept<SimpleQuadMesh>);
    static_assert(QuadMeshConcept<SimpleQuadMesh>);
    static_assert(!PolygonMeshConcept<SimpleQuadMesh>);

    static_assert(!HasTriangles<SimplePolyMesh>);
    static_assert(!HasQuads<SimplePolyMesh>);
    static_assert(HasPolygons<SimplePolyMesh>);
    static_assert(!TriangleMeshConcept<SimplePolyMesh>);
    static_assert(!QuadMeshConcept<SimplePolyMesh>);
    static_assert(PolygonMeshConcept<SimplePolyMesh>);

    static_assert(!TriangleMeshConcept<PointCloudMesh>);
    static_assert(!TriangleMeshConcept<MyEdgeMesh>);

    // Element Components on Mesh
    // Normals
    static_assert(HasVertexNormals<FullMesh>);
    static_assert(HasOptionalVertexNormals<MyMesh>);
    static_assert(!HasVertexNormals<SimpleMesh>);
    static_assert(!HasFaceNormals<MyMesh>);
    static_assert(HasFaceNormals<TangentMesh>);

    // Colors
    static_assert(HasVertexColors<MyMesh>);
    static_assert(HasOptionalVertexColors<MyMesh>);
    static_assert(HasFaceColors<MyMesh>);
    static_assert(HasOptionalFaceColors<MyMesh>);
    static_assert(HasVertexColors<FullMesh>);
    static_assert(!HasOptionalVertexColors<FullMesh>);
    static_assert(HasFaceColors<FullMesh>);
    static_assert(!HasOptionalFaceColors<FullMesh>);
    static_assert(!HasVertexColors<SimpleMesh>);
    static_assert(HasFaceColors<SimpleMesh>);
    static_assert(HasOptionalFaceColors<SimpleMesh>);
    static_assert(!HasFaceColors<SimplePolyMesh>);
    static_assert(!HasFaceColors<SimpleQuadMesh>);
    static_assert(!HasFaceColors<MyEdgeMesh>);
    static_assert(HasEdgeColors<MyEdgeMesh>);
    static_assert(!HasOptionalEdgeColors<MyEdgeMesh>);
    static_assert(!HasEdgeColors<MyMesh>);

    // Adjacencies
    static_assert(HasVertexAdjacentFaces<AdjMesh>);
    static_assert(HasFaceAdjacentFaces<AdjMesh>);
    static_assert(HasVertexAdjacentVertices<AdjMesh>);
    static_assert(HasVertexAdjacentEdges<AdjMesh>);
    static_assert(HasFaceAdjacentEdges<AdjMesh>);
    static_assert(!HasVertexAdjacentFaces<MyMesh>);
    static_assert(!HasFaceAdjacentFaces<MyMesh>);
    static_assert(!HasVertexAdjacentEdges<MyMesh>);

    // Qualities
    static_assert(HasVertexQualities<QualityMesh>);
    static_assert(HasFaceQualities<QualityMesh>);
    static_assert(!HasOptionalVertexQualities<QualityMesh>);
    static_assert(HasOptionalFaceQualities<QualityMesh>);
    static_assert(HasOptionalVertexQualities<OptionalQualityMesh>);
    static_assert(!HasVertexQualities<MyMesh>);

    // Curvatures
    static_assert(HasVertexPrincipalCurvatures<CurvatureMesh>);
    static_assert(HasFacePrincipalCurvatures<CurvatureMesh>);
    static_assert(!HasOptionalVertexPrincipalCurvatures<CurvatureMesh>);
    static_assert(HasOptionalFacePrincipalCurvatures<CurvatureMesh>);
    static_assert(HasOptionalVertexPrincipalCurvatures<OptionalCurvatureMesh>);
    static_assert(!HasVertexPrincipalCurvatures<MyMesh>);

    // Tangents
    static_assert(HasVertexTangents<TangentMesh>);
    static_assert(HasFaceTangents<TangentMesh>);
    static_assert(!HasOptionalVertexTangents<TangentMesh>);
    static_assert(HasOptionalFaceTangents<TangentMesh>);
    static_assert(HasOptionalVertexTangents<OptionalTangentMesh>);
    static_assert(!HasVertexTangents<MyMesh>);

    // Material Index
    static_assert(HasFaceMaterialIndices<MaterialMesh>);
    static_assert(!HasFaceMaterialIndices<MyMesh>);
    static_assert(HasOptionalFaceMaterialIndices<OptionalMaterialMesh>);

    // Custom Components
    static_assert(HasVertexCustomComponents<CCMesh>);
    static_assert(HasFaceCustomComponents<CCMesh>);
    static_assert(HasEdgeCustomComponents<EdgeMeshWithCC>);
    static_assert(!HasVertexCustomComponents<MyMesh>);
    static_assert(!HasEdgeCustomComponents<MyMesh>);
    static_assert(!HasFaceCustomComponents<MyMesh>);

    // Mesh-Level Components
    static_assert(HasTransformMatrix<TransformedMesh>);
    static_assert(!HasTransformMatrix<MyMesh>);
    static_assert(HasBoundingBox<BoundingBoxMesh>);
    static_assert(!HasBoundingBox<MyMesh>);
    static_assert(HasMaterials<MaterialMeshWithMaterials>);
    static_assert(!HasMaterials<MyMesh>);
    static_assert(HasName<NamedMesh>);
    static_assert(!HasName<MyMesh>);
    static_assert(HasMeshColor<MyMesh>);
    static_assert(!HasMeshColor<SimpleMesh>);
    static_assert(HasMeshCustomComponents<CCMesh>);
    static_assert(!HasMeshCustomComponents<MyMesh>);

    // NULL_INDEX exposure and consistency across all levels
    static_assert(MyMesh::NULL_INDEX == vcl::NULL_INDEX<MyMesh::IndexType>);
    static_assert(MyMesh::NULL_INDEX == MyMesh::Vertex::NULL_INDEX);
    static_assert(MyMesh::NULL_INDEX == MyMesh::Face::NULL_INDEX);
    static_assert(MyMesh::NULL_INDEX == MyMesh::VertexContainer::NULL_INDEX);
    static_assert(MyMesh::NULL_INDEX == MyMesh::FaceContainer::NULL_INDEX);
    static_assert(vcl::comp::VertexReferences<3>::NULL_INDEX == vcl::NULL_INDEX<uint>);

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
        vcl::mesh::ComponentIterator<vcl::CompId::POSITION, MyMesh::VertexContainer>>);
    static_assert(std::random_access_iterator<
        vcl::mesh::ComponentIterator<vcl::CompId::NORMAL, MyMesh::VertexContainer>>);
    static_assert(std::random_access_iterator<
        vcl::mesh::ComponentIterator<vcl::CompId::QUALITY, QualityMesh::VertexContainer>>);
    static_assert(std::random_access_iterator<
        vcl::mesh::ComponentIterator<vcl::CompId::TANGENT, TangentMesh::VertexContainer>>);

    // Random access ranges
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().vertices())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().faces())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().vertices().positions())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<QualityMesh>().vertices().qualities())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<TangentMesh>().vertices().tangents())>);

    // Views over elements and components
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::ElemId::VERTEX>(0u))>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::ElemId::VERTEX>(0u) | vcl::views::positions)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::ElemId::VERTEX>(0u) | vcl::views::normals)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<QualityMesh>().elements<vcl::ElemId::VERTEX>(0u) | vcl::views::qualities)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<TangentMesh>().elements<vcl::ElemId::VERTEX>(0u) | vcl::views::tangents)>);
}

inline void testContracts()
{
    // 1. Compactness tests
    MyMesh m = createSampleQuadMesh();
    assert(m.isCompact());
    assert(m.deletedCount<ElemId::VERTEX>() == 0);
    assert(m.deletedCount<ElemId::FACE>() == 0);
    requireCompactness(m);
    requireVertexCompactness(m);
    requireFaceCompactness(m);

    // Mark a face as deleted
    m.deleteFace(0);
    assert(!m.isCompact());
    assert(m.deletedCount<ElemId::FACE>() != 0);
    assert(m.deletedCount<ElemId::VERTEX>() == 0); // vertices are still compact

    bool caught = false;
    try {
        requireFaceCompactness(m);
    } catch (const MissingCompactnessException&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        requireCompactness(m);
    } catch (const MissingCompactnessException&) {
        caught = true;
    }
    assert(caught);

    // Compact the mesh
    m.compact();
    assert(m.isCompact());
    requireCompactness(m);

    // 2. Topology tests
    assert(isTriangleMesh(m));
    assert(!isQuadMesh(m));
    requireTriangleMesh(m);

    SimpleQuadMesh qm;
    assert(isQuadMesh(qm));
    assert(!isTriangleMesh(qm));
    requireQuadMesh(qm);

    caught = false;
    try {
        requireQuadMesh(m);
    } catch (const MissingQuadRequirementException&) {
        caught = true;
    }
    assert(caught);

    // 3. Component availability & enablement
    MyMesh optMesh;
    optMesh.addVertices(3);
    optMesh.addFace(0, 1, 2);

    // Optional vertex normals start disabled
    assert((!optMesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    caught = false;
    try {
        requireVertexNormals(optMesh);
    } catch (const MissingComponentException&) {
        caught = true;
    }
    assert(caught);

    // Enable optional vertex normals
    bool enabled = enableIfVertexNormalsOptional(optMesh);
    assert(enabled);
    assert((optMesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    requireVertexNormals(optMesh); // succeeds without throwing

    // Optional face color starts disabled
    assert((!optMesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));
    caught = false;
    try {
        requireFaceColors(optMesh);
    } catch (const MissingComponentException&) {
        caught = true;
    }
    assert(caught);

    enableIfFaceColorsOptional(optMesh);
    assert((optMesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));
    requireFaceColors(optMesh);

    // Non-optional components on FullMesh are always available
    FullMesh fullMesh;
    assert((fullMesh.isElementComponentAvailable<ElemId::VERTEX, CompId::NORMAL>()));
    assert((fullMesh.isElementComponentAvailable<ElemId::VERTEX, CompId::COLOR>()));
    assert((fullMesh.isElementComponentAvailable<ElemId::FACE, CompId::COLOR>()));
    requireVertexNormals(fullMesh);
    requireVertexColors(fullMesh);
    requireFaceColors(fullMesh);

    // MyEdgeMesh
    MyEdgeMesh edgeMesh = createSampleEdgeMesh();
    assert((edgeMesh.isElementComponentAvailable<ElemId::EDGE, CompId::COLOR>()));
    assert((!edgeMesh.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>())); // optional quality not enabled
    enableIfEdgeQualitiesOptional(edgeMesh);
    assert((edgeMesh.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
    requireEdgeQualities(edgeMesh);

    // Generic requireElementComponent
    requireElementComponent<ElemId::VERTEX, CompId::NORMAL>(optMesh);
}

template<typename T>
concept HasNormalType = requires {
    typename NormalType<T>;
};

template<typename T>
concept HasTexCoordType = requires {
    typename TexCoordType<T>;
};

inline void testTraits()
{
    // 1. VertexType
    static_assert(std::is_same_v<VertexType<SimpleMesh>, SimpleMesh::Vertex>);
    static_assert(std::is_same_v<VertexType<const SimpleMesh&>, SimpleMesh::Vertex>);
    static_assert(std::is_same_v<VertexType<SimpleMesh::Vertex>, SimpleMesh::Vertex>);
    static_assert(std::is_same_v<VertexType<SimpleMesh::Face>, SimpleMesh::Vertex>);
    static_assert(std::is_same_v<VertexType<const SimpleMesh::Face&>, SimpleMesh::Vertex>);
    static_assert(std::is_same_v<VertexType<MyEdgeMesh::Edge>, MyEdgeMesh::Vertex>);
    static_assert(std::is_same_v<VertexType<SimpleMesh::ContainerType<ElemId::VERTEX>>, SimpleMesh::Vertex>);

    // 2. FaceType
    static_assert(std::is_same_v<FaceType<SimpleMesh>, SimpleMesh::Face>);
    static_assert(std::is_same_v<FaceType<const SimpleMesh&>, SimpleMesh::Face>);
    static_assert(std::is_same_v<FaceType<SimpleMesh::Face>, SimpleMesh::Face>);
    static_assert(std::is_same_v<FaceType<const SimpleMesh::Face&>, SimpleMesh::Face>);
    static_assert(std::is_same_v<FaceType<SimpleMesh::ContainerType<ElemId::FACE>>, SimpleMesh::Face>);

    // 3. EdgeType
    static_assert(std::is_same_v<EdgeType<MyEdgeMesh>, MyEdgeMesh::Edge>);
    static_assert(std::is_same_v<EdgeType<const MyEdgeMesh&>, MyEdgeMesh::Edge>);
    static_assert(std::is_same_v<EdgeType<MyEdgeMesh::Edge>, MyEdgeMesh::Edge>);
    static_assert(std::is_same_v<EdgeType<MyEdgeMesh::ContainerType<ElemId::EDGE>>, MyEdgeMesh::Edge>);

    // 4. IndexType
    static_assert(std::is_same_v<IndexType<SimpleMesh>, uint>);
    static_assert(std::is_same_v<IndexType<SimpleMesh::Vertex>, uint>);
    static_assert(std::is_same_v<IndexType<SimpleMesh::Face>, uint>);
    static_assert(std::is_same_v<IndexType<MyEdgeMesh::Edge>, uint>);

    // 5. PositionType
    static_assert(std::is_same_v<PositionType<SimpleMesh>, Point3f>);
    static_assert(std::is_same_v<PositionType<const SimpleMesh&>, Point3f>);
    static_assert(std::is_same_v<PositionType<SimpleMesh::Vertex>, Point3f>);
    static_assert(std::is_same_v<PositionType<FullMesh>, Point3f>);
    static_assert(std::is_same_v<PositionType<FullMesh::Vertex>, Point3f>);

    // 6. ScalarType
    static_assert(std::is_same_v<ScalarType<SimpleMesh>, float>);
    static_assert(std::is_same_v<ScalarType<const SimpleMesh&>, float>);
    static_assert(std::is_same_v<ScalarType<SimpleMesh::Vertex>, float>);
    static_assert(std::is_same_v<ScalarType<Point3f>, float>);
    static_assert(std::is_same_v<ScalarType<Point3d>, double>);

    // 7. NormalType
    static_assert(std::is_same_v<NormalType<MyMesh::Vertex>, Point3f>);
    static_assert(std::is_same_v<NormalType<FullMesh::Vertex>, Point3f>);
    static_assert(std::is_same_v<NormalType<TangentMesh::Face>, Point3f>);
    // Not available on Vertex without normal
    static_assert(!HasNormalType<SimpleMesh::Vertex>);
    // Not available on Face without face normal
    static_assert(!HasNormalType<SimpleMesh::Face>);
    static_assert(!HasNormalType<FullMesh::Face>);
    // Not available on Mesh directly to prevent ambiguity
    static_assert(!HasNormalType<SimpleMesh>);
    static_assert(!HasNormalType<FullMesh>);

    // 8. TexCoordType
    // Not available on Face or Mesh directly to prevent ambiguity
    static_assert(!HasTexCoordType<SimpleMesh::Face>);
    static_assert(!HasTexCoordType<SimpleMesh>);
}

inline void testConcepts()
{
    std::println("--- Testing Concepts & Strong Typing ---");
    testComponentConcepts();
    testElementConcepts();
    testMeshConcepts();
    testIteratorConcepts();
    testContracts();
    testTraits();
    std::println("Compile-time concepts & runtime contracts verified successfully!\n");
}

#endif // TEST_CONCEPTS_H
