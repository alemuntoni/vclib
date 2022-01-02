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

#ifndef VCL_ALGORITHMS_COLOR_H
#define VCL_ALGORITHMS_COLOR_H

#include <set>

#include <vclib/space/color.h>

namespace vcl {

template<typename MeshType>
void setPerVertexColor(MeshType& m, vcl::Color c = vcl::Color::White, bool onlySelected = false);

template<typename MeshType>
void setPerFaceColor(MeshType& m, vcl::Color c = vcl::Color::White, bool onlySelected = false);

template<typename MeshType>
void setPerVertexColorFromFaceColor(MeshType& m);

template<typename MeshType>
void setPerFaceColorFromVertexColor(MeshType& m);

template<typename MeshType>
void setPerVertexColorFromScalar(
	MeshType&                                 m,
	vcl::Color::ColorMap                      colorMap  = vcl::Color::RedBlue,
	typename MeshType::VertexType::ScalarType minScalar = 0,
	typename MeshType::VertexType::ScalarType maxScalar = 0);

template<typename MeshType>
void setPerFaceColorFromScalar(
	MeshType&                               m,
	vcl::Color::ColorMap                    colorMap  = vcl::Color::RedBlue,
	typename MeshType::FaceType::ScalarType minScalar = 0,
	typename MeshType::FaceType::ScalarType maxScalar = 0);

template<typename MeshType>
void setPerVertexColorFromFaceBorderFlag(
	MeshType&  m,
	vcl::Color borderColor   = vcl::Color::Blue,
	vcl::Color internalColor = vcl::Color::White,
	vcl::Color mixColor      = vcl::Color::Cyan);

template<typename MeshType>
void setPerFaceColorFromConnectedComponents(
	MeshType& m,
	const std::vector<std::set<uint>>& connectedComponents);

template<typename MeshType>
void setPerFaceColorFromConnectedComponents(MeshType& m);

template<typename MeshType>
void setPerFaceColorScattering(MeshType& m, uint nColors = 50, bool checkFauxEdges = true);

template<typename MeshType, typename PointType>
void setPerVertexColorPerlinNoise(
	MeshType& m,
	PointType period,
	PointType offset     = PointType(0, 0, 0),
	bool      onSelected = false);

template<typename MeshType, typename PointType>
void setPerVertexPerlinColor(
	MeshType& m,
	double period,
	PointType offset  = PointType(0, 0, 0),
	Color color1 = Color::Black,
	Color color2 = Color::White,
	bool onSelected = false);

} // namespace vcl

#include "color.cpp"

#endif // VCL_ALGORITHMS_COLOR_H