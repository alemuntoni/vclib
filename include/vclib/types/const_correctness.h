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

#ifndef VCL_TYPES_CONST_CORRECTNESS_H
#define VCL_TYPES_CONST_CORRECTNESS_H

#ifndef VCLIB_WITH_MODULES
#include <type_traits>
#endif

namespace vcl {

/*
 * Utility type that makes possible to treat const pointers in a templated class
 * that can treat a both const and non-const pointer type.
 */
template<typename T>
struct MakeConstPointer
{
    typedef T type;
};

template<typename T>
struct MakeConstPointer<T*>
{
    typedef const T* type;
};

/*
 * Full deduction for the possibility to re-use same code for const and
 * non-const member functions https://stackoverflow.com/a/47369227/5851101
 */

template<typename T>
constexpr T& asConst(const T& value) noexcept
{
    return const_cast<T&>(value);
}

template<typename T>
constexpr T* asConst(const T* value) noexcept
{
    return const_cast<T*>(value);
}

template<typename T>
constexpr T* asConst(T* value) noexcept
{
    return value;
}

template<typename T>
void asConst(const T&&) = delete;

} // namespace vcl

#endif // VCL_TYPES_CONST_CORRECTNESS_H
