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

#ifndef VCL_CONCEPTS_POINTERS_H
#define VCL_CONCEPTS_POINTERS_H

#include "const_correctness.h"

namespace vcl {

/*
 * Utility Pointer concept to check if a type is a Pointer
 */
template<typename T>
concept IsPointer = std::is_pointer_v<std::remove_reference_t<T>>;

/*
 * Utility Pointer concept to check if it is a pointer to const object
 * https://stackoverflow.com/a/37370281/5851101
 */
template<typename T>
concept IsPointerToConst = IsPointer<T> && IsConst<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_POINTERS_H