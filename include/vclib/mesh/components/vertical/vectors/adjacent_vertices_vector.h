/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_VECTOR_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_VECTOR_H

#include "../../detection/adjacent_vertices_detection.h"

#include "generic_component_vector.h"

namespace vcl::internal {

template<typename, typename = void>
class AdjacentVerticesVector
{
public:
	void clear() {}
	void resize(uint) {}
	void reserve(uint) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class AdjacentVerticesVector<
	T,
	std::enable_if_t<comp::hasOptionalAdjacentVertices<T>()>> :
		private GenericComponentVector<typename T::AdjVertsContainer>
{
private:
	using AdjVertsContainer = typename T::AdjVertsContainer;
	using Base = GenericComponentVector<AdjVertsContainer>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool             isAdjacentVerticesEnabled() const { return Base::isEnabled(); };
	void             enableAdjacentVertices(uint size) { Base::enable(size); }
	void             disableAdjacentVertices() { Base::disable(); }
	AdjVertsContainer&       adjVerts(uint i) { return Base::at(i); }
	const AdjVertsContainer& adjVerts(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_VECTOR_H