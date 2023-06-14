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

#ifndef VCL_VIEWS_MESH_ELEMENTS_FACE_H
#define VCL_VIEWS_MESH_ELEMENTS_FACE_H

#include <vclib/concepts/mesh.h>

namespace vcl::views {
namespace internal {

template<typename T>
concept CleanFaceMeshConcept = FaceMeshConcept<std::remove_cvref_t<T>>;

struct FacesView
{
	constexpr FacesView() = default;

	template <CleanFaceMeshConcept R>
	friend constexpr auto operator|(R&& r, FacesView)
	{
		return r.faces();
	}
};

} // namespace vcl::views::internal

/**
 * @brief A view that allows to iterate overt the Face elements of an object.
 *
 * This view can be applied to objects having type that satisfies the FaceMeshConcept
 *
 * @ingroup views
 */
inline constexpr internal::FacesView faces;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_ELEMENTS_FACE_H