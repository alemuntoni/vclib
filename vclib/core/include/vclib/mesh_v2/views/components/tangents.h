// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_VIEWS_COMPONENTS_TANGENTS_H
#define VCL_MESH_V2_VIEWS_COMPONENTS_TANGENTS_H

#include "../component.h"

// TODO: When Tangent component is ported to mesh_v2:
// 1. Define TangentRangeConcept
// 2. Define detail::getTangent lambda
// 3. Define TangentsView : std::ranges::range_adaptor_closure<TangentsView>
// 4. Define inline constexpr detail::TangentsView tangents;
// 5. Specialize template<> inline constexpr auto component<CompId::TANGENT> = tangents;

#endif // VCL_MESH_V2_VIEWS_COMPONENTS_TANGENTS_H
