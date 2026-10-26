// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_CONCEPTS_H
#define TEST_CONCEPTS_H

#include "types.h"
#include <vclib/mesh_v2/requirements.h>

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

    // BoundingBox Component Concepts
    static_assert(meshv2::mesh::HasBoundingBox<BoundingBoxMesh>);
    static_assert(meshv2::mesh::HasBoundingBox<meshv2::mesh::BoundingBox3f>);
    static_assert(!meshv2::mesh::HasBoundingBox<MyMesh>);
    static_assert(meshv2::HasBoundingBox<BoundingBoxMesh>);
    static_assert(!meshv2::HasBoundingBox<MyMesh>);
    static_assert(!meshv2::ElementContainerConcept<meshv2::mesh::BoundingBox3f>);
    static_assert(meshv2::ComponentConcept<meshv2::mesh::BoundingBox3f>);
    static_assert(std::is_same_v<
                  BoundingBoxMesh::Components,
                  vcl::TypeWrapper<meshv2::mesh::BoundingBox3f>>);

    // Name Component Concepts
    static_assert(meshv2::mesh::HasName<NamedMesh>);
    static_assert(meshv2::mesh::HasName<meshv2::mesh::Name>);
    static_assert(!meshv2::mesh::HasName<MyMesh>);
    static_assert(meshv2::HasName<NamedMesh>);
    static_assert(!meshv2::HasName<MyMesh>);
    static_assert(!meshv2::ElementContainerConcept<meshv2::mesh::Name>);
    static_assert(meshv2::ComponentConcept<meshv2::mesh::Name>);
    static_assert(std::is_same_v<
                  NamedMesh::Components,
                  vcl::TypeWrapper<meshv2::mesh::Name>>);

    // PrincipalCurvature Component Concepts
    static_assert(meshv2::vert::HasPrincipalCurvature<CurvatureMesh::Vertex>);
    static_assert(meshv2::vert::HasPrincipalCurvature<CurvatureMesh::ConstVertex>);
    static_assert(meshv2::vert::HasPrincipalCurvature<CurvatureVertexComponents>);
    static_assert(!meshv2::vert::HasPrincipalCurvature<MyMesh::Vertex>);
    static_assert(!meshv2::vert::HasOptionalPrincipalCurvature<CurvatureVertexComponents>);
    static_assert(meshv2::vert::HasOptionalPrincipalCurvature<OptionalCurvatureVertexComponents>);
    static_assert(meshv2::vert::HasOptionalPrincipalCurvature<OptionalCurvatureMesh::Vertex>);
    static_assert(meshv2::face::HasPrincipalCurvature<CurvatureMesh::Face>);
    static_assert(meshv2::face::HasOptionalPrincipalCurvature<CurvatureMesh::Face>);
    static_assert(!meshv2::face::HasPrincipalCurvature<MyMesh::Face>);
    static_assert(vcl::PrincipalCurvatureConcept<vcl::PrincipalCurvaturef>);
    static_assert(vcl::PrincipalCurvatureConcept<vcl::PrincipalCurvatured>);

    // Quality Component Concepts
    static_assert(meshv2::vert::HasQuality<QualityMesh::Vertex>);
    static_assert(meshv2::vert::HasQuality<QualityMesh::ConstVertex>);
    static_assert(meshv2::vert::HasQuality<QualityVertexComponents>);
    static_assert(!meshv2::vert::HasQuality<MyMesh::Vertex>);
    static_assert(!meshv2::vert::HasOptionalQuality<QualityVertexComponents>);
    static_assert(meshv2::vert::HasOptionalQuality<OptionalQualityVertexComponents>);
    static_assert(meshv2::vert::HasOptionalQuality<OptionalQualityMesh::Vertex>);
    static_assert(meshv2::face::HasQuality<QualityMesh::Face>);
    static_assert(meshv2::face::HasOptionalQuality<QualityMesh::Face>);
    static_assert(!meshv2::face::HasQuality<MyMesh::Face>);
    static_assert(std::is_same_v<QualityMesh::Vertex::QualityType, float>);
    static_assert(std::is_same_v<OptionalQualityMesh::Vertex::QualityType, double>);

    // Tangent Component Concepts
    static_assert(meshv2::vert::HasTangent<TangentMesh::Vertex>);
    static_assert(meshv2::vert::HasTangent<TangentMesh::ConstVertex>);
    static_assert(meshv2::vert::HasTangent<TangentVertexComponents>);
    static_assert(!meshv2::vert::HasTangent<MyMesh::Vertex>);
    static_assert(!meshv2::vert::HasOptionalTangent<TangentVertexComponents>);
    static_assert(meshv2::vert::HasOptionalTangent<OptionalTangentVertexComponents>);
    static_assert(meshv2::vert::HasOptionalTangent<OptionalTangentMesh::Vertex>);
    static_assert(meshv2::face::HasTangent<TangentMesh::Face>);
    static_assert(meshv2::face::HasOptionalTangent<TangentMesh::Face>);
    static_assert(!meshv2::face::HasTangent<MyMesh::Face>);
    static_assert(std::is_same_v<TangentMesh::Vertex::TangentType, Point3f>);
    static_assert(std::is_same_v<OptionalTangentMesh::Vertex::TangentType, Point3d>);
    static_assert(meshv2::comp::HasDefaultValue<meshv2::vert::Tangent3f>);

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

    // Adjacency Component Concepts
    static_assert(meshv2::HasAdjacentFaces<AdjFaceComponents>);
    static_assert(meshv2::HasAdjacentFaces<AdjMesh::Face>);
    static_assert(meshv2::face::HasTriangleAdjacentFaces<AdjFaceComponents>);
    static_assert(meshv2::face::HasTriangleAdjacentFaces<AdjMesh::Face>);
    static_assert(!meshv2::face::HasPolygonAdjacentFaces<AdjMesh::Face>);
    static_assert(!meshv2::face::HasOptionalAdjacentFaces<AdjMesh::Face>);

    static_assert(meshv2::HasAdjacentFaces<AdjVertexComponents>);
    static_assert(meshv2::HasAdjacentFaces<AdjMesh::Vertex>);
    static_assert(!meshv2::comp::HasStaticAdjacentFaces<AdjMesh::Vertex>);

    static_assert(meshv2::HasAdjacentVertices<AdjVertexComponents>);
    static_assert(meshv2::HasAdjacentVertices<AdjMesh::Vertex>);

    static_assert(meshv2::HasAdjacentEdges<AdjMesh::Vertex>);
    static_assert(meshv2::vert::HasOptionalAdjacentEdges<AdjMesh::Vertex>);
    static_assert(meshv2::HasAdjacentEdges<AdjMesh::Face>);
    static_assert(meshv2::face::HasOptionalAdjacentEdges<AdjMesh::Face>);

    static_assert(meshv2::HasVertexAdjacentFaces<AdjMesh>);
    static_assert(meshv2::HasFaceAdjacentFaces<AdjMesh>);
    static_assert(meshv2::HasVertexAdjacentVertices<AdjMesh>);
    static_assert(meshv2::HasVertexAdjacentEdges<AdjMesh>);
    static_assert(meshv2::HasFaceAdjacentEdges<AdjMesh>);
    static_assert(!meshv2::HasVertexAdjacentFaces<MyMesh>);
}

inline void testElementConcepts()
{
    // Base element classes satisfy ElementConcept
    static_assert(meshv2::ElementConcept<MyMesh::Vertex>);
    static_assert(meshv2::ElementConcept<MyMesh::ConstVertex>);
    static_assert(meshv2::ElementConcept<MyMesh::Face>);
    static_assert(meshv2::ElementConcept<MyMesh::ConstFace>);
    static_assert(meshv2::ElementConcept<EdgeMesh::Edge>);
    static_assert(meshv2::ElementConcept<EdgeMesh::ConstEdge>);

    // VertexConcept
    static_assert(meshv2::VertexConcept<MyMesh::Vertex>);
    static_assert(meshv2::VertexConcept<MyMesh::ConstVertex>);
    static_assert(!meshv2::VertexConcept<MyMesh::Face>);
    static_assert(!meshv2::VertexConcept<EdgeMesh::Edge>);

    // FaceConcept
    static_assert(meshv2::FaceConcept<MyMesh::Face>);
    static_assert(meshv2::FaceConcept<MyMesh::ConstFace>);
    static_assert(meshv2::FaceConcept<PolyMesh::Face>);
    static_assert(meshv2::FaceConcept<PolyMesh::ConstFace>);
    static_assert(meshv2::FaceConcept<QuadMesh::Face>);
    static_assert(meshv2::FaceConcept<QuadMesh::ConstFace>);
    static_assert(!meshv2::FaceConcept<MyMesh::Vertex>);
    static_assert(!meshv2::FaceConcept<EdgeMesh::Edge>);

    // TriangleFaceConcept
    static_assert(meshv2::TriangleFaceConcept<MyMesh::Face>);
    static_assert(meshv2::TriangleFaceConcept<MyMesh::ConstFace>);
    static_assert(!meshv2::TriangleFaceConcept<QuadMesh::Face>);
    static_assert(!meshv2::TriangleFaceConcept<PolyMesh::Face>);

    // QuadFaceConcept
    static_assert(meshv2::QuadFaceConcept<QuadMesh::Face>);
    static_assert(meshv2::QuadFaceConcept<QuadMesh::ConstFace>);
    static_assert(!meshv2::QuadFaceConcept<MyMesh::Face>);
    static_assert(!meshv2::QuadFaceConcept<PolyMesh::Face>);

    // PolygonFaceConcept
    static_assert(meshv2::PolygonFaceConcept<PolyMesh::Face>);
    static_assert(meshv2::PolygonFaceConcept<PolyMesh::ConstFace>);
    static_assert(!meshv2::PolygonFaceConcept<MyMesh::Face>);
    static_assert(!meshv2::PolygonFaceConcept<QuadMesh::Face>);

    // EdgeConcept
    static_assert(meshv2::EdgeConcept<EdgeMesh::Edge>);
    static_assert(meshv2::EdgeConcept<EdgeMesh::ConstEdge>);
    static_assert(!meshv2::EdgeConcept<MyMesh::Vertex>);
    static_assert(!meshv2::EdgeConcept<MyMesh::Face>);

    // Component presence concepts on elements (HasPosition, HasNormal, HasColor, etc.)
    // HasPosition
    static_assert(meshv2::HasPosition<MyMesh::Vertex>);
    static_assert(meshv2::HasPosition<MyMesh::ConstVertex>);
    static_assert(meshv2::HasPosition<MyVertexComponents>);
    static_assert(!meshv2::HasPosition<MyMesh::Face>);

    // HasNormal
    static_assert(meshv2::HasNormal<MyMesh::Vertex>);
    static_assert(meshv2::HasNormal<MyMesh::ConstVertex>);
    static_assert(!meshv2::HasNormal<SimpleMesh::Vertex>);
    static_assert(!meshv2::HasNormal<MyMesh::Face>);

    // HasColor
    static_assert(meshv2::HasColor<MyMesh::Vertex>);
    static_assert(meshv2::HasColor<MyMesh::Face>);
    static_assert(!meshv2::HasColor<SimpleMesh::Vertex>);

    // HasAdjacentFaces
    static_assert(meshv2::HasAdjacentFaces<AdjMesh::Vertex>);
    static_assert(meshv2::HasAdjacentFaces<AdjMesh::Face>);
    static_assert(!meshv2::HasAdjacentFaces<MyMesh::Vertex>);

    // HasAdjacentEdges
    static_assert(meshv2::HasAdjacentEdges<AdjMesh::Vertex>);
    static_assert(meshv2::HasAdjacentEdges<AdjMesh::Face>);
    static_assert(!meshv2::HasAdjacentEdges<MyMesh::Vertex>);

    // HasAdjacentVertices
    static_assert(meshv2::HasAdjacentVertices<AdjMesh::Vertex>);
    static_assert(!meshv2::HasAdjacentVertices<MyMesh::Vertex>);

    // HasBitFlags
    static_assert(meshv2::HasBitFlags<MyMesh::Vertex>);
    static_assert(meshv2::HasBitFlags<MyMesh::Face>);
    static_assert(meshv2::HasBitFlags<EdgeMesh::Edge>);

    // HasQuality
    static_assert(meshv2::HasQuality<QualityMesh::Vertex>);
    static_assert(meshv2::HasQuality<QualityMesh::Face>);
    static_assert(!meshv2::HasQuality<MyMesh::Vertex>);

    // HasPrincipalCurvature
    static_assert(meshv2::HasPrincipalCurvature<CurvatureMesh::Vertex>);
    static_assert(meshv2::HasPrincipalCurvature<CurvatureMesh::Face>);
    static_assert(!meshv2::HasPrincipalCurvature<MyMesh::Vertex>);

    // HasTangent
    static_assert(meshv2::HasTangent<TangentMesh::Vertex>);
    static_assert(meshv2::HasTangent<TangentMesh::Face>);
    static_assert(!meshv2::HasTangent<MyMesh::Vertex>);

    // HasMaterialIndex
    static_assert(meshv2::HasMaterialIndex<MaterialMesh::Face>);
    static_assert(!meshv2::HasMaterialIndex<MyMesh::Face>);

    // HasVertexReferences
    static_assert(meshv2::HasVertexReferences<MyMesh::Face>);
    static_assert(meshv2::HasVertexReferences<EdgeMesh::Edge>);
    static_assert(!meshv2::HasVertexReferences<MyMesh::Vertex>);

    // HasCustomComponents
    static_assert(meshv2::HasCustomComponents<CCMesh::Vertex>);
    static_assert(meshv2::HasCustomComponents<CCMesh::Face>);
    static_assert(!meshv2::HasCustomComponents<MyMesh::Vertex>);
}


inline void testMeshConcepts()
{
    // Base Mesh Structure & Container Presence
    static_assert(meshv2::MeshConcept<MyMesh>);
    static_assert(meshv2::HasContainerOf<MyMesh, meshv2::ElemId::VERTEX>);
    static_assert(meshv2::HasContainerOf<MyMesh, meshv2::ElemId::FACE>);
    static_assert(!meshv2::HasContainerOf<MyMesh, meshv2::ElemId::EDGE>);
    static_assert(meshv2::HasFaces<MyMesh>);
    static_assert(!meshv2::HasEdges<MyMesh>);
    static_assert(meshv2::FaceMeshConcept<MyMesh>);
    static_assert(!meshv2::EdgeMeshConcept<MyMesh>);

    static_assert(!meshv2::HasFaces<EdgeMesh>);
    static_assert(meshv2::HasEdges<EdgeMesh>);
    static_assert(!meshv2::FaceMeshConcept<EdgeMesh>);
    static_assert(meshv2::EdgeMeshConcept<EdgeMesh>);

    static_assert(meshv2::HasFaces<GraphMesh>);
    static_assert(meshv2::HasEdges<GraphMesh>);
    static_assert(meshv2::FaceMeshConcept<GraphMesh>);
    static_assert(meshv2::EdgeMeshConcept<GraphMesh>);

    static_assert(meshv2::MeshConcept<PointCloudMesh>);
    static_assert(!meshv2::HasFaces<PointCloudMesh>);
    static_assert(!meshv2::HasEdges<PointCloudMesh>);
    static_assert(!meshv2::FaceMeshConcept<PointCloudMesh>);
    static_assert(!meshv2::EdgeMeshConcept<PointCloudMesh>);

    // Face Topology Specializations
    static_assert(meshv2::HasTriangles<MyMesh>);
    static_assert(!meshv2::HasQuads<MyMesh>);
    static_assert(!meshv2::HasPolygons<MyMesh>);
    static_assert(meshv2::TriMeshConcept<MyMesh>);
    static_assert(!meshv2::QuadMeshConcept<MyMesh>);
    static_assert(!meshv2::PolyMeshConcept<MyMesh>);

    static_assert(!meshv2::HasTriangles<QuadMesh>);
    static_assert(meshv2::HasQuads<QuadMesh>);
    static_assert(!meshv2::HasPolygons<QuadMesh>);
    static_assert(!meshv2::TriMeshConcept<QuadMesh>);
    static_assert(meshv2::QuadMeshConcept<QuadMesh>);
    static_assert(!meshv2::PolyMeshConcept<QuadMesh>);

    static_assert(!meshv2::HasTriangles<PolyMesh>);
    static_assert(!meshv2::HasQuads<PolyMesh>);
    static_assert(meshv2::HasPolygons<PolyMesh>);
    static_assert(!meshv2::TriMeshConcept<PolyMesh>);
    static_assert(!meshv2::QuadMeshConcept<PolyMesh>);
    static_assert(meshv2::PolyMeshConcept<PolyMesh>);

    static_assert(!meshv2::TriMeshConcept<PointCloudMesh>);
    static_assert(!meshv2::TriMeshConcept<EdgeMesh>);

    // Element Components on Mesh
    // Normals
    static_assert(meshv2::HasVertexNormals<FullMesh>);
    static_assert(meshv2::HasOptionalVertexNormals<MyMesh>);
    static_assert(!meshv2::HasVertexNormals<SimpleMesh>);
    static_assert(!meshv2::HasFaceNormals<MyMesh>);
    static_assert(meshv2::HasFaceNormals<TangentMesh>);

    // Colors
    static_assert(meshv2::HasVertexColors<MyMesh>);
    static_assert(meshv2::HasOptionalVertexColors<MyMesh>);
    static_assert(meshv2::HasFaceColors<MyMesh>);
    static_assert(meshv2::HasOptionalFaceColors<MyMesh>);
    static_assert(meshv2::HasVertexColors<FullMesh>);
    static_assert(!meshv2::HasOptionalVertexColors<FullMesh>);
    static_assert(meshv2::HasFaceColors<FullMesh>);
    static_assert(!meshv2::HasOptionalFaceColors<FullMesh>);
    static_assert(!meshv2::HasVertexColors<SimpleMesh>);
    static_assert(meshv2::HasFaceColors<SimpleMesh>);
    static_assert(meshv2::HasOptionalFaceColors<SimpleMesh>);
    static_assert(!meshv2::HasFaceColors<PolyMesh>);
    static_assert(!meshv2::HasFaceColors<QuadMesh>);
    static_assert(!meshv2::HasFaceColors<EdgeMesh>);
    static_assert(meshv2::HasEdgeColors<EdgeMesh>);
    static_assert(!meshv2::HasOptionalEdgeColors<EdgeMesh>);
    static_assert(!meshv2::HasEdgeColors<MyMesh>);

    // Adjacencies
    static_assert(meshv2::HasVertexAdjacentFaces<AdjMesh>);
    static_assert(meshv2::HasFaceAdjacentFaces<AdjMesh>);
    static_assert(meshv2::HasVertexAdjacentVertices<AdjMesh>);
    static_assert(meshv2::HasVertexAdjacentEdges<AdjMesh>);
    static_assert(meshv2::HasFaceAdjacentEdges<AdjMesh>);
    static_assert(!meshv2::HasVertexAdjacentFaces<MyMesh>);
    static_assert(!meshv2::HasFaceAdjacentFaces<MyMesh>);
    static_assert(!meshv2::HasVertexAdjacentEdges<MyMesh>);

    // Qualities
    static_assert(meshv2::HasVertexQualities<QualityMesh>);
    static_assert(meshv2::HasFaceQualities<QualityMesh>);
    static_assert(!meshv2::HasOptionalVertexQualities<QualityMesh>);
    static_assert(meshv2::HasOptionalFaceQualities<QualityMesh>);
    static_assert(meshv2::HasOptionalVertexQualities<OptionalQualityMesh>);
    static_assert(!meshv2::HasVertexQualities<MyMesh>);

    // Curvatures
    static_assert(meshv2::HasVertexPrincipalCurvatures<CurvatureMesh>);
    static_assert(meshv2::HasFacePrincipalCurvatures<CurvatureMesh>);
    static_assert(!meshv2::HasOptionalVertexPrincipalCurvatures<CurvatureMesh>);
    static_assert(meshv2::HasOptionalFacePrincipalCurvatures<CurvatureMesh>);
    static_assert(meshv2::HasOptionalVertexPrincipalCurvatures<OptionalCurvatureMesh>);
    static_assert(!meshv2::HasVertexPrincipalCurvatures<MyMesh>);

    // Tangents
    static_assert(meshv2::HasVertexTangents<TangentMesh>);
    static_assert(meshv2::HasFaceTangents<TangentMesh>);
    static_assert(!meshv2::HasOptionalVertexTangents<TangentMesh>);
    static_assert(meshv2::HasOptionalFaceTangents<TangentMesh>);
    static_assert(meshv2::HasOptionalVertexTangents<OptionalTangentMesh>);
    static_assert(!meshv2::HasVertexTangents<MyMesh>);

    // Material Index
    static_assert(meshv2::HasFaceMaterialIndices<MaterialMesh>);
    static_assert(!meshv2::HasFaceMaterialIndices<MyMesh>);
    static_assert(meshv2::HasOptionalFaceMaterialIndices<OptionalMaterialMesh>);

    // Custom Components
    static_assert(meshv2::HasVertexCustomComponents<CCMesh>);
    static_assert(meshv2::HasFaceCustomComponents<CCMesh>);
    static_assert(meshv2::HasEdgeCustomComponents<EdgeMeshWithCC>);
    static_assert(!meshv2::HasVertexCustomComponents<MyMesh>);
    static_assert(!meshv2::HasEdgeCustomComponents<MyMesh>);
    static_assert(!meshv2::HasFaceCustomComponents<MyMesh>);

    // Mesh-Level Components
    static_assert(meshv2::HasTransformMatrix<TransformedMesh>);
    static_assert(!meshv2::HasTransformMatrix<MyMesh>);
    static_assert(meshv2::HasBoundingBox<BoundingBoxMesh>);
    static_assert(!meshv2::HasBoundingBox<MyMesh>);
    static_assert(meshv2::HasMaterials<MaterialMeshWithMaterials>);
    static_assert(!meshv2::HasMaterials<MyMesh>);
    static_assert(meshv2::HasName<NamedMesh>);
    static_assert(!meshv2::HasName<MyMesh>);
    static_assert(meshv2::HasMeshColor<MyMesh>);
    static_assert(!meshv2::HasMeshColor<SimpleMesh>);
    static_assert(meshv2::HasMeshCustomComponents<CCMesh>);
    static_assert(!meshv2::HasMeshCustomComponents<MyMesh>);

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
    static_assert(std::random_access_iterator<
        vcl::meshv2::ComponentIterator<vcl::meshv2::CompId::QUALITY, QualityMesh::VertexContainer>>);
    static_assert(std::random_access_iterator<
        vcl::meshv2::ComponentIterator<vcl::meshv2::CompId::TANGENT, TangentMesh::VertexContainer>>);

    // Random access ranges
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().vertices())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().faces())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<MyMesh>().vertices().positions())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<QualityMesh>().vertices().qualities())>);
    static_assert(std::ranges::random_access_range<decltype(std::declval<TangentMesh>().vertices().tangents())>);

    // Views over elements and components
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u))>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u) | vcl::meshv2::views::positions)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<MyMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u) | vcl::meshv2::views::normals)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<QualityMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u) | vcl::meshv2::views::qualities)>);
    static_assert(std::ranges::random_access_range<
        decltype(std::declval<TangentMesh>().elements<vcl::meshv2::ElemId::VERTEX>(0u) | vcl::meshv2::views::tangents)>);
}

inline void testContracts()
{
    // 1. Compactness tests
    MyMesh m = createSampleQuadMesh();
    assert(m.isCompact());
    assert(m.deletedCount<meshv2::ElemId::VERTEX>() == 0);
    assert(m.deletedCount<meshv2::ElemId::FACE>() == 0);
    meshv2::requireCompactness(m);
    meshv2::requireVertexCompactness(m);
    meshv2::requireFaceCompactness(m);

    // Mark a face as deleted
    m.deleteFace(0);
    assert(!m.isCompact());
    assert(m.deletedCount<meshv2::ElemId::FACE>() != 0);
    assert(m.deletedCount<meshv2::ElemId::VERTEX>() == 0); // vertices are still compact

    bool caught = false;
    try {
        meshv2::requireFaceCompactness(m);
    } catch (const meshv2::MissingCompactnessException&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        meshv2::requireCompactness(m);
    } catch (const meshv2::MissingCompactnessException&) {
        caught = true;
    }
    assert(caught);

    // Compact the mesh
    m.compact();
    assert(m.isCompact());
    meshv2::requireCompactness(m);

    // 2. Topology tests
    assert(meshv2::isTriangleMesh(m));
    assert(!meshv2::isQuadMesh(m));
    meshv2::requireTriangleMesh(m);

    QuadMesh qm;
    assert(meshv2::isQuadMesh(qm));
    assert(!meshv2::isTriangleMesh(qm));
    meshv2::requireQuadMesh(qm);

    caught = false;
    try {
        meshv2::requireQuadMesh(m);
    } catch (const meshv2::MissingQuadRequirementException&) {
        caught = true;
    }
    assert(caught);

    // 3. Component availability & enablement
    MyMesh optMesh;
    optMesh.addVertices(3);
    optMesh.addFace(0, 1, 2);

    // Optional vertex normals start disabled
    assert((!optMesh.isElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    caught = false;
    try {
        meshv2::requireVertexNormals(optMesh);
    } catch (const meshv2::MissingComponentException&) {
        caught = true;
    }
    assert(caught);

    // Enable optional vertex normals
    bool enabled = meshv2::enableIfVertexNormalsOptional(optMesh);
    assert(enabled);
    assert((optMesh.isElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    meshv2::requireVertexNormals(optMesh); // succeeds without throwing

    // Optional face color starts disabled
    assert((!optMesh.isElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
    caught = false;
    try {
        meshv2::requireFaceColors(optMesh);
    } catch (const meshv2::MissingComponentException&) {
        caught = true;
    }
    assert(caught);

    meshv2::enableIfFaceColorsOptional(optMesh);
    assert((optMesh.isElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
    meshv2::requireFaceColors(optMesh);

    // Non-optional components on FullMesh are always available
    FullMesh fullMesh;
    assert((fullMesh.isElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((fullMesh.isElementComponentAvailable<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((fullMesh.isElementComponentAvailable<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
    meshv2::requireVertexNormals(fullMesh);
    meshv2::requireVertexColors(fullMesh);
    meshv2::requireFaceColors(fullMesh);

    // EdgeMesh
    EdgeMesh edgeMesh = createSampleEdgeMesh();
    assert((edgeMesh.isElementComponentAvailable<meshv2::ElemId::EDGE, meshv2::CompId::COLOR>()));
    assert((!edgeMesh.isElementComponentAvailable<meshv2::ElemId::EDGE, meshv2::CompId::QUALITY>())); // optional quality not enabled
    meshv2::enableIfEdgeQualitiesOptional(edgeMesh);
    assert((edgeMesh.isElementComponentAvailable<meshv2::ElemId::EDGE, meshv2::CompId::QUALITY>()));
    meshv2::requireEdgeQualities(edgeMesh);

    // Generic requireElementComponent
    meshv2::requireElementComponent<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>(optMesh);
}

inline void testConcepts()
{
    std::println("--- Testing Concepts & Strong Typing ---");
    testComponentConcepts();
    testElementConcepts();
    testMeshConcepts();
    testIteratorConcepts();
    testContracts();
    std::println("Compile-time concepts & runtime contracts verified successfully!\n");
}

#endif // TEST_CONCEPTS_H
