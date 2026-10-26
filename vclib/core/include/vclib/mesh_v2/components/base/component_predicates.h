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

template<uint COMP_ID, typename T>
inline constexpr bool HasComponentImpl = requires {
    requires std::remove_cvref_t<T>::COMPONENT_ID == COMP_ID;
};

template<uint COMP_ID, typename... Components>
inline constexpr bool
    HasComponentImpl<COMP_ID, vcl::TypeWrapper<Components...>> =
    (HasComponentImpl<COMP_ID, Components> || ...);

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
        HasComponentImpl<COMP_ID, First> ?
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
 * @brief Concept that checks whether a component Comp is tied to the vertex
 * count (has static constexpr bool TIED_TO_VERTEX_COUNT = true).
 */
template<typename Comp>
concept IsTiedToVertexCount = requires {
    { std::remove_cvref_t<Comp>::TIED_TO_VERTEX_COUNT } -> std::convertible_to<bool>;
} && std::remove_cvref_t<Comp>::TIED_TO_VERTEX_COUNT;

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
 * @brief Trait to extract the Component type with the given COMP_ID from a
 * TypeWrapper or from an object that contains components (ElementProxy,
 * ElementContainer, Mesh, etc.).
 */
template<uint COMP_ID, typename ComponentListWrapper>
struct ComponentType;

template<uint COMP_ID>
struct ComponentType<COMP_ID, vcl::TypeWrapper<>>
{
    using type = void;
};

template<uint COMP_ID, typename First, typename... Rest>
struct ComponentType<COMP_ID, vcl::TypeWrapper<First, Rest...>>
{
    using type = std::conditional_t<
        First::COMPONENT_ID == COMP_ID,
        First,
        typename ComponentType<COMP_ID, vcl::TypeWrapper<Rest...>>::type>;
};

template<uint COMP_ID, typename T>
using ComponentTypeT = typename ComponentType<
    COMP_ID,
    typename detail::ComponentListHelper<T>::type>::type;

/**
 * @brief Concept that checks whether a component type is optional
 * (has static constexpr bool IS_OPTIONAL = true).
 */
template<typename Comp>
concept IsOptionalComponent = requires {
    requires std::remove_cvref_t<Comp>::IS_OPTIONAL;
};

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) contains an
 * optional component with the given COMP_ID.
 */
template<uint COMP_ID, typename T>
concept HasOptionalComponent =
    HasComponent<COMP_ID, T> &&
    IsOptionalComponent<ComponentTypeT<COMP_ID, T>>;


/**
 * @brief Concept that checks whether a component type provides a default value,
 * either through a static constexpr defaultValue() member function or a
 * static constexpr DEFAULT_VALUE variable.
 *
 * @tparam Comp: The component type to check.
 *
 * @ingroup components_concepts
 */
template<typename Comp>
concept HasDefaultValue = requires {
    {
        std::remove_cvref_t<Comp>::defaultValue()
    } -> std::convertible_to<typename std::remove_cvref_t<Comp>::DataType>;
} || requires {
    {
        std::remove_cvref_t<Comp>::DEFAULT_VALUE
    } -> std::convertible_to<typename std::remove_cvref_t<Comp>::DataType>;
};

/**
 * @brief Returns the default initial value for a component type satisfying
 * HasDefaultValue.
 *
 * @tparam Comp: The component type.
 * @return The default value.
 */
template<typename Comp>
constexpr auto defaultValue()
{
    using PureComp = std::remove_cvref_t<Comp>;
    if constexpr (requires {
                      {
                          PureComp::defaultValue()
                      } -> std::convertible_to<typename PureComp::DataType>;
                  }) {
        return PureComp::defaultValue();
    }
    else if constexpr (requires {
                           {
                               PureComp::DEFAULT_VALUE
                           } -> std::convertible_to<
                               typename PureComp::DataType>;
                       }) {
        return PureComp::DEFAULT_VALUE;
    }
}

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_BASE_COMPONENT_PREDICATES_H
