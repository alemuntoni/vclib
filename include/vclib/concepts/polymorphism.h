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

#ifndef VCL_CONCEPTS_POLYMORPHISM_H
#define VCL_CONCEPTS_POLYMORPHISM_H

#ifndef VCLIB_WITH_MODULES
#include <concepts>
#include <memory>
#endif

namespace vcl {

template<typename T>
concept Cloneable = requires (const T& o) {
    // clang-format off
    { o.clone() } -> std::same_as<std::shared_ptr<T>>;
    // clang-format on
};

} // namespace vcl

#endif // VCL_CONCEPTS_POLYMORPHISM_H
