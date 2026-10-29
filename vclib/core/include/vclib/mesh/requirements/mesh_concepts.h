// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements/element_concepts.h>

/**
 * @defgroup mesh_concepts Mesh Concepts
 * @ingroup mesh_requirements
 *
 * @brief Concepts that check the structure, topology, and component availability
 * of Mesh classes.
 */

namespace vcl {

/* =========================================================================
 * Base Mesh Structure & Container Presence
 *
 * MeshConcept<M> is defined in <vclib/mesh/mesh.h> as the primary identity
 * concept for any Mesh class.
 * ========================================================================= */

/**
 * @brief Concept that checks whether a Mesh has an ElementContainer of type ELEM_ID.
 * @tparam MeshType: The mesh type to check.
 * @tparam ELEM_ID: The element ID.
 * @ingroup mesh_concepts
 */
template<typename MeshType, uint ELEM_ID>
concept HasContainerOf = requires {
    requires std::remove_cvref_t<MeshType>::template hasContainerOf<ELEM_ID>();
};

/**
 * @brief Concept that checks whether a Mesh has a FaceContainer.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasFaces = HasContainerOf<MeshType, ElemId::FACE>;

/**
 * @brief Concept that checks whether a Mesh has an EdgeContainer.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasEdges = HasContainerOf<MeshType, ElemId::EDGE>;

/**
 * @brief Concept that checks whether a type is a Mesh with faces.
 * @ingroup mesh_concepts
 */
template<typename M>
concept FaceMeshConcept = MeshConcept<M> && HasFaces<M>;

/**
 * @brief Concept that checks whether a type is a Mesh with edges.
 * @ingroup mesh_concepts
 */
template<typename M>
concept EdgeMeshConcept = MeshConcept<M> && HasEdges<M>;

/* =========================================================================
 * Face Topology Specializations
 * ========================================================================= */

/**
 * @brief Concept that checks whether a Mesh has triangular faces.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTriangles = HasFaces<MeshType> && requires {
    typename std::remove_cvref_t<MeshType>::Face;
    requires TriangleFaceConcept<typename std::remove_cvref_t<MeshType>::Face>;
};

/**
 * @brief Concept that checks whether a Mesh has quadrilateral faces.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasQuads = HasFaces<MeshType> && requires {
    typename std::remove_cvref_t<MeshType>::Face;
    requires QuadFaceConcept<typename std::remove_cvref_t<MeshType>::Face>;
};

/**
 * @brief Concept that checks whether a Mesh has general polygonal faces.
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasPolygons = HasFaces<MeshType> && requires {
    typename std::remove_cvref_t<MeshType>::Face;
    requires PolygonFaceConcept<typename std::remove_cvref_t<MeshType>::Face>;
};

/**
 * @brief Concept that checks whether a type is a triangle Mesh.
 * @ingroup mesh_concepts
 */
template<typename M>
concept TriangleMeshConcept = FaceMeshConcept<M> && HasTriangles<M>;

/**
 * @brief Concept that checks whether a type is a quadrilateral Mesh.
 * @ingroup mesh_concepts
 */
template<typename M>
concept QuadMeshConcept = FaceMeshConcept<M> && HasQuads<M>;

/**
 * @brief Concept that checks whether a type is a general polygon Mesh.
 * @ingroup mesh_concepts
 */
template<typename M>
concept PolygonMeshConcept = FaceMeshConcept<M> && HasPolygons<M>;

/* =========================================================================
 * Element Component Presence on Mesh
 * ========================================================================= */

/**
 * @brief Concept that checks whether a Mesh has a container for the given
 * ELEM_ID with the given component COMP_ID.
 * @ingroup mesh_concepts
 */
template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasElementComponent =
    HasContainerOf<MeshType, ELEM_ID> &&
    comp::HasComponent<
        COMP_ID,
        typename std::remove_cvref_t<MeshType>::template ContainerType<
            ELEM_ID>>;

/**
 * @brief Concept that checks whether a Mesh has a container for the given
 * ELEM_ID with the given optional component COMP_ID.
 * @ingroup mesh_concepts
 */
template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasElementOptionalComponent =
    HasContainerOf<MeshType, ELEM_ID> &&
    comp::HasOptionalComponent<
        COMP_ID,
        typename std::remove_cvref_t<MeshType>::template ContainerType<
            ELEM_ID>>;

/* Normals */
template<typename M>
concept HasVertexNormals =
    HasElementComponent<M, ElemId::VERTEX, CompId::NORMAL>;

template<typename M>
concept HasOptionalVertexNormals =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::NORMAL>;

template<typename M>
concept HasFaceNormals =
    HasElementComponent<M, ElemId::FACE, CompId::NORMAL>;

template<typename M>
concept HasOptionalFaceNormals =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::NORMAL>;

template<typename M>
concept HasEdgeNormals =
    HasElementComponent<M, ElemId::EDGE, CompId::NORMAL>;

template<typename M>
concept HasOptionalEdgeNormals =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::NORMAL>;

/* Colors */
template<typename M>
concept HasVertexColors =
    HasElementComponent<M, ElemId::VERTEX, CompId::COLOR>;

template<typename M>
concept HasOptionalVertexColors =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::COLOR>;

template<typename M>
concept HasFaceColors =
    HasElementComponent<M, ElemId::FACE, CompId::COLOR>;

template<typename M>
concept HasOptionalFaceColors =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::COLOR>;

template<typename M>
concept HasEdgeColors =
    HasElementComponent<M, ElemId::EDGE, CompId::COLOR>;

template<typename M>
concept HasOptionalEdgeColors =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::COLOR>;

/* TexCoords */
template<typename M>
concept HasVertexTexCoords =
    HasElementComponent<M, ElemId::VERTEX, CompId::TEX_COORD>;

template<typename M>
concept HasOptionalVertexTexCoords =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::TEX_COORD>;

/* Wedges */
template<typename M>
concept HasFaceWedgeColors =
    HasElementComponent<M, ElemId::FACE, CompId::WEDGE_COLORS>;

template<typename M>
concept HasOptionalFaceWedgeColors =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::WEDGE_COLORS>;

template<typename M>
concept HasFaceWedgeTexCoords =
    HasElementComponent<M, ElemId::FACE, CompId::WEDGE_TEX_COORDS>;

template<typename M>
concept HasOptionalFaceWedgeTexCoords =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::WEDGE_TEX_COORDS>;

template<typename M>
concept HasFaceWedgeNormals =
    HasElementComponent<M, ElemId::FACE, CompId::WEDGE_NORMALS>;

template<typename M>
concept HasOptionalFaceWedgeNormals =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::WEDGE_NORMALS>;

template<typename M>
concept HasFaceWedges =
    HasFaceWedgeColors<M> || HasFaceWedgeTexCoords<M> || HasFaceWedgeNormals<M>;

/* Adjacencies */
template<typename M, uint ELEM_ID>
concept HasElementAdjacentFaces =
    HasElementComponent<M, ELEM_ID, CompId::ADJACENT_FACES>;

template<typename M, uint ELEM_ID>
concept HasElementOptionalAdjacentFaces =
    HasElementOptionalComponent<M, ELEM_ID, CompId::ADJACENT_FACES>;

template<typename M>
concept HasVertexAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::VERTEX>;

template<typename M>
concept HasFaceAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::FACE>;

template<typename M>
concept HasEdgeAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::EDGE>;

template<typename M, uint ELEM_ID>
concept HasElementAdjacentEdges =
    HasElementComponent<M, ELEM_ID, CompId::ADJACENT_EDGES>;

template<typename M, uint ELEM_ID>
concept HasElementOptionalAdjacentEdges =
    HasElementOptionalComponent<M, ELEM_ID, CompId::ADJACENT_EDGES>;

template<typename M>
concept HasVertexAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::VERTEX>;

template<typename M>
concept HasFaceAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::FACE>;

template<typename M>
concept HasEdgeAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::EDGE>;

template<typename M, uint ELEM_ID>
concept HasElementAdjacentVertices =
    HasElementComponent<M, ELEM_ID, CompId::ADJACENT_VERTICES>;

template<typename M, uint ELEM_ID>
concept HasElementOptionalAdjacentVertices =
    HasElementOptionalComponent<M, ELEM_ID, CompId::ADJACENT_VERTICES>;

template<typename M>
concept HasVertexAdjacentVertices =
    HasElementAdjacentVertices<M, ElemId::VERTEX>;

template<typename M>
concept HasFaceAdjacentVertices =
    HasElementAdjacentVertices<M, ElemId::FACE>;

template<typename M>
concept HasEdgeAdjacentVertices =
    HasElementAdjacentVertices<M, ElemId::EDGE>;

/* Quality */
template<typename M>
concept HasVertexQualities =
    HasElementComponent<M, ElemId::VERTEX, CompId::QUALITY>;

template<typename M>
concept HasOptionalVertexQualities =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::QUALITY>;

template<typename M>
concept HasFaceQualities =
    HasElementComponent<M, ElemId::FACE, CompId::QUALITY>;

template<typename M>
concept HasOptionalFaceQualities =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::QUALITY>;

template<typename M>
concept HasEdgeQualities =
    HasElementComponent<M, ElemId::EDGE, CompId::QUALITY>;

template<typename M>
concept HasOptionalEdgeQualities =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::QUALITY>;

/* PrincipalCurvature */
template<typename M>
concept HasVertexPrincipalCurvatures =
    HasElementComponent<M, ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>;

template<typename M>
concept HasOptionalVertexPrincipalCurvatures =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>;

template<typename M>
concept HasFacePrincipalCurvatures =
    HasElementComponent<M, ElemId::FACE, CompId::PRINCIPAL_CURVATURE>;

template<typename M>
concept HasOptionalFacePrincipalCurvatures =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::PRINCIPAL_CURVATURE>;

/* Tangent */
template<typename M>
concept HasVertexTangents =
    HasElementComponent<M, ElemId::VERTEX, CompId::TANGENT>;

template<typename M>
concept HasOptionalVertexTangents =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::TANGENT>;

template<typename M>
concept HasFaceTangents =
    HasElementComponent<M, ElemId::FACE, CompId::TANGENT>;

template<typename M>
concept HasOptionalFaceTangents =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::TANGENT>;

/* MaterialIndex */
template<typename M>
concept HasVertexMaterialIndices =
    HasElementComponent<M, ElemId::VERTEX, CompId::MATERIAL_INDEX>;

template<typename M>
concept HasOptionalVertexMaterialIndices =
    HasElementOptionalComponent<M, ElemId::VERTEX, CompId::MATERIAL_INDEX>;

template<typename M>
concept HasFaceMaterialIndices =
    HasElementComponent<M, ElemId::FACE, CompId::MATERIAL_INDEX>;

template<typename M>
concept HasOptionalFaceMaterialIndices =
    HasElementOptionalComponent<M, ElemId::FACE, CompId::MATERIAL_INDEX>;

/* Custom Components on Elements */
template<typename M, uint ELEM_ID>
concept HasElementCustomComponents =
    HasElementComponent<M, ELEM_ID, CompId::CUSTOM_COMPONENTS>;

template<typename M>
concept HasVertexCustomComponents =
    HasElementCustomComponents<M, ElemId::VERTEX>;

template<typename M>
concept HasFaceCustomComponents =
    HasElementCustomComponents<M, ElemId::FACE>;

template<typename M>
concept HasEdgeCustomComponents =
    HasElementCustomComponents<M, ElemId::EDGE>;

/* =========================================================================
 * Mesh-Level Components
 * ========================================================================= */

template<typename M>
concept HasTransformMatrix = mesh::HasTransformMatrix<M>;

template<typename M>
concept HasBoundingBox = mesh::HasBoundingBox<M>;

template<typename M>
concept HasMaterials = mesh::HasMaterials<M>;

template<typename M>
concept HasName = mesh::HasName<M>;

template<typename M>
concept HasMeshColor = mesh::HasColor<M>;

template<typename M>
concept HasMeshCustomComponents = mesh::HasCustomComponents<M>;

/* =========================================================================
 * Mesh Ranges Utility Concepts
 * ========================================================================= */

/**
 * @brief A concept that checks whether a type is a range of Meshes.
 *
 * The concept is satisfied when `T` is a range and the value type of the
 * range satisfies the MeshConcept.
 *
 * @tparam T: The type to be tested for conformity to the RangeOfMeshes concept.
 *
 * @ingroup mesh
 * @ingroup mesh_concepts
 */
template<typename T>
concept RangeOfMeshes =
    std::ranges::range<T> && MeshConcept<std::ranges::range_value_t<T>>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_H
