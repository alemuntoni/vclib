// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SHAPES_CYLINDER_SHAPE_H
#define VCL_BGFX_SHAPES_CYLINDER_SHAPE_H

#include <vclib/algorithms/mesh/create.h>
#include <vclib/bgfx/shapes/shape.h>

namespace vcl {

/**
 * @brief A basic 3D cylinder shape.
 *
 * Creates a cylinder mesh extending from -height/2 to height/2 along the Y axis,
 * with a default radius of 1.
 */
class CylinderShape : public Shape
{
public:
    CylinderShape() : CylinderShape(1.0, 1.0) {}

    CylinderShape(
        double                radius,
        double                height,
        uint                  subdivisions = 36,
        const vcl::Matrix44d& offset       = vcl::Matrix44d::Identity()) :
            Shape([&]() {
                auto mesh = vcl::createCylinder<vcl::TriMesh>(
                    radius, height, subdivisions);
                if (offset != vcl::Matrix44d::Identity()) {
                    vcl::applyTransformMatrix(mesh, offset);
                    vcl::updatePerVertexNormals(mesh);
                }
                return mesh;
            }())
    {
    }
};

} // namespace vcl

#endif // VCL_BGFX_SHAPES_CYLINDER_SHAPE_H
