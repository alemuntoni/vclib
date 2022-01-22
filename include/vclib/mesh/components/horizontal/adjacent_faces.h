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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_H

#include "../detection/adjacent_faces_detection.h"
#include "internal/element_references.h"

namespace vcl::comp {

/**
 * @brief The AdjacentFaces class is a container of Face references. It could be used by any
 * Element to save adjacencies information (also the Face element itself).
 *
 * It is a random access container having static or dynamic size, depending on the value of N (a
 * negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentFaces component, you'll be
 * able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * v.adjFacesNumber();
 * @endcode
 *
 * @note If this component is part of a Face Element, the number of Adjacent Faces is tied to the
 * Vertex Number of the Face, therefore all the members that allows to modify the number of
 * Adjacent Faces in case of dynamic size won't be available on Face Elements.
 */
template<typename Face, int N>
class AdjacentFaces : protected internal::ElementReferences<Face, N>, public AdjacentFacesTriggerer
{
	using Base = internal::ElementReferences<Face, N>;

public:
	/// Static size of the container. If the container is dynamic, this value will be negative and
	/// you should use the adjFacesNumber() member function.
	static const int ADJ_FACE_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using AdjacentFaceIterator           = typename Base::GCIterator;
	using ConstAdjacentFaceIterator      = typename Base::ConstGCIterator;
	using AdjacentFaceRangeIterator      = typename Base::GCRangeIterator;
	using ConstAdjacentFaceRangeIterator = typename Base::ConstGCRangeIterator;

	/* Constructor */

	AdjacentFaces();

	/* Member functions */

	uint adjFacesNumber() const;

	Face*&      adjFace(uint i);
	const Face* adjFace(uint i) const;
	Face*&      adjFaceMod(int i);
	const Face* adjFaceMod(int i) const;

	void setAdjFace(Face* f, uint i);
	void setAdjFaces(const std::vector<Face*>& list);

	bool containsAdjFace(const Face* f) const;

	AdjacentFaceIterator findAdjFace(const Face* f);
	ConstAdjacentFaceIterator findAdjFace(const Face* f) const;

	int indexOfAdjFace(const Face* f) const;

	/* Member functions specific for vector */

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeAdjFaces(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushAdjFace(Face* f);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insertAdjFace(uint i, Face* f);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseAdjFace(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearAdjFaces();

	/* Iterator Member functions */

	AdjacentFaceIterator           adjFaceBegin();
	AdjacentFaceIterator           adjFaceEnd();
	ConstAdjacentFaceIterator      adjFaceBegin() const;
	ConstAdjacentFaceIterator      adjFaceEnd() const;
	AdjacentFaceRangeIterator      adjFaces();
	ConstAdjacentFaceRangeIterator adjFaces() const;

protected:
	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

	template <typename Element>
	void importFrom(const Element& e);

	template<typename Element, typename ElFType>
	void
	importFaceReferencesFrom(const Element& e, Face* base, const ElFType* ebase);

private:
	template<typename Element, typename ElFType>
	void
	importReferencesFrom(const Element& e, Face* base, const ElFType* ebase);
};

} // namespace vcl::comp

#include "adjacent_faces.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_H