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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H

#ifndef VCLIB_WITH_MODULES
#include <string>

#include "component.h"
#endif

namespace vcl::comp {

/**
 * @brief HasTexturePaths concept is satisfied only if a Element or Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::TexturePaths component of a
 * given element/mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTexturePaths = requires (T o, const T& co, std::string s) {
    // clang-format off
    typename T::TexFileNamesIterator;
    typename T::ConstTexFileNamesIterator;

    { co.textureNumber() } -> std::same_as<uint>;
    { co.texturePath(uint()) } -> std::same_as<const std::string&>;
    { o.texturePath(uint()) } -> std::same_as<std::string&>;
    { co.meshBasePath() } -> std::same_as<const std::string&>;
    { o.meshBasePath() } -> std::same_as<std::string&>;

    { o.clearTexturePaths() } -> std::same_as<void>;
    { o.pushTexturePath(s) } -> std::same_as<void>;

    { o.texturePathBegin() } -> std::same_as<typename T::TexFileNamesIterator>;
    { o.texturePathEnd() } -> std::same_as<typename T::TexFileNamesIterator>;
    { co.texturePathBegin() } ->
        std::same_as<typename T::ConstTexFileNamesIterator>;
    { co.texturePathEnd() } ->
        std::same_as<typename T::ConstTexFileNamesIterator>;
    o.texturePaths();
    co.texturePaths();
    // clang-format on
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
