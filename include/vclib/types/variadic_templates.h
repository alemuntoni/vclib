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

#ifndef VCL_TYPES_VARIADIC_TEMPLATES_H
#define VCL_TYPES_VARIADIC_TEMPLATES_H

#ifndef VCLIB_WITH_MODULES
#include "base.h"

#include <tuple>
#include <typeindex>
#endif

namespace vcl {

/**
 * @brief A simple structure that wraps a list of variadic templates, without
 * instantiating anything. Useful when you need to wrap a list of types, and
 * consider them as a single type.
 */
template<typename... Args>
struct TypeWrapper
{
    static constexpr uint size() { return sizeof...(Args); }
};

/**
 * @brief Get the first type of a pack of types (variadic templates) or a
 * TypeWrapper.
 *
 * Usage:
 * @code{.cpp}
 * using ResType = FirstType<int, float, double>::type;
 * static_assert(std::is_same<ResType, int>::value, "");
 * @endcode
 */
template<typename... Args>
struct FirstType
{
    using type = std::tuple_element<0, std::tuple<Args...>>::type;
};

/**
 * @copydoc FirstType
 */
template<typename... Args>
struct FirstType<TypeWrapper<Args...>>
{
    using type = std::tuple_element<0, std::tuple<Args...>>::type;
};

/**
 * @brief Alias for the type of the first type in a pack of types.
 *
 * Usage:
 * @code{.cpp}
 * using ResType = FirstTypeT<int, float, double>;
 * static_assert(std::is_same<ResType, int>::value, "");
 * @endcode
 */
template<typename... Args>
using FirstTypeT = typename FirstType<Args...>::type;

/**
 * @brief Function that returns the index of a Type T in a pack of types
 * (variadic templates). The pack is composed of U and Us...
 */
// https://stackoverflow.com/a/71477756/5851101
template<typename T, typename U, typename... Us>
constexpr uint indexInTypePack()
{
    if constexpr (std::is_same_v<T, U>) {
        return 0;
    }
    else {
        if constexpr (sizeof...(Us)) {
            // there is at least another type to check
            constexpr uint res =
                indexInTypePack<T, Us...>(); // look in the rest of the types
            if constexpr (res == UINT_NULL)  // not found in the rest
                return UINT_NULL;
            else
                return 1 + res; // found
        }
        else { // not found
            return UINT_NULL;
        }
    }
}

/**
 * @brief Function that returns the index of the type having the given
 * type_index in a pack of types (variadic templates).
 * The pack is composed of U and Us...
 *
 * @param ti: the type_index of the type to search.
 */
template<typename U, typename... Us>
uint indexInTypePack(std::type_index ti)
{
    if (ti == typeid(U)) {
        return 0;
    }
    else {
        if constexpr (sizeof...(Us)) {
            // there is at least another type to check
            uint res = indexInTypePack<Us...>(ti);
            if (res == UINT_NULL)
                return UINT_NULL;
            else
                return 1 + res;
        }
        else { // not found
            return UINT_NULL;
        }
    }
}

template<typename... Args>
uint indexInTypePack(std::type_index ti, TypeWrapper<Args...>)
{
    return indexInTypePack<Args...>(ti);
}

template<typename T, typename... Us>
struct IndexInTypes
{
    static constexpr uint value = indexInTypePack<T, Us...>();
};

template<typename T, typename... Us>
struct IndexInTypes<T, TypeWrapper<Us...>>
{
    static constexpr uint value = indexInTypePack<T, Us...>();
};

template<typename... Args>
struct NumberOfTypes
{
    static constexpr uint value = sizeof...(Args);
};

template<typename... Args>
struct NumberOfTypes<TypeWrapper<Args...>> : public NumberOfTypes<Args...>
{
};

/**
 * @brief Allows to apply a function to each type in a variadic template pack.
 *
 * Usage:
 * @code{.cpp}
 * // declare the function as a lambda - will do something with T
 * auto f = []<typename T>()
 * {
 *     std::cout << typeid(T).name() << std::endl;
 * };
 *
 * // call a function for each type in a parameter pack
 * vcl::ForEachType<int, float, double>::apply(f);
 * @endcode
 */
template<typename... T>
struct ForEachType
{
    template<typename F>
    static void apply(F&& f)
    {
        (f.template operator()<T>(), ...);
    }
};

template<typename... T>
struct ForEachType<TypeWrapper<T...>> : public ForEachType<T...>
{
};

/* Remove all types that do not satisfy a condition, and get them as a
 * TypeWrapper. */

namespace detail {

template<typename, typename>
struct TypeWrapperConstructor;

template<typename T, typename... Args>
struct TypeWrapperConstructor<T, TypeWrapper<Args...>>
{
    using type = TypeWrapper<T, Args...>;
};

} // namespace detail

template<template<class> class, typename...>
struct FilterTypesByCondition
{
    using type = TypeWrapper<>;
};

/**
 * @brief Removes all types that do not satisfy a condition, and get them as a
 * tuple.
 *
 * Usage:
 *
 * @code{.cpp}
 * using ResTypes =
 *     FilterTypesByCondition<std::is_integral, int, float, double, char>::type;
 * static_assert(std::is_same<ResTypes, TypeWrapper<int, char>>::value, "");
 * @endcode
 *
 * ResTuple will be a TypeWrapper<int, char> (int and char are the only integral
 * types)
 */
template<template<class> class Pred, typename Head, typename... Tail>
struct FilterTypesByCondition<Pred, Head, Tail...>
{
    using type = std::conditional<
        Pred<Head>::value,
        typename detail::TypeWrapperConstructor<
            Head,
            typename FilterTypesByCondition<Pred, Tail...>::type>::type,
        typename FilterTypesByCondition<Pred, Tail...>::type>::type;
};

// TypeWrapper specialization
template<template<class> class Pred, typename... Tail>
struct FilterTypesByCondition<Pred, TypeWrapper<Tail...>>
{
    using type = FilterTypesByCondition<Pred, Tail...>::type;
};

/**
 * @brief Its value is set to true if there is at least one type in the given
 * pack Args... that satisfies the given condition
 *
 * Usage:
 *
 * @code{.cpp}
 * // there is a type (int) that is integral
 * static const bool res =
 *     TypesContainConditionType<std::is_integral, int, float, double>::value;
 * static_assert(res == true, "");
 *
 * static const bool res2
 *     = TypesContainConditionType<std::is_integral, float, double>::value;
 * static_assert(res2 != true, "");
 * @endcode
 */
template<template<class> class Pred, typename... Args>
struct TypesContainConditionType
{
private:
    using ResTypes = FilterTypesByCondition<Pred, Args...>::type;

public:
    static constexpr bool value = NumberOfTypes<ResTypes>::value != 0;
};

// TypeWrapper specialization
template<template<class> class Pred, typename... Args>
struct TypesContainConditionType<Pred, TypeWrapper<Args...>>
{
    using type = TypesContainConditionType<Pred, Args...>::type;
};

/**
 * @brief The the first type of a pack that satisties the given condition
 *
 * Usage:
 * @code{.cpp}
 * // the first integral type is char
 * using ResType =
 *     GetTypeByCondition<std::is_integral, float, double, char, int>::type;
 * static_assert(std::is_same<ResType, char>::value, "");
 * @endcode
 */
template<template<class> class Pred, typename... Args>
struct GetTypeByCondition
{
private:
    using ResTypes = FilterTypesByCondition<Pred, Args...>::type;

public:
    using type = FirstTypeT<ResTypes>;
};

// TypeWrapper specialization
template<template<class> class Pred, typename... Args>
struct GetTypeByCondition<Pred, TypeWrapper<Args...>>
{
    using type = GetTypeByCondition<Pred, Args...>::type;
};

} // namespace vcl

#endif // VCL_TYPES_VARIADIC_TEMPLATES_H
