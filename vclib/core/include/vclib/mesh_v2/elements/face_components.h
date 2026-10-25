// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/material_index.h"
#include "../components/normal.h"
#include "../components/polygon_bit_flags.h"
#include "../components/principal_curvature.h"
#include "../components/quality.h"
#include "../components/tangent.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_references.h"
#include "../components/wedges.h"

namespace vcl::meshv2::face {

template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<T>;
template<typename T>
concept HasStaticAdjacentFaces = comp::HasStaticAdjacentFaces<T>;
template<typename T>
concept HasTriangleAdjacentFaces = comp::HasTriangleAdjacentFaces<T>;
template<typename T>
concept HasPolygonAdjacentFaces = comp::HasPolygonAdjacentFaces<T>;

template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<T>;
template<typename T>
concept HasStaticAdjacentEdges = comp::HasStaticAdjacentEdges<T>;
template<typename T>
concept HasTriangleAdjacentEdges = comp::HasTriangleAdjacentEdges<T>;
template<typename T>
concept HasPolygonAdjacentEdges = comp::HasPolygonAdjacentEdges<T>;

template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasTriangleBitFlags = comp::HasTriangleBitFlags<T>;
template<typename T>
concept HasPolygonBitFlags = comp::HasPolygonBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasMaterialIndex = comp::HasMaterialIndex<T>;
template<typename T>
concept HasOptionalMaterialIndex = comp::HasOptionalMaterialIndex<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasOptionalNormal = comp::HasOptionalNormal<T>;
template<typename T>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<T>;
template<typename T>
concept HasOptionalPrincipalCurvature = comp::HasOptionalPrincipalCurvature<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasTangent = comp::HasTangent<T>;
template<typename T>
concept HasOptionalTangent = comp::HasOptionalTangent<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;
template<typename T>
concept HasStaticVertexReferences = comp::HasStaticVertexReferences<T>;
template<typename T>
concept HasTriangleVertexReferences = comp::HasTriangleVertexReferences<T>;
template<typename T>
concept HasPolygonVertexReferences = comp::HasPolygonVertexReferences<T>;
template<typename T>
concept HasWedgeColors = comp::HasWedgeColors<T>;
template<typename T>
concept HasOptionalWedgeColors = comp::HasOptionalWedgeColors<T>;
template<typename T>
concept HasWedgeTexCoords = comp::HasWedgeTexCoords<T>;
template<typename T>
concept HasOptionalWedgeTexCoords = comp::HasOptionalWedgeTexCoords<T>;
template<typename T>
concept HasWedgeNormals = comp::HasWedgeNormals<T>;
template<typename T>
concept HasOptionalWedgeNormals = comp::HasOptionalWedgeNormals<T>;
template<typename T>
concept HasWedges = comp::HasWedges<T>;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has either the
 * PolygonBitFlags or the TriangleBitFlags component.
 */
template<typename T>
concept HasFaceBitFlags = comp::HasFaceBitFlags<T>;

/* Port AdjacentEdges classes into face namespace */
template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   TTVC = false,
    bool                   OPT  = false>
using AdjacentEdges = comp::AdjacentEdges<N, Index, TTVC, OPT>;

template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   TTVC = false>
using OptionalAdjacentEdges = comp::AdjacentEdges<N, Index, TTVC, true>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using TriangleAdjacentEdges = comp::TriangleAdjacentEdges<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalTriangleAdjacentEdges = comp::OptionalTriangleAdjacentEdges<Index>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using PolygonAdjacentEdges = comp::PolygonAdjacentEdges<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalPolygonAdjacentEdges = comp::OptionalPolygonAdjacentEdges<Index>;

// Old mesh v1 compatibility aliases
template<std::unsigned_integral Index = uint, bool OPT = false>
using AdjacentTriangleEdges = TriangleAdjacentEdges<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentTriangleEdges = OptionalTriangleAdjacentEdges<Index>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using AdjacentPolygonEdges = PolygonAdjacentEdges<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentPolygonEdges = OptionalPolygonAdjacentEdges<Index>;

/* Port AdjacentFaces classes into face namespace */
template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   TTVC = false,
    bool                   OPT  = false>
using AdjacentFaces = comp::AdjacentFaces<N, Index, TTVC, OPT>;

template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   TTVC = false>
using OptionalAdjacentFaces = comp::AdjacentFaces<N, Index, TTVC, true>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using TriangleAdjacentFaces = comp::TriangleAdjacentFaces<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalTriangleAdjacentFaces = comp::OptionalTriangleAdjacentFaces<Index>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using PolygonAdjacentFaces = comp::PolygonAdjacentFaces<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalPolygonAdjacentFaces = comp::OptionalPolygonAdjacentFaces<Index>;

// Old mesh v1 compatibility aliases
template<std::unsigned_integral Index = uint, bool OPT = false>
using AdjacentTriangleFaces = TriangleAdjacentFaces<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentTriangleFaces = OptionalTriangleAdjacentFaces<Index>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using AdjacentPolygonFaces = PolygonAdjacentFaces<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentPolygonFaces = OptionalPolygonAdjacentFaces<Index>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using AdjacentTriangles = TriangleAdjacentFaces<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentTriangles = OptionalTriangleAdjacentFaces<Index>;

template<std::unsigned_integral Index = uint, bool OPT = false>
using AdjacentPolygons = PolygonAdjacentFaces<Index, OPT>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentPolygons = OptionalPolygonAdjacentFaces<Index>;

/* Port BitFlags classes into face namespace */
using BitFlags         = comp::BitFlags;
using TriangleBitFlags = comp::TriangleBitFlags;
using PolygonBitFlags  = comp::PolygonBitFlags<-1>;

/* Port Color class into face namespace */
using Color         = comp::Color<false>;
using OptionalColor = comp::OptionalColor;

/* Port CustomComponents class into face namespace */
using CustomComponents = comp::CustomComponents;

/* Port MaterialIndex class into face namespace */
using MaterialIndex           = comp::MaterialIndex<ushort, false>;
using OptionalMaterialIndex   = comp::OptionalMaterialIndex<ushort>;
using MaterialIndexus         = comp::MaterialIndexus;
using MaterialIndexu          = comp::MaterialIndexu;
using OptionalMaterialIndexus = comp::OptionalMaterialIndexus;
using OptionalMaterialIndexu  = comp::OptionalMaterialIndexu;

/* Port Normal classes into face namespace */
template<typename ScalarType, int N, bool OPT = false>
using Normal = comp::Normal<Point<ScalarType, N>, OPT>;

template<typename ScalarType, bool OPT = false>
using Normal3 = comp::Normal3<ScalarType, OPT>;

using Normal3f         = comp::Normal3f;
using Normal3d         = comp::Normal3d;
using OptionalNormal3f = comp::OptionalNormal3f;
using OptionalNormal3d = comp::OptionalNormal3d;

/* Port PrincipalCurvature classes into face namespace */
template<typename ScalarType, bool OPT = false>
using PrincipalCurvature = comp::PrincipalCurvature<ScalarType, OPT>;

template<typename ScalarType>
using OptionalPrincipalCurvature = comp::PrincipalCurvature<ScalarType, true>;

using PrincipalCurvaturef         = comp::PrincipalCurvaturef<false>;
using PrincipalCurvatured         = comp::PrincipalCurvatured<false>;
using OptionalPrincipalCurvaturef = comp::PrincipalCurvaturef<true>;
using OptionalPrincipalCurvatured = comp::PrincipalCurvatured<true>;

/* Port Quality classes into face namespace */

template<typename ScalarType, bool OPT = false>
using Quality = comp::Quality<ScalarType, OPT>;

template<typename ScalarType>
using OptionalQuality = comp::Quality<ScalarType, true>;

using Qualityf         = comp::Qualityf<false>;
using Qualityd         = comp::Qualityd<false>;
using OptionalQualityf = comp::Qualityf<true>;
using OptionalQualityd = comp::Qualityd<true>;

/* Port Tangent classes into face namespace */

template<typename ScalarType, int N, bool OPT = false>
using Tangent = comp::Tangent<Point<ScalarType, N>, OPT>;

template<typename ScalarType, bool OPT = false>
using Tangent3 = comp::Tangent3<ScalarType, OPT>;

using Tangent3f         = comp::Tangent3f;
using Tangent3d         = comp::Tangent3d;
using OptionalTangent3f = comp::OptionalTangent3f;
using OptionalTangent3d = comp::OptionalTangent3d;

/* Port VertexReferences class into face namespace */
template<int N, std::unsigned_integral Index = uint>
using VertexReferences = comp::VertexReferences<N, Index>;

template<std::unsigned_integral Index = uint>
using TriangleVertexReferences = comp::TriangleVertexReferences<Index>;

template<std::unsigned_integral Index = uint>
using PolygonVertexReferences = comp::PolygonVertexReferences<Index>;

/* Port Wedges class and aliases into face namespace */
template<typename Comp, int N = 3, bool OPT = comp::IsCompOptional<Comp>::value>
using Wedges = comp::Wedges<Comp, N, OPT>;

template<typename Comp, int N = 3, bool OPT = comp::IsCompOptional<Comp>::value>
using Wedge = comp::Wedge<Comp, N, OPT>;

template<int N = 3, bool OPT = false>
using WedgesColors = comp::WedgesColors<N, OPT>;

template<int N = 3, bool OPT = false>
using WedgeColors = comp::WedgeColors<N, OPT>;

template<typename Scalar = float, int N = 3, bool OPT = false>
using WedgesTexCoords = comp::WedgesTexCoords<Scalar, N, OPT>;

template<typename Scalar = float, int N = 3, bool OPT = false>
using WedgeTexCoords = comp::WedgeTexCoords<Scalar, N, OPT>;

template<typename Point = Point3f, int N = 3, bool OPT = false>
using WedgesNormals = comp::WedgesNormals<Point, N, OPT>;

template<typename Point = Point3f, int N = 3, bool OPT = false>
using WedgeNormals = comp::WedgeNormals<Point, N, OPT>;

// Triangle wedge aliases (N = 3)
using TriangleWedgeColors              = comp::TriangleWedgeColors;
using OptionalTriangleWedgeColors      = comp::OptionalTriangleWedgeColors;

using TriangleWedgeTexCoordsf          = comp::TriangleWedgeTexCoordsf;
using OptionalTriangleWedgeTexCoordsf  = comp::OptionalTriangleWedgeTexCoordsf;
using TriangleWedgeTexCoordsd          = comp::TriangleWedgeTexCoordsd;
using OptionalTriangleWedgeTexCoordsd  = comp::OptionalTriangleWedgeTexCoordsd;

using TriangleWedgeNormals3f           = comp::TriangleWedgeNormals3f;
using OptionalTriangleWedgeNormals3f   = comp::OptionalTriangleWedgeNormals3f;
using TriangleWedgeNormals3d           = comp::TriangleWedgeNormals3d;
using OptionalTriangleWedgeNormals3d   = comp::OptionalTriangleWedgeNormals3d;

// Polygon wedge aliases (N = -1)
using PolygonWedgeColors               = comp::PolygonWedgeColors;
using OptionalPolygonWedgeColors       = comp::OptionalPolygonWedgeColors;

using PolygonWedgeTexCoordsf           = comp::PolygonWedgeTexCoordsf;
using OptionalPolygonWedgeTexCoordsf   = comp::OptionalPolygonWedgeTexCoordsf;
using PolygonWedgeTexCoordsd           = comp::PolygonWedgeTexCoordsd;
using OptionalPolygonWedgeTexCoordsd   = comp::OptionalPolygonWedgeTexCoordsd;

using PolygonWedgeNormals3f            = comp::PolygonWedgeNormals3f;
using OptionalPolygonWedgeNormals3f    = comp::OptionalPolygonWedgeNormals3f;
using PolygonWedgeNormals3d            = comp::PolygonWedgeNormals3d;
using OptionalPolygonWedgeNormals3d    = comp::OptionalPolygonWedgeNormals3d;

} // namespace vcl::meshv2::face

#endif // VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H
