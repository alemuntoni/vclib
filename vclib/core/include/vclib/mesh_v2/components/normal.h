// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_NORMAL_H
#define VCL_MESH_V2_COMPONENTS_NORMAL_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::meshv2::comp {

/**
 * @brief The Normal class represents a N-Dimensional normal vector that will be
 * part of an Element (e.g. Vertex, Face...).
 *
 * Allows to get and set an object that represents a normal. The type of this
 * object must satisfy the PointConcept.
 *
 * For example, if you have a Vertex Element `v` with the Normal component,
 * you'll be able to access this component's member functions from `v`:
 *
 * @code{.cpp}
 * auto n = v.normal();
 * @endcode
 *
 * @tparam P The type of the normal. This template argument must satisfy the
 * PointConcept.
 *
 * @ingroup components
 */
template<PointConcept P>
class Normal : public Component<CompId::NORMAL, P>
{
    using Base = Component<CompId::NORMAL, P>;
public:
    /**
     * @brief Expose the type of the Normal.
     */
    using NormalType = P;

    /* Constructors */

    /**
     * @brief Initializes the Normal component.
     */
    Normal() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the normal of the element.
     * Propagates constness automatically from the caller (proxy).
     * @return a reference to the normal of the element.
     */
    decltype(auto) normal(this auto&& self) { return Base::compData(self); }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the Normal
 * component (COMP_ID = CompId::NORMAL).
 *
 * @tparam T The type to be tested for conformity to HasNormal.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasNormal = HasComponent<CompId::NORMAL, T>;

/* Detector function to check if a class has Normal available */

/**
 * @brief Checks if the given Element has Normal component available.
 *
 * @param[in] element The element to check.
 * @return `true` if the element has Normal component available, `false`
 * otherwise.
 */
inline bool isNormalAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::NORMAL>(element);
}

/* Implementation of member functions */

template<PointConcept P>
template<typename OtherElement>
void Normal<P>::importFrom(this auto&& self, const OtherElement& other, bool)
{
    if constexpr (HasNormal<OtherElement>) {
        if (isNormalAvailableOn(self) && isNormalAvailableOn(other)) {
            using ScalarType = NormalType::ScalarType;
            self.normal()    = other.normal().template cast<ScalarType>();
        }
    }
}

/* Specialization Aliases */

/**
 * @brief The Normal3 alias of the Normal component that uses 3-dimensional
 * Points.
 *
 * @tparam Scalar The scalar type of the point (e.g. float, double).
 *
 * @ingroup components
 */
template<typename Scalar>
using Normal3 = Normal<Point3<Scalar>>;

/**
 * @brief The Normal3f alias of the Normal component that uses 3-dimensional
 * Points with float precision.
 *
 * @ingroup components
 */
using Normal3f = Normal3<float>;

/**
 * @brief The Normal3d alias of the Normal component that uses 3-dimensional
 * Points with double precision.
 *
 * @ingroup components
 */
using Normal3d = Normal3<double>;

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_NORMAL_H
