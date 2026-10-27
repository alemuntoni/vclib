// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_V1_MESH_VIEWS_COMPONENT_H
#define VCL_V1_MESH_VIEWS_COMPONENT_H

#include <vclib/base.h>

namespace vcl::v1::views {

namespace detail {
struct UnsupportedComponentView
{
};
} // namespace detail

template<uint COMP_ID>
inline constexpr auto component = detail::UnsupportedComponentView {};

} // namespace vcl::v1::views

#endif // VCL_V1_MESH_VIEWS_COMPONENT_H
