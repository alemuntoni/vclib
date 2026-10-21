// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H
#define VCL_MESH_V2_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H

#include "../component.h"

// TODO: When PrincipalCurvature component is ported to mesh_v2:
// 1. Define PrincipalCurvaturesRangeConcept
// 2. Define detail::getPrincipalCurvature lambda
// 3. Define PrincipalCurvaturesView : std::ranges::range_adaptor_closure<PrincipalCurvaturesView>
// 4. Define inline constexpr detail::PrincipalCurvaturesView principalCurvatures;
// 5. Specialize template<> inline constexpr auto component<CompId::PRINCIPAL_CURVATURE> = principalCurvatures;

#endif // VCL_MESH_V2_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H
