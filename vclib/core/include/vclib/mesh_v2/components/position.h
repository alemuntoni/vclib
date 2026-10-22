// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_POSITION_H
#define VCL_MESH_V2_COMPONENTS_POSITION_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::meshv2::comp {

/**
 * @brief The Position class represents a N-Dimensional point that will be part
 * of an Element (e.g. Vertex...).
 *
 * Allows to get and set an object that represents a position in a space. The
 * type of this object must satisfy the PointConcept.
 *
 * For example, if you have a Vertex Element `v` with the Position component,
 * you'll be able to access this component's member functions from `v`:
 *
 * @code{.cpp}
 * auto pos = v.position();
 * @endcode
 *
 * @tparam P The type of the position. This template argument must satisfy the
 * PointConcept.
 *
 * @ingroup components
 */
template<PointConcept P>
class Position : public Component<CompId::POSITION, P>
{
    using Base = Component<CompId::POSITION, P>;

public:
    /**
     * @brief Expose the type of the Position.
     */
    using PositionType = P;

    Position() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the position of the element.
     * Propagates constness automatically from the caller (proxy).
     */
    decltype(auto) position(this auto&& self) { return Base::compData(self); }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the Position
 * component (COMP_ID = CompId::POSITION).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasPosition = HasComponent<CompId::POSITION, T>;

/* Detector function to check if a class has Position available */

/**
 * @brief Checks if the given Element has Position component available.
 *
 * @param[in] element The element to check.
 * @return `true` if the element has Position component available, `false`
 * otherwise.
 */
inline bool isPositionAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::POSITION>(element);
}

/* Implementation of member functions */

template<PointConcept P>
template<typename OtherElement>
void Position<P>::importFrom(this auto&& self, const OtherElement& other, bool)
{
    if constexpr (HasPosition<OtherElement>) {
        if (isPositionAvailableOn(self) && isPositionAvailableOn(other)) {
            using ScalarType = PositionType::ScalarType;
            self.position() = other.position().template cast<ScalarType>();
        }
    }
}

/* Specialization Aliases */

/**
 * @brief The Position3 alias of the Position component that uses 3-dimensional
 * Points.
 *
 * @tparam Scalar The scalar type of the point (e.g. float, double).
 *
 * @ingroup components
 */
template<typename Scalar>
using Position3 = Position<Point3<Scalar>>;

/**
 * @brief The Position3f alias of the Position component that uses 3-dimensional
 * Points with float precision.
 *
 * @ingroup components
 */
using Position3f = Position3<float>;

/**
 * @brief The Position3d alias of the Position component that uses 3-dimensional
 * Points with double precision.
 *
 * @ingroup components
 */
using Position3d = Position3<double>;

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_POSITION_H
