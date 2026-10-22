// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_COLOR_H
#define VCL_MESH_V2_COMPONENTS_COLOR_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::meshv2::comp {

/**
 * @brief The Color class represents a RGBA color that will be part of an
 * Element (e.g. Vertex, Face...).
 *
 * Allows to get and set an object of type Color.
 *
 * For example, if you have a Vertex Element `v` with the Color component,
 * you'll be able to access this component's member functions from `v`:
 *
 * @code{.cpp}
 * auto c = v.color();
 * @endcode
 *
 * @ingroup components
 */
class Color : public Component<CompId::COLOR, vcl::Color>
{
    using Base = Component<CompId::COLOR, vcl::Color>;

public:
    /**
     * @brief Expose the type of the Color.
     */
    using ColorType = vcl::Color;

    /* Constructors */

    /**
     * @brief Initializes the Color component.
     */
    Color() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the color of the element.
     * Propagates constness automatically from the caller (proxy).
     * @return a reference to the color of the element.
     */
    decltype(auto) color(this auto&& self) { return Base::compData(self); }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the Color
 * component (COMP_ID = CompId::COLOR).
 *
 * @tparam T The type to be tested for conformity to HasColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasColor = HasComponent<CompId::COLOR, T>;

/* Detector function to check if a class has Color available */

/**
 * @brief Checks if the given Element has Color component available.
 *
 * @param[in] element The element to check.
 * @return `true` if the element has Color component available, `false`
 * otherwise.
 */
inline bool isColorAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::COLOR>(element);
}

/* Implementation of member functions */

template<typename OtherElement>
void Color::importFrom(this auto&& self, const OtherElement& other, bool)
{
    if constexpr (HasColor<OtherElement>) {
        if (isColorAvailableOn(other)) {
            self.color() = other.color();
        }
    }
}

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_COLOR_H
