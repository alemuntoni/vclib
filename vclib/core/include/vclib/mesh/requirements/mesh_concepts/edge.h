// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_EDGE_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_EDGE_H

#include "base.h"

namespace vcl {

/* =========================================================================
 * Edge Component Concepts
 * ========================================================================= */

/* Normals */
template<typename M>
concept HasEdgeNormal =
    HasElementComponent<M, ElemId::EDGE, CompId::NORMAL>;

template<typename M>
concept HasEdgeOptionalNormal =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::NORMAL>;

/* Colors */
template<typename M>
concept HasEdgeColor =
    HasElementComponent<M, ElemId::EDGE, CompId::COLOR>;

template<typename M>
concept HasEdgeOptionalColor =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::COLOR>;

/* TexCoords */
template<typename M>
concept HasEdgeTexCoord =
    HasElementComponent<M, ElemId::EDGE, CompId::TEX_COORD>;

template<typename M>
concept HasEdgeOptionalTexCoord =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::TEX_COORD>;

/* Adjacencies */
template<typename M>
concept HasEdgeAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeOptionalAdjacentFaces =
    HasElementOptionalAdjacentFaces<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeOptionalAdjacentEdges =
    HasElementOptionalAdjacentEdges<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeAdjacentVertices =
    HasElementAdjacentVertices<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeOptionalAdjacentVertices =
    HasElementOptionalAdjacentVertices<M, ElemId::EDGE>;

/* Quality */
template<typename M>
concept HasEdgeQuality =
    HasElementComponent<M, ElemId::EDGE, CompId::QUALITY>;

template<typename M>
concept HasEdgeOptionalQuality =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::QUALITY>;

/* PrincipalCurvature */
template<typename M>
concept HasEdgePrincipalCurvature =
    HasElementComponent<M, ElemId::EDGE, CompId::PRINCIPAL_CURVATURE>;

template<typename M>
concept HasEdgeOptionalPrincipalCurvature =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::PRINCIPAL_CURVATURE>;

/* Tangent */
template<typename M>
concept HasEdgeTangent =
    HasElementComponent<M, ElemId::EDGE, CompId::TANGENT>;

template<typename M>
concept HasEdgeOptionalTangent =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::TANGENT>;

/* MaterialIndex */
template<typename M>
concept HasEdgeMaterialIndex =
    HasElementComponent<M, ElemId::EDGE, CompId::MATERIAL_INDEX>;

template<typename M>
concept HasEdgeOptionalMaterialIndex =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::MATERIAL_INDEX>;

/* Custom Components */
template<typename M>
concept HasEdgeCustomComponents =
    HasElementCustomComponents<M, ElemId::EDGE>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_EDGE_H
