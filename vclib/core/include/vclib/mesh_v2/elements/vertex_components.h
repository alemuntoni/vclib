// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_VERTEX_COMPONENTS_H
#define VCL_MESH_V2_ELEMENTS_VERTEX_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/adjacent_vertices.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/material_index.h"
#include "../components/normal.h"
#include "../components/position.h"
#include "../components/tex_coord.h"

namespace vcl::meshv2::vert {

template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<T>;

template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<T>;

template<typename T>
concept HasAdjacentVertices = comp::HasAdjacentVertices<T>;
template<typename T>
concept HasOptionalAdjacentVertices = comp::HasOptionalAdjacentVertices<T>;

template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
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
concept HasPosition = comp::HasPosition<T>;
template<typename T>
concept HasTexCoord = comp::HasTexCoord<T>;
template<typename T>
concept HasOptionalTexCoord = comp::HasOptionalTexCoord<T>;

/* Port AdjacentEdges class into vert namespace */
template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   OPT  = false>
using CustomAdjacentEdges = comp::AdjacentEdges<N, Index, false, OPT>;

using AdjacentEdges         = CustomAdjacentEdges<-1, uint, false>;
using OptionalAdjacentEdges = CustomAdjacentEdges<-1, uint, true>;

/* Port AdjacentFaces class into vert namespace */
template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   OPT  = false>
using CustomAdjacentFaces = comp::AdjacentFaces<N, Index, false, OPT>;

using AdjacentFaces         = CustomAdjacentFaces<-1, uint, false>;
using OptionalAdjacentFaces = CustomAdjacentFaces<-1, uint, true>;

/* Port AdjacentVertices class into vert namespace */
template<
    int                    N    = -1,
    std::unsigned_integral Index = uint,
    bool                   OPT  = false>
using CustomAdjacentVertices = comp::AdjacentVertices<N, Index, false, OPT>;

using AdjacentVertices         = CustomAdjacentVertices<-1, uint, false>;
using OptionalAdjacentVertices = CustomAdjacentVertices<-1, uint, true>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags;

/* Port Color class into vert namespace */
using Color         = comp::Color<false>;
using OptionalColor = comp::OptionalColor;

/* Port CustomComponents class into vert namespace */
using CustomComponents = comp::CustomComponents;

/* Port MaterialIndex class into vert namespace */
template<std::unsigned_integral Index = ushort, bool OPT = false>
using CustomMaterialIndex = comp::MaterialIndex<Index, OPT>;

using MaterialIndex           = comp::MaterialIndex<ushort, false>;
using OptionalMaterialIndex   = comp::OptionalMaterialIndex<ushort>;
using MaterialIndexus         = comp::MaterialIndexus;
using MaterialIndexu          = comp::MaterialIndexu;
using OptionalMaterialIndexus = comp::OptionalMaterialIndexus;
using OptionalMaterialIndexu  = comp::OptionalMaterialIndexu;

/* Port Normal classes into vert namespace */
template<typename ScalarType, int N, bool OPT = false>
using Normal = comp::Normal<Point<ScalarType, N>, OPT>;

template<typename ScalarType, bool OPT = false>
using Normal3 = comp::Normal3<ScalarType, OPT>;

using Normal3f         = comp::Normal3f;
using Normal3d         = comp::Normal3d;
using OptionalNormal3f = comp::OptionalNormal3f;
using OptionalNormal3d = comp::OptionalNormal3d;

/* Port Position class into vert namespace */
template<typename ScalarType, int N, bool OPT = false>
using Position = comp::Position<Point<ScalarType, N>, OPT>;

template<typename ScalarType, bool OPT = false>
using Position3 = comp::Position3<ScalarType, OPT>;

using Position3f         = comp::Position3f;
using Position3d         = comp::Position3d;

/* Port TexCoord class into vert namespace */
template<typename ScalarType, bool OPT = false>
using TexCoord = comp::TexCoord<ScalarType, OPT>;

template <typename ScalarType>
using OptionalTexCoord = comp::TexCoord<ScalarType, true>;

using TexCoordf = comp::TexCoordf<false>;
using TexCoordd = comp::TexCoordd<false>;
using OptionalTexCoordf = comp::TexCoordf<true>;
using OptionalTexCoordd = comp::TexCoordd<true>;

} // namespace vcl::meshv2::vert

#endif // VCL_MESH_V2_ELEMENTS_VERTEX_COMPONENTS_H
