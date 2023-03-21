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

#include "edge_adj_face_iterator.h"

namespace vcl::internal {

template<typename FaceType, bool CNST>
EdgeAdjFaceIterator<FaceType, CNST>::EdgeAdjFaceIterator()
{
}

template<typename FaceType, bool CNST>
EdgeAdjFaceIterator<FaceType, CNST>::EdgeAdjFaceIterator(FaceType& f, uint edge) :
		current(&f), end(&f), v0(f.vertex(edge)), v1(f.vertexMod(edge+1))
{
}

template<typename FaceType, bool CNST>
bool EdgeAdjFaceIterator<FaceType, CNST>::operator==(const EdgeAdjFaceIterator& oi) const
{
	return current == oi.current && v0 == oi.v0 && v1 == oi.v1;
}

template<typename FaceType, bool CNST>
bool EdgeAdjFaceIterator<FaceType, CNST>::operator!=(const EdgeAdjFaceIterator& oi) const
{
	return !(*this == oi);
}

template<typename FaceType, bool CNST>
EdgeAdjFaceIterator<FaceType, CNST> EdgeAdjFaceIterator<FaceType, CNST>::operator++()
{
	assert(current);
	int edge = current->indexOfEdge(v0, v1);
	assert(edge >= 0);
	current = current->adjFace(edge);
	if (current == end || current == nullptr) {
		current = nullptr;
		v0 = nullptr;
		v1 = nullptr;
	}
	return *this;
}

template<typename FaceType, bool CNST>
EdgeAdjFaceIterator<FaceType, CNST> EdgeAdjFaceIterator<FaceType, CNST>::operator++(int)
{
	auto it = *this;
	++(*this);
	return it;
}

template<typename FaceType, bool CNST>
typename EdgeAdjFaceIterator<FaceType, CNST>::reference
EdgeAdjFaceIterator<FaceType, CNST>::operator*() const
{
	return current;
}

template<typename FaceType, bool CNST>
typename EdgeAdjFaceIterator<FaceType, CNST>::pointer
EdgeAdjFaceIterator<FaceType, CNST>::operator->() const
{
	return &current;
}

} // namespace vcl::internal