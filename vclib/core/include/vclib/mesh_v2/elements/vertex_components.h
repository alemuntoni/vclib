// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_VERTEX_COMPONENTS_H
#define VCL_MESH_V2_ELEMENTS_VERTEX_COMPONENTS_H

#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/normal.h"
#include "../components/position.h"

namespace vcl::meshv2::vert {

template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasPosition = comp::HasPosition<T>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags;

/* Port Color class into vert namespace */
using Color = comp::Color;

/* Port CustomComponents class into vert namespace */
using CustomComponents = comp::CustomComponents;

/* Port Normal classes into vert namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<Point<ScalarType, N>>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f;
using Normal3d = comp::Normal3d;

/* Port Position class into vert namespace */
template<typename ScalarType, int N>
using Position = comp::Position<Point<ScalarType, N>>;

template<typename ScalarType>
using Position3 = comp::Position3<ScalarType>;

using Position3f = comp::Position3f;
using Position3d = comp::Position3d;

} // namespace vcl::meshv2::vert

#endif // VCL_MESH_V2_ELEMENTS_VERTEX_COMPONENTS_H
