/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_VIEWS_MESH_COMPONENTS_PRINCIPAL_CURVATURES_H
#define VCL_VIEWS_MESH_COMPONENTS_PRINCIPAL_CURVATURES_H

#ifndef VCLIB_WITH_MODULES
#include <vclib/concepts/pointers.h>
#include <vclib/types.h>

#include <ranges>
#endif

namespace vcl::views {

namespace detail {

constexpr auto constPrincipalCurvature = [](const auto& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->principalCurvature();
    else
        return p.principalCurvature();
};

constexpr auto principalCurvature = [](auto& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->principalCurvature();
    else
        return p.principalCurvature();
};

struct PrincipalCurvaturesView
{
    constexpr PrincipalCurvaturesView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, PrincipalCurvaturesView)
    {
        using ElemType = std::ranges::range_value_t<R>;
        if constexpr (IsConst<ElemType>)
            return std::forward<R>(r) |
                   std::views::transform(constPrincipalCurvature);
        else
            return std::forward<R>(r) |
                   std::views::transform(principalCurvature);
    }
};

} // namespace detail

inline constexpr detail::PrincipalCurvaturesView principalCurvatures;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_PRINCIPAL_CURVATURES_H
