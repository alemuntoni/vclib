// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_VIEWS_COMPONENTS_ADJ_VERTICES_H
#define VCL_MESH_V2_VIEWS_COMPONENTS_ADJ_VERTICES_H

#include "../component.h"

// TODO: When AdjacentVertices component is ported to mesh_v2:
// 1. Define AdjVerticesRangeConcept
// 2. Define detail::getAdjVertices lambda
// 3. Define AdjVerticesView : std::ranges::range_adaptor_closure<AdjVerticesView>
// 4. Define inline constexpr detail::AdjVerticesView adjVertices;
// 5. Specialize template<> inline constexpr auto component<CompId::ADJ_VERTICES> = adjVertices;

#endif // VCL_MESH_V2_VIEWS_COMPONENTS_ADJ_VERTICES_H
