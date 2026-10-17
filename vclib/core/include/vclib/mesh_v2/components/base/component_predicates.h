// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_PREDICATES_H
#define VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_PREDICATES_H

#include "component.h"

#include <vclib/base.h>

namespace vcl::meshv2::comp {

namespace detail {

// Helper details for finding the index of a Component by COMP_ID in a list of
// Components. It recursively checks each component's COMPONENT_ID against the
// target COMP_ID, incrementing the CurrentIndex until a match is found or the
// list is exhausted.
template<uint COMP_ID, uint CurrentIndex, typename... Components>
struct IndexOfCompIdImpl;

template<uint COMP_ID, uint CurrentIndex, typename First, typename... Rest>
struct IndexOfCompIdImpl<COMP_ID, CurrentIndex, First, Rest...>
{
    static constexpr uint value =
        (First::COMPONENT_ID == COMP_ID) ?
            CurrentIndex :
            IndexOfCompIdImpl<COMP_ID, CurrentIndex + 1, Rest...>::value;
};

// Base case when the component is not found
template<uint COMP_ID, uint CurrentIndex>
struct IndexOfCompIdImpl<COMP_ID, CurrentIndex>
{
    static constexpr uint value = UINT_NULL;
};

template<typename... Ts>
vcl::TypeWrapper<Ts...> extractTypeWrapper(const vcl::TypeWrapper<Ts...>&);

// Helper to extract the inner Components type from a type T if present,
// or the underlying TypeWrapper if T inherits from TypeWrapper,
// otherwise uses T directly.
template<typename T>
struct ComponentListHelper
{
private:
    static auto helper()
    {
        using RawT = std::remove_cvref_t<T>;
        if constexpr (requires { typename RawT::Components; }) {
            return typename RawT::Components{};
        }
        else if constexpr (requires { extractTypeWrapper(std::declval<RawT>()); }) {
            return decltype(extractTypeWrapper(std::declval<RawT>())){};
        }
        else {
            return RawT{};
        }
    }

public:
    using type = decltype(helper());
};

template<uint COMP_ID, typename T>
inline constexpr bool HasComponentImpl = false;

template<uint COMP_ID, typename... Components>
inline constexpr bool
    HasComponentImpl<COMP_ID, vcl::TypeWrapper<Components...>> =
    ((Components::COMPONENT_ID == COMP_ID) || ...);

} // namespace detail

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) contains a
 * component with the given COMP_ID.
 */
template<uint COMP_ID, typename T>
concept HasComponent = detail::
    HasComponentImpl<COMP_ID, typename detail::ComponentListHelper<T>::type>;

/**
 * @brief Concept that checks whether a component Comp stores references to
 * elements of type TARGET_ELEM_ID (e.g. ElemId::VERTEX).
 *
 * A component declares this by defining:
 *   template<uint TARGET_ELEM_ID> static constexpr bool referencesElement()
 */
template<typename Comp, uint TARGET_ELEM_ID>
concept ComponentReferencesElement = requires {
    requires Comp::template referencesElement<TARGET_ELEM_ID>();
};

/**
 * @brief Trait to find the index of a Component by its COMP_ID inside a
 * TypeWrapper.
 *
 * Usage: IndexOfCompId<COMP_ID, TypeWrapper<...>>::value
 */
template<uint COMP_ID, typename ComponentListWrapper>
struct IndexOfCompId;

template<uint COMP_ID, typename... Components>
struct IndexOfCompId<COMP_ID, vcl::TypeWrapper<Components...>>
{
    static_assert(
        detail::HasComponentImpl<COMP_ID, vcl::TypeWrapper<Components...>>,
        "Component ID not found in the ComponentList");

    static constexpr uint value =
        detail::IndexOfCompIdImpl<COMP_ID, 0, Components...>::value;
};

/**
 * @brief Checks if the given object (ElementProxy, ElementContainer, Mesh...)
 * has the component COMPONENT_ID available (present and enabled).
 *
 * @tparam COMPONENT_ID The component ID to check.
 * @param[in] obj The object to check.
 * @return true if the object has the component and it is enabled, false otherwise.
 *
 * @ingroup base
 */
template<uint COMPONENT_ID, typename T>
inline bool isComponentAvailableOn(const T& obj)
{
    if constexpr (HasComponent<COMPONENT_ID, T>) {
        if constexpr (requires { obj.template isEnabled<COMPONENT_ID>(); }) {
            return obj.template isEnabled<COMPONENT_ID>();
        }
        else if constexpr (requires { obj.template isComponentEnabled<COMPONENT_ID>(); }) {
            return obj.template isComponentEnabled<COMPONENT_ID>();
        }
        else if constexpr (requires { obj.container().template isEnabled<COMPONENT_ID>(); }) {
            return obj.container().template isEnabled<COMPONENT_ID>();
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_PREDICATES_H
