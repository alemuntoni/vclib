// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_V1_MESH_VIEWS_COMPONENTS_COLORS_H
#define VCL_V1_MESH_VIEWS_COMPONENTS_COLORS_H

#include <v1/mesh/components/color.h>
#include <v1/mesh/components/wedge_colors.h>
#include <v1/mesh/views/component.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::v1::views {

namespace detail {

inline constexpr auto color = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->color();
    else
        return p.color();
};

struct ColorsView
{
    constexpr ColorsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, ColorsView)
    {
        return std::forward<R>(r) | std::views::transform(color);
    }

    template<comp::HasWedgeColors R>
    friend constexpr auto operator|(R&& r, ColorsView)
    {
        if constexpr (IsPointer<R>)
            return r->wedgeColors();
        else
            return r.wedgeColors();
    }
};

} // namespace detail

inline constexpr detail::ColorsView colors;

template<>
inline constexpr auto component<CompId::COLOR> = colors;

} // namespace vcl::v1::views

#endif // VCL_V1_MESH_VIEWS_COMPONENTS_COLORS_H
