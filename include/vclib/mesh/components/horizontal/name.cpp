/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "name.h"

namespace vcl::comp {

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
inline std::string& Name::name()
{
	return n;
}

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
inline const std::string& Name::name() const
{
	return n;
}

template<typename Element>
void Name::importFrom(const Element &e)
{
	if constexpr(HasName<Element>) {
		n = e.name();
	}
}

} // namespace vcl::comp