// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_V1_MESH_VIEWS_COMPONENTS_NORMALS_H
#define VCL_V1_MESH_VIEWS_COMPONENTS_NORMALS_H

#include <v1/mesh/components/normal.h>
#include <v1/mesh/views/component.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::v1::views {

namespace detail {

inline constexpr auto normal = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->normal();
    else
        return p.normal();
};

struct NormalsView
{
    constexpr NormalsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, NormalsView)
    {
        return std::forward<R>(r) | std::views::transform(normal);
    }
};

} // namespace detail

inline constexpr detail::NormalsView normals;

template<>
inline constexpr auto component<CompId::NORMAL> = normals;

} // namespace vcl::v1::views

#endif // VCL_V1_MESH_VIEWS_COMPONENTS_NORMALS_H
