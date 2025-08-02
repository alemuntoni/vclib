/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

module;

#include <tuple>

export module vclib.base:templated_type_wrapper;

import :variadic_templates;

namespace vcl {

template<template<typename...> typename... Args>
struct TemplatedTypeWrapper
{
    static constexpr uint size() { return sizeof...(Args); }
};

// note: specialization from variadic_templates.h
template<template<typename...> typename... Args>
struct FirstType<TemplatedTypeWrapper<Args...>>
{
    template<typename... T>
    using type = std::tuple_element<0, std::tuple<Args<T>...>>::type;
};

} // namespace vcl
