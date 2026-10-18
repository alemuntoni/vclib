// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_V2_ELEMENTS_FACE_COMPONENTS_H

#include "../components/bit_flags.h"
#include "../components/normal.h"
#include "../components/vertex_references.h"

namespace vcl::meshv2::face {

template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;
template<typename T>
concept HasTriangleVertexReferences = comp::HasTriangleVertexReferences<T>;
template<typename T>
concept HasPolygonVertexReferences = comp::HasPolygonVertexReferences<T>;

/* Port BitFlags class into face namespace */
using BitFlags = comp::BitFlags;

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
