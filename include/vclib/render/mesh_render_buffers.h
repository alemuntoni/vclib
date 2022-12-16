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

#ifndef VCLIB_RENDER_MESH_RENDER_BUFFERS_H
#define VCLIB_RENDER_MESH_RENDER_BUFFERS_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshRenderBuffers
{
public:
	MeshRenderBuffers();
	MeshRenderBuffers(const MeshType& m);

	unsigned int vertexNumber() const;
	unsigned int triangleNumber() const;
	const vcl::Point3d& bbMin() const;
	const vcl::Point3d& bbMax() const;

	const double* vertexBufferData() const;
	const int* triangleBufferData() const;
	const double* vertexNormalBufferData() const;
	const float* vertexColorBufferData() const;
	const double* triangleNormalBufferData() const;
	const float* triangleColorBufferData() const;

private:
	uint nv = 0;
	uint nt = 0;
	std::vector<double> verts;
	std::vector<int> tris;
	std::vector<double> vNormals;
	std::vector<float> vColors;
	std::vector<double> tNormals;
	std::vector<float> tColors;
	vcl::Point3d bbmin, bbmax;

	void fillVertices(const MeshType& m);
	void fillTriangles(const MeshType& m);
};

} // namespace vcl

#include "mesh_render_buffers.cpp"

#endif // VCLIB_RENDER_MESH_RENDER_BUFFERS_H