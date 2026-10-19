// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H

#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/normal.h"
#include "../components/polygon_bit_flags.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_references.h"

namespace vcl::meshv2::face {

template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasTriangleBitFlags = comp::HasTriangleBitFlags<T>;
template<typename T>
concept HasPolygonBitFlags = comp::HasPolygonBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;
template<typename T>
concept HasStaticVertexReferences = comp::HasStaticVertexReferences<T>;
template<typename T>
concept HasTriangleVertexReferences = comp::HasTriangleVertexReferences<T>;
template<typename T>
concept HasPolygonVertexReferences = comp::HasPolygonVertexReferences<T>;

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has either the
 * PolygonBitFlags or the TriangleBitFlags component.
 */
template<typename T>
concept HasFaceBitFlags = HasTriangleBitFlags<T> || HasPolygonBitFlags<T>;

/* Port BitFlags classes into face namespace */
using BitFlags         = comp::BitFlags;
using TriangleBitFlags = comp::TriangleBitFlags;
using PolygonBitFlags  = comp::PolygonBitFlags<-1>;

/* Port Color class into face namespace */
using Color = comp::Color;

/* Port Normal classes into face namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<Point<ScalarType, N>>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

/* Port VertexReferences class into face namespace */
template<int N, std::unsigned_integral Index = uint>
using VertexReferences = comp::VertexReferences<N, Index>;

template<std::unsigned_integral Index = uint>
using TriangleVertexReferences = comp::TriangleVertexReferences<Index>;

template<std::unsigned_integral Index = uint>
using PolygonVertexReferences = comp::PolygonVertexReferences<Index>;

} // namespace vcl::meshv2::face

#endif // VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H
