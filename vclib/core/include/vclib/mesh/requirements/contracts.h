// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_CONTRACTS_H
#define VCL_MESH_REQUIREMENTS_CONTRACTS_H

#include <vclib/mesh/components/base/base.h>
#include <vclib/mesh/elements/base/base.h>
#include <vclib/mesh/exceptions.h>
#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements/mesh_concepts.h>

/**
 * @defgroup mesh_contracts Mesh Runtime & Compile-time Contracts
 * @ingroup mesh_requirements
 *
 * @brief Free functions for asserting container compactness, checking/asserting
 * mesh topology, and enabling/asserting component availability at compile-time and run-time.
 */

namespace vcl {

/* =========================================================================
 * 1. Compactness Requirements
 * ========================================================================= */

/**
 * @brief Asserts that the mesh is compact (all containers have no deleted elements).
 * Throws MissingCompactnessException if the mesh is not compact.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @throws MissingCompactnessException
 * @ingroup mesh_contracts
 */
template<MeshConcept MeshType>
void requireCompactness(const MeshType& m)
{
    if (!m.isCompact()) {
        throw MissingCompactnessException("Mesh is not compact.");
    }
}

/**
 * @brief Asserts that the container of elements with ID ELEM_ID is compact.
 * Throws MissingCompactnessException if the container is not compact.
 * @tparam ELEM_ID: The element ID.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @throws MissingCompactnessException
 * @ingroup mesh_contracts
 */
template<uint ELEM_ID, MeshConcept MeshType>
requires (HasContainerOf<MeshType, ELEM_ID>)
void requireElementCompactness(const MeshType& m)
{
    if (m.template deletedCount<ELEM_ID>() != 0) {
        throw MissingCompactnessException(
            elementEnumString<ELEM_ID>() + " container of the Mesh is not compact.");
    }
}

template<MeshConcept MeshType>
void requireVertexCompactness(const MeshType& m)
{
    requireElementCompactness<ElemId::VERTEX>(m);
}

template<MeshConcept MeshType>
requires (HasFaces<MeshType>)
void requireFaceCompactness(const MeshType& m)
{
    requireElementCompactness<ElemId::FACE>(m);
}

template<MeshConcept MeshType>
requires (HasEdges<MeshType>)
void requireEdgeCompactness(const MeshType& m)
{
    requireElementCompactness<ElemId::EDGE>(m);
}

/* =========================================================================
 * 2. Mesh Topology Checks and Requirements
 * ========================================================================= */

/**
 * @brief Checks at compile-time and run-time if the mesh is composed of triangles.
 * For static triangle meshes, returns true at compile time.
 * For general polygon meshes, checks at runtime if every face has 3 vertices.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @return true if every face has 3 vertices.
 * @ingroup mesh_contracts
 */
template<MeshConcept MeshType>
bool isTriangleMesh(const MeshType& m)
{
    if constexpr (HasTriangles<MeshType>) {
        return true;
    }
    else if constexpr (HasPolygons<MeshType>) {
        for (const auto& f : m.faces()) {
            if (f.vertexCount() != 3) {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Asserts that the mesh is composed of triangles.
 * Throws MissingTriangularRequirementException if any face has vertexCount != 3.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @throws MissingTriangularRequirementException
 * @ingroup mesh_contracts
 */
template<FaceMeshConcept MeshType>
void requireTriangleMesh(const MeshType& m)
{
    if (!isTriangleMesh(m)) {
        throw MissingTriangularRequirementException("Triangle Mesh Required.");
    }
}

/**
 * @brief Checks at compile-time and run-time if the mesh is composed of quads.
 * For static quad meshes, returns true at compile time.
 * For general polygon meshes, checks at runtime if every face has 4 vertices.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @return true if every face has 4 vertices.
 * @ingroup mesh_contracts
 */
template<MeshConcept MeshType>
bool isQuadMesh(const MeshType& m)
{
    if constexpr (HasQuads<MeshType>) {
        return true;
    }
    else if constexpr (HasPolygons<MeshType>) {
        for (const auto& f : m.faces()) {
            if (f.vertexCount() != 4) {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Asserts that the mesh is composed of quads.
 * Throws MissingQuadRequirementException if any face has vertexCount != 4.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @throws MissingQuadRequirementException
 * @ingroup mesh_contracts
 */
template<FaceMeshConcept MeshType>
void requireQuadMesh(const MeshType& m)
{
    if (!isQuadMesh(m)) {
        throw MissingQuadRequirementException("Quad Mesh Required.");
    }
}

/* =========================================================================
 * 3. Element Component Enablement & Generic Require
 * ========================================================================= */

/**
 * @brief Enables the given optional Component COMP_ID in element ELEM_ID of mesh m,
 * and returns true if it is available after the call.
 * If the component is non-optional, returns true immediately.
 * If the mesh or element does not have the component, returns false.
 * @tparam ELEM_ID: The element ID.
 * @tparam COMP_ID: The component ID.
 * @tparam MeshType: The mesh type to check.
 * @param[in,out] m: The mesh instance.
 * @return true if the component is available after the call.
 * @ingroup mesh_contracts
 */
template<uint ELEM_ID, uint COMP_ID, MeshConcept MeshType>
bool enableIfElementComponentOptional(MeshType& m)
{
    if constexpr (HasElementOptionalComponent<MeshType, ELEM_ID, COMP_ID>) {
        m.template enableElementComponent<ELEM_ID, COMP_ID>();
        return true;
    }
    else if constexpr (HasElementComponent<MeshType, ELEM_ID, COMP_ID>) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Asserts that the mesh has the element ELEM_ID with component COMP_ID,
 * and that it is available (enabled) at runtime.
 * Throws MissingComponentException if the component is optional and disabled.
 * @tparam ELEM_ID: The element ID.
 * @tparam COMP_ID: The component ID.
 * @tparam MeshType: The mesh type to check.
 * @param[in] m: The mesh instance.
 * @throws MissingComponentException
 * @ingroup mesh_contracts
 */
template<uint ELEM_ID, uint COMP_ID, MeshConcept MeshType>
requires (HasElementComponent<MeshType, ELEM_ID, COMP_ID>)
void requireElementComponent(const MeshType& m)
{
    if (!m.template isElementComponentAvailable<ELEM_ID, COMP_ID>()) {
        throw MissingComponentException(
            elementEnumString<ELEM_ID>() + " " +
            std::string(componentEnumString<COMP_ID>()) +
            " Component is not enabled.");
    }
}

/* =========================================================================
 * 4. Specific Speaking Contract Functions for Components
 * ========================================================================= */

/* --- Normals --- */

template<MeshConcept MeshType>
bool enableIfVertexNormalsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::NORMAL>(m);
}

template<MeshConcept MeshType>
requires (HasVertexNormals<MeshType>)
void requireVertexNormals(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::NORMAL>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceNormalsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::NORMAL>(m);
}

template<MeshConcept MeshType>
requires (HasFaceNormals<MeshType>)
void requireFaceNormals(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::NORMAL>(m);
}

template<MeshConcept MeshType>
bool enableIfEdgeNormalsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::EDGE, CompId::NORMAL>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeNormals<MeshType>)
void requireEdgeNormals(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::NORMAL>(m);
}

/* --- Colors --- */

template<MeshConcept MeshType>
bool enableIfVertexColorsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::COLOR>(m);
}

template<MeshConcept MeshType>
requires (HasVertexColors<MeshType>)
void requireVertexColors(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::COLOR>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceColorsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::COLOR>(m);
}

template<MeshConcept MeshType>
requires (HasFaceColors<MeshType>)
void requireFaceColors(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::COLOR>(m);
}

template<MeshConcept MeshType>
bool enableIfEdgeColorsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::EDGE, CompId::COLOR>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeColors<MeshType>)
void requireEdgeColors(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::COLOR>(m);
}

/* --- TexCoords --- */

template<MeshConcept MeshType>
bool enableIfVertexTexCoordsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::TEX_COORD>(m);
}

template<MeshConcept MeshType>
requires (HasVertexTexCoords<MeshType>)
void requireVertexTexCoords(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::TEX_COORD>(m);
}

/* --- Wedges (Face) --- */

template<MeshConcept MeshType>
bool enableIfFaceWedgeColorsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::WEDGE_COLORS>(m);
}

template<MeshConcept MeshType>
requires (HasFaceWedgeColors<MeshType>)
void requireFaceWedgeColors(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::WEDGE_COLORS>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceWedgeTexCoordsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::WEDGE_TEX_COORDS>(m);
}

template<MeshConcept MeshType>
requires (HasFaceWedgeTexCoords<MeshType>)
void requireFaceWedgeTexCoords(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::WEDGE_TEX_COORDS>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceWedgeNormalsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::WEDGE_NORMALS>(m);
}

template<MeshConcept MeshType>
requires (HasFaceWedgeNormals<MeshType>)
void requireFaceWedgeNormals(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::WEDGE_NORMALS>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceWedgesOptional(MeshType& m)
{
    if constexpr (HasFaceWedges<MeshType>) {
        bool ok = true;
        if constexpr (HasFaceWedgeColors<MeshType>) {
            ok = ok && enableIfFaceWedgeColorsOptional(m);
        }
        if constexpr (HasFaceWedgeTexCoords<MeshType>) {
            ok = ok && enableIfFaceWedgeTexCoordsOptional(m);
        }
        if constexpr (HasFaceWedgeNormals<MeshType>) {
            ok = ok && enableIfFaceWedgeNormalsOptional(m);
        }
        return ok;
    }
    else {
        return false;
    }
}

template<MeshConcept MeshType>
requires (HasFaceWedges<MeshType>)
void requireFaceWedges(const MeshType& m)
{
    if constexpr (HasFaceWedgeColors<MeshType>) {
        requireFaceWedgeColors(m);
    }
    if constexpr (HasFaceWedgeTexCoords<MeshType>) {
        requireFaceWedgeTexCoords(m);
    }
    if constexpr (HasFaceWedgeNormals<MeshType>) {
        requireFaceWedgeNormals(m);
    }
}

/* --- Adjacencies: AdjacentFaces --- */

template<MeshConcept MeshType>
bool enableIfVertexAdjacentFacesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
requires (HasVertexAdjacentFaces<MeshType>)
void requireVertexAdjacentFaces(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceAdjacentFacesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
requires (HasFaceAdjacentFaces<MeshType>)
void requireFaceAdjacentFaces(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
bool enableIfEdgeAdjacentFacesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::EDGE, CompId::ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeAdjacentFaces<MeshType>)
void requireEdgeAdjacentFaces(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::ADJACENT_FACES>(m);
}

/* --- Adjacencies: AdjacentEdges --- */

template<MeshConcept MeshType>
bool enableIfVertexAdjacentEdgesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::ADJACENT_EDGES>(m);
}

template<MeshConcept MeshType>
requires (HasVertexAdjacentEdges<MeshType>)
void requireVertexAdjacentEdges(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::ADJACENT_EDGES>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceAdjacentEdgesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::ADJACENT_EDGES>(m);
}

template<MeshConcept MeshType>
requires (HasFaceAdjacentEdges<MeshType>)
void requireFaceAdjacentEdges(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::ADJACENT_EDGES>(m);
}

template<MeshConcept MeshType>
bool enableIfEdgeAdjacentEdgesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::EDGE, CompId::ADJACENT_EDGES>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeAdjacentEdges<MeshType>)
void requireEdgeAdjacentEdges(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::ADJACENT_EDGES>(m);
}

/* --- Adjacencies: AdjacentVertices --- */

template<MeshConcept MeshType>
bool enableIfVertexAdjacentVerticesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
requires (HasVertexAdjacentVertices<MeshType>)
void requireVertexAdjacentVertices(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceAdjacentVerticesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
requires (HasFaceAdjacentVertices<MeshType>)
void requireFaceAdjacentVertices(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
bool enableIfEdgeAdjacentVerticesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::EDGE, CompId::ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeAdjacentVertices<MeshType>)
void requireEdgeAdjacentVertices(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::ADJACENT_VERTICES>(m);
}

/* --- Qualities --- */

template<MeshConcept MeshType>
bool enableIfVertexQualitiesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::QUALITY>(m);
}

template<MeshConcept MeshType>
requires (HasVertexQualities<MeshType>)
void requireVertexQualities(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::QUALITY>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceQualitiesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::QUALITY>(m);
}

template<MeshConcept MeshType>
requires (HasFaceQualities<MeshType>)
void requireFaceQualities(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::QUALITY>(m);
}

template<MeshConcept MeshType>
bool enableIfEdgeQualitiesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::EDGE, CompId::QUALITY>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeQualities<MeshType>)
void requireEdgeQualities(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::QUALITY>(m);
}

/* --- Principal Curvatures --- */

template<MeshConcept MeshType>
bool enableIfVertexPrincipalCurvaturesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
requires (HasVertexPrincipalCurvatures<MeshType>)
void requireVertexPrincipalCurvatures(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
bool enableIfFacePrincipalCurvaturesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
requires (HasFacePrincipalCurvatures<MeshType>)
void requireFacePrincipalCurvatures(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::PRINCIPAL_CURVATURE>(m);
}

/* --- Tangents --- */

template<MeshConcept MeshType>
bool enableIfVertexTangentsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::TANGENT>(m);
}

template<MeshConcept MeshType>
requires (HasVertexTangents<MeshType>)
void requireVertexTangents(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::TANGENT>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceTangentsOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::TANGENT>(m);
}

template<MeshConcept MeshType>
requires (HasFaceTangents<MeshType>)
void requireFaceTangents(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::TANGENT>(m);
}

/* --- Material Index --- */

template<MeshConcept MeshType>
bool enableIfVertexMaterialIndicesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::VERTEX, CompId::MATERIAL_INDEX>(m);
}

template<MeshConcept MeshType>
requires (HasFaceMaterialIndices<MeshType>)
void requireVertexMaterialIndices(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::MATERIAL_INDEX>(m);
}

template<MeshConcept MeshType>
bool enableIfFaceMaterialIndicesOptional(MeshType& m)
{
    return enableIfElementComponentOptional<ElemId::FACE, CompId::MATERIAL_INDEX>(m);
}

template<MeshConcept MeshType>
requires (HasFaceMaterialIndices<MeshType>)
void requireFaceMaterialIndices(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::MATERIAL_INDEX>(m);
}

/* --- Custom Components --- */

template<MeshConcept MeshType>
requires (HasVertexCustomComponents<MeshType>)
void requireVertexCustomComponents(const MeshType& m)
{
    requireElementComponent<ElemId::VERTEX, CompId::CUSTOM_COMPONENTS>(m);
}

template<MeshConcept MeshType>
requires (HasFaceCustomComponents<MeshType>)
void requireFaceCustomComponents(const MeshType& m)
{
    requireElementComponent<ElemId::FACE, CompId::CUSTOM_COMPONENTS>(m);
}

template<MeshConcept MeshType>
requires (HasEdgeCustomComponents<MeshType>)
void requireEdgeCustomComponents(const MeshType& m)
{
    requireElementComponent<ElemId::EDGE, CompId::CUSTOM_COMPONENTS>(m);
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_CONTRACTS_H
