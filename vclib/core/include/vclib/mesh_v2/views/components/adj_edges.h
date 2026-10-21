// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_VIEWS_COMPONENTS_ADJ_EDGES_H
#define VCL_MESH_V2_VIEWS_COMPONENTS_ADJ_EDGES_H

#include "../component.h"

// TODO: When AdjacentEdges component is ported to mesh_v2:
// 1. Define AdjEdgesRangeConcept
// 2. Define detail::getAdjEdges lambda
// 3. Define AdjEdgesView : std::ranges::range_adaptor_closure<AdjEdgesView>
// 4. Define inline constexpr detail::AdjEdgesView adjEdges;
// 5. Specialize template<> inline constexpr auto component<CompId::ADJ_EDGES> = adjEdges;

#endif // VCL_MESH_V2_VIEWS_COMPONENTS_ADJ_EDGES_H
