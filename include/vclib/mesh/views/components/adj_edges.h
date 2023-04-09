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

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H

#include <vclib/concepts/mesh.h>
#include <vclib/types.h>

namespace vcl::views {

namespace internal {

template<typename T>
concept CleanAdjEdgesConcept = comp::HasAdjacentEdges<RemoveConstRef<T>>;

struct AdjEdgesView
{
	constexpr AdjEdgesView() = default;

	template <CleanAdjEdgesConcept R>
	friend constexpr auto operator|(R&& r, AdjEdgesView)
	{
		if constexpr(IsPointer<R>)
			return r->adjEdges();
		else
			return r.adjEdges();
	}
};

} // namespace vcl::views::internal

inline constexpr internal::AdjEdgesView adjEdges;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H