// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_TEX_COORDS_H
#define VCL_MESH_VIEWS_COMPONENTS_TEX_COORDS_H

#include "../component.h"

// TODO: When TexCoord component is ported to mesh_v2:
// 1. Define TexCoordRangeConcept
// 2. Define detail::getTexCoord lambda
// 3. Define TexCoordsView : std::ranges::range_adaptor_closure<TexCoordsView>
// 4. Define inline constexpr detail::TexCoordsView texCoords;
// 5. Specialize template<> inline constexpr auto component<CompId::TEX_COORD> = texCoords;

#endif // VCL_MESH_VIEWS_COMPONENTS_TEX_COORDS_H
