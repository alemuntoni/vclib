// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_V1_MESH_VIEWS_COMPONENTS_SELECTION_H
#define VCL_V1_MESH_VIEWS_COMPONENTS_SELECTION_H

#include <vclib/base.h>

#include <ranges>

namespace vcl::v1::views {

namespace detail {

inline constexpr auto isSelected = [](auto&& e) -> decltype(auto) {
    if constexpr (vcl::IsPointer<decltype(e)>) {
        return e->selected();
    }
    else {
        return e.selected();
    }
};

inline constexpr auto isNotSelected = [](auto&& e) -> decltype(auto) {
    if constexpr (vcl::IsPointer<decltype(e)>) {
        return !e->selected();
    }
    else {
        return !e.selected();
    }
};

struct SelectionView
{
    constexpr SelectionView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, SelectionView)
    {
        return std::forward<R>(r) | std::views::transform(isSelected);
    }
};

struct SelectedView
{
    constexpr SelectedView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, SelectedView)
    {
        return std::forward<R>(r) | std::views::filter(isSelected);
    }
};

struct NotSelectedView
{
    constexpr NotSelectedView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, NotSelectedView)
    {
        return std::forward<R>(r) | std::views::filter(isNotSelected);
    }
};

} // namespace detail

inline constexpr detail::SelectionView   selection;
inline constexpr detail::SelectedView    selected;
inline constexpr detail::NotSelectedView notSelected;

} // namespace vcl::v1::views

#endif // VCL_V1_MESH_VIEWS_COMPONENTS_SELECTION_H
