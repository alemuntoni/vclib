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

#ifndef VCL_SPACE_GRID_REGULAR_GRID_H
#define VCL_SPACE_GRID_REGULAR_GRID_H

#include <vclib/iterators/space/grid/cell_iterator.h>
#include <vclib/space/box.h>
#include <vclib/views/view.h>

namespace vcl {

template<typename Scalar, int N>
class RegularGrid
{
public:
	static_assert(N > 0, "Number of dimensions of the regular grid must be > 1.");
	static const int DIM  = N;
	using ScalarType      = Scalar;
	using PointType       = Point<Scalar, N>;
	using CellCoord       = Point<uint, N>;
	using BBoxType        = Box<PointType>;

	using CellIterator = vcl::CellIterator<N>;
	using CellView     = vcl::View<CellIterator>;

	RegularGrid() = default;
	RegularGrid(const Point<Scalar, N>& min, const Point<Scalar, N>& max, const Point<uint, N>& size);
	RegularGrid(const Box<Point<Scalar, N>>& bbox, const Point<uint, N>& size);

	Point<Scalar, N> min() const;
	Point<Scalar, N> max() const;

	Scalar length(uint d) const;
	Point<Scalar, N> lengths() const;

	uint cellNumber(uint d) const;
	Point<uint, N> cellNumbers() const;

	uint indexOfCell(const CellCoord& c) const;
	CellCoord cellOfIndex(uint i) const;

	Scalar cellLength(uint d) const;
	Point<Scalar, N> cellLengths() const;
	Scalar cellDiagonal() const;

	uint cell(uint d, const Scalar& s) const;
	CellCoord cell(const Point<Scalar, N>& p) const;

	Point<Scalar, N> cellLowerCorner(const CellCoord& c) const;

	BBoxType cellBox(const CellCoord& c) const;

	CellIterator cellBegin() const;
	CellIterator cellBegin(const CellCoord& first, const CellCoord& last) const;
	CellIterator cellEnd() const;
	CellView     cells() const;
	CellView     cells(const CellCoord& first, const CellCoord& last) const;

protected:
	void set(const Box<Point<Scalar, N>>& box, const Point<uint, N>& size);

private:
	BBoxType       bbox;
	Point<uint, N> siz;
};

template<typename Scalar>
using RegularGrid2 = RegularGrid<Scalar, 2>;

template<typename Scalar>
using RegularGrid3 = RegularGrid<Scalar, 3>;

// deduction guides
template<PointConcept PointType, typename D>
RegularGrid(PointType, PointType, D) -> RegularGrid<typename PointType::ScalarType, PointType::DIM>;

template<PointConcept PointType>
vcl::Point<uint, PointType::DIM> bestGridSize(const PointType& lengths, uint nElements);

} // namespace vcl

#include "regular_grid.cpp"

#endif // VCL_SPACE_GRID_REGULAR_GRID_H