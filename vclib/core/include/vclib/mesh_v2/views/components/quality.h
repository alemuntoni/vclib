// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_VIEWS_COMPONENTS_QUALITY_H
#define VCL_MESH_V2_VIEWS_COMPONENTS_QUALITY_H

#include "../component.h"

// TODO: When Quality component is ported to mesh_v2:
// 1. Define QualityRangeConcept
// 2. Define detail::getQuality lambda
// 3. Define QualityView : std::ranges::range_adaptor_closure<QualityView>
// 4. Define inline constexpr detail::QualityView quality;
// 5. Specialize template<> inline constexpr auto component<CompId::QUALITY> = quality;

#endif // VCL_MESH_V2_VIEWS_COMPONENTS_QUALITY_H
