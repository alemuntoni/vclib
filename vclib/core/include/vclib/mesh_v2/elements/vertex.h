// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_ELEMENTS_VERTEX_H
#define VCL_MESH_V2_ELEMENTS_VERTEX_H

#include "vertex_components.h"

#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/components/position.h>
#include <vclib/mesh_v2/elements/base/base.h>
#include <vclib/mesh_v2/elements/base/element_proxy.h>

namespace vcl::meshv2 {

/**
 * @brief The Vertex class represents the runtime proxy (view) of a
 * Vertex element in a mesh container.
 *
 * It inherits from ElementProxy and provides object-oriented access to
 * the per-vertex components stored in the container's Structure of Arrays.
 *
 * @tparam ContainerType The type of the container (mutable or const).
 *
 * @ingroup elements
 */
template<typename ContainerType>
class Vertex : public ElementProxy<ElemId::VERTEX, ContainerType>
{
    using Base = ElementProxy<ElemId::VERTEX, ContainerType>;

public:
    using typename Base::Components;
    using typename Base::IndexType;

    static_assert(
        comp::HasPosition<Components>,
        "A Vertex must have the Position component "
        "(vcl::meshv2::comp::Position).");
    static_assert(
        comp::HasBitFlags<Components>,
        "A Vertex must have the BitFlags component "
        "(vcl::meshv2::comp::BitFlags).");

    Vertex() = default;

    // Implicit conversion constructor from mutable Vertex to const Vertex
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    Vertex(const Vertex<OtherContainerType>& other) : Base(other)
    {
    }

protected:
    Vertex(ContainerType& container, IndexType index) : Base(container, index)
    {
    }

private:
    /// friend classes declarations

    template<uint, typename, typename>
    friend class ElementProxy;

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class ElementContainer;

    template<typename>
    friend class ElementIterator;

    template<typename>
    friend class Vertex;
};

/**
 * @brief The VertexComponents struct defines a type-wrapper for the components
 * of a Vertex, checking at compile-time that the required components (BitFlags,
 * Position) are present.
 *
 * @tparam Comps The components that compose the Vertex.
 *
 * @ingroup elements
 */
template<typename... Comps>
struct VertexComponents : vcl::TypeWrapper<Comps...>
{
    static_assert(
        comp::HasPosition<vcl::TypeWrapper<Comps...>>,
        "Vertex components must include Position "
        "(vcl::meshv2::comp::Position).");
    static_assert(
        comp::HasBitFlags<vcl::TypeWrapper<Comps...>>,
        "Vertex components must include BitFlags "
        "(vcl::meshv2::comp::BitFlags).");
};

template<typename... Comps>
struct VertexComponents<vcl::TypeWrapper<Comps...>> : VertexComponents<Comps...>
{
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T is a Vertex element.
 *
 * The concept is satisfied when T satisfies ElementConcept, has ELEMENT_ID ==
 * ElemId::VERTEX, and has BitFlags and Position components.
 *
 * @ingroup vertex_concepts
 */
template<typename T>
concept VertexConcept =
    ElementConcept<T> &&
    (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::VERTEX) &&
    comp::HasBitFlags<typename std::remove_cvref_t<T>::Components> &&
    comp::HasPosition<typename std::remove_cvref_t<T>::Components>;

/**
 * @brief Concept that checks whether a type T is a valid component list for a
 * Vertex.
 *
 * @ingroup vertex_concepts
 */
template<typename T>
concept HasRequiredVertexComponents =
    comp::HasBitFlags<T> && comp::HasPosition<T>;

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_ELEMENTS_VERTEX_H
