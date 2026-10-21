// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_VIEWS_COMPONENT_H
#define VCL_MESH_V2_VIEWS_COMPONENT_H

#include <vclib/base.h>

namespace vcl::meshv2::views {

namespace detail {

struct UnsupportedComponentView
{
};

} // namespace detail

/**
 * @brief Generic component view adaptor template variable for a given component ID.
 *
 * Specializations for supported component types:
 * - component<CompId::POSITION> = positions
 * - component<CompId::NORMAL>   = normals
 * - component<CompId::COLOR>    = colors
 * - ...
 */
template<uint COMP_ID>
inline constexpr auto component = detail::UnsupportedComponentView {};

} // namespace vcl::meshv2::views

#endif // VCL_MESH_V2_VIEWS_COMPONENT_H
