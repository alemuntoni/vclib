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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_NAME_H
#define VCL_CONCEPTS_MESH_COMPONENTS_NAME_H

#include <vclib/concepts/const_correctness.h>

#include <string>

namespace vcl::comp {

/**
 * @brief HasName concept is satisfied only if a Element or Mesh class provides
 * the member functions specified in this concept. These member functions allows
 * to access to a @ref vcl::comp::Name component of a given element/mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasName = requires (T&& obj) {
    { obj.name() } -> std::convertible_to<std::string>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.name() } -> std::same_as<std::string&>;
    };
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_NAME_H
