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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#ifndef VCLIB_WITH_MODULES
#include <vector>

#include "component.h"
#endif

namespace vcl::comp {

/**
 * @brief HasWedgeTexCoords concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to an @ref
 * vcl::comp::WedgeTexCoords component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * WedgeTexCoords component and the vertical OptionalWedgeTexCoords component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides WedgeTexCoords component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasWedgeTexCoords = requires (
    T                                          o,
    const T&                                   co,
    const typename T::WedgeTexCoordType&       t,
    std::vector<typename T::WedgeTexCoordType> v) {
    // clang-format off
    T::WEDGE_TEX_COORD_NUMBER;
    typename T::WedgeTexCoordType;
    typename T::WedgeTexCoordsIterator;
    typename T::ConstWedgeTexCoordsIterator;

    { o.wedgeTexCoord(uint()) } -> std::same_as<typename T::WedgeTexCoordType&>;
    { co.wedgeTexCoord(uint()) } ->
        std::same_as<const typename T::WedgeTexCoordType&>;
    { o.wedgeTexCoordMod(int()) } ->
        std::same_as<typename T::WedgeTexCoordType&>;
    { co.wedgeTexCoordMod(int()) } ->
        std::same_as<const typename T::WedgeTexCoordType&>;
    { o.setWedgeTexCoord(uint(), t) } -> std::same_as<void>;
    { o.setWedgeTexCoords(v) } -> std::same_as<void>;
    { o.textureIndex() } -> std::same_as<short&>;
    { co.textureIndex() } -> std::same_as<short>;

    { o.wedgeTexCoordBegin() } ->
        std::same_as<typename T::WedgeTexCoordsIterator>;
    { o.wedgeTexCoordEnd() } ->
        std::same_as<typename T::WedgeTexCoordsIterator>;
    { co.wedgeTexCoordBegin() } ->
        std::same_as<typename T::ConstWedgeTexCoordsIterator>;
    { co.wedgeTexCoordEnd() } ->
        std::same_as<typename T::ConstWedgeTexCoordsIterator>;
    o.wedgeTexCoords();
    co.wedgeTexCoords();
    // clang-format on
};

/**
 * @brief HasOptionalWedgeTexCoords concept is satisfied only if a class
 * satisfied the @ref vcl::comp::HasWedgeCoords and has the static
 * boolean constant `IS_OPTIONAL` is set to `true`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalWedgeTexCoords =
    HasWedgeTexCoords<T> && IsOptionalComponent<typename T::WedgeTexCoords>;

/**
 * @private
 * @brief HasRightNumberOfWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge texcoords must be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge texcoords is the
 * same of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeTexCoords =
    T::VERTEX_NUMBER == T::WEDGE_TEX_COORD_NUMBER;

/**
 * @private
 * @brief SanityCheckWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge texcoords must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge texcoords;
 * - in case it has wedge texcoords, they have the same number of vertices of
 * the face.
 */
template<typename T>
concept SanityCheckWedgeTexCoords =
    !HasWedgeTexCoords<T> || HasRightNumberOfWedgeTexCoords<T>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
