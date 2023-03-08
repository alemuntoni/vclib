/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MATH_MIN_MAX_H
#define VCL_MATH_MIN_MAX_H

#include <vclib/space/point.h>

namespace vcl {

template<typename T>
constexpr auto min(const T& el1, const T& el2);

template <typename Head, typename... Tail>
constexpr auto min(const Head& head0, const Head& head1, const Tail&... tail) requires(sizeof...(tail) > 0);

template<typename T>
constexpr auto max(const T& el1, const T& el2);

template <typename Head, typename... Tail>
constexpr auto max(const Head& head0, const Head& head1, const Tail&... tail) requires(sizeof...(tail) > 0);

// min specializations

template<PointConcept PointType>
constexpr auto min(const PointType& p1, const PointType& p2);

// max specializations

template<PointConcept PointType>
constexpr auto max(const PointType& p1, const PointType& p2);

} // namespace vcl

#include "min_max.cpp"

#endif // VCL_MATH_MIN_MAX_H