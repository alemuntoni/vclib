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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_CONCEPTS_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#ifndef VCLIB_WITH_MODULES
#include "component.h"
#endif

namespace vcl::comp {

/**
 * @brief HasPrincipalCurvature concept is satisfied only if a Element class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a PrincipalCurvature component
 * of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * PrincipalCurvature component and the vertical OptionalPrincipalCurvature
 * component, therefore it does not guarantee that a template Element type that
 * satisfies this concept provides PrincipalCurvature component at runtime (it
 * is guaranteed only that the proper member functions are available at compile
 * time).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasPrincipalCurvature = requires (T o, const T& co) {
    // clang-format off
    typename T::PrincipalCurvatureType;
    { o.principalCurvature() } ->
        std::same_as<typename T::PrincipalCurvatureType&>;
    { co.principalCurvature() } ->
        std::same_as<const typename T::PrincipalCurvatureType&>;
    // clang-format on
};

/**
 * @brief HasOptionalPrincipalCurvature concept is satisfied only if a class
 * satisfis the HasPrincipalCurvature concept and the static boolean constant
 * IS_OPTIONAL is set to true.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalPrincipalCurvature =
    HasPrincipalCurvature<T> &&
    IsOptionalComponent<typename T::PrincipalCurvature>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
