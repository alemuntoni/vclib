// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_TRANSLATE_GIZMO_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_TRANSLATE_GIZMO_BGFX_H

#include <vclib/algorithms/core.h>
#include <vclib/algorithms/mesh.h>
#include <vclib/bgfx/shapes/cone_shape.h>
#include <vclib/bgfx/shapes/cylinder_shape.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

namespace vcl {

class TranslateGizmoBGFX
{
    // Cylinder from Y=0 to Y=1
    CylinderShape mCylinder = CylinderShape(
        vcl::Point3d(0.0, 0.0, 0.0),
        vcl::Point3d(0.0, 1.0, 0.0),
        0.01,
        16);

    // Cone for the tip, from Y=1.0 to Y=1.2
    ConeShape mCone = ConeShape(
        vcl::Point3d(0.0, 1.0, 0.0),
        vcl::Point3d(0.0, 1.2, 0.0),
        0.03,
        0.0,
        16);

    // We can store states if needed
    vcl::Point3d mAnchorPoint3D = vcl::Point3d::Zero();

    static const uint64_t DRAW_STATE =
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_CULL_CW;

public:
    TranslateGizmoBGFX() = default;

    void draw(
        uint                  viewId,
        const vcl::Matrix44f& baseTransform,
        const vcl::Matrix44f& viewMatrix)
    {
        vcl::Point3f centerWorld =
            vcl::Point3f(0.0f, 0.0f, 0.0f) * baseTransform;
        vcl::Point3f centerView = centerWorld * viewMatrix;

        vcl::Point3f col0(viewMatrix(0, 0), viewMatrix(1, 0), viewMatrix(2, 0));
        float        viewScale = std::max(0.0001f, col0.norm());

        float depth       = std::max(0.1f, std::abs(centerView.z()));
        float visualScale = (depth / viewScale) * 0.15f;

        vcl::Matrix44f noScaleBase = baseTransform;
        noScaleBase.block<3, 1>(0, 0).normalize();
        noScaleBase.block<3, 1>(0, 1).normalize();
        noScaleBase.block<3, 1>(0, 2).normalize();

        vcl::Matrix44f scaleMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixScale(
            scaleMat, vcl::Point3f(visualScale, visualScale, visualScale));

        vcl::Matrix44f gizmoTransform = noScaleBase * scaleMat;

        // X Axis (Red) -> Rotate Y to X (around Z by -90 deg)
        vcl::Matrix44f rotX = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixRotation(
            rotX, vcl::Point3f(0, 0, 1), float(-M_PI / 2.0));
        vcl::Matrix44f xTransform = gizmoTransform * rotX;
        mCylinder.draw(viewId, vcl::Color::Red, xTransform, DRAW_STATE);
        mCone.draw(viewId, vcl::Color::Red, xTransform, DRAW_STATE);

        // Y Axis (Green) -> Already along Y
        vcl::Matrix44f yTransform = gizmoTransform;
        mCylinder.draw(viewId, vcl::Color::Green, yTransform, DRAW_STATE);
        mCone.draw(viewId, vcl::Color::Green, yTransform, DRAW_STATE);

        // Z Axis (Blue) -> Rotate Y to Z (around X by +90 deg)
        vcl::Matrix44f rotZ = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixRotation(
            rotZ, vcl::Point3f(1, 0, 0), float(M_PI / 2.0));
        vcl::Matrix44f zTransform = gizmoTransform * rotZ;
        mCylinder.draw(viewId, vcl::Color::Blue, zTransform, DRAW_STATE);
        mCone.draw(viewId, vcl::Color::Blue, zTransform, DRAW_STATE);
    }

    void drawId(
        uint /*viewId*/,
        const vcl::Matrix44f& /*baseTransform*/,
        const vcl::Matrix44f& /*viewMatrix*/)
    {
        // No pickable visualization yet (the user picks the mesh directly for
        // translation currently)
    }

    void calculateAnchor(std::shared_ptr<AbstractDrawableMesh> mesh)
    {
        mAnchorPoint3D = mesh->meshProvider().boundingBox().center();
        // The actual depth will be calculated in the parent using this point
    }

    vcl::Point3d anchorPointInWorld(
        std::shared_ptr<AbstractDrawableMesh> mesh) const
    {
        // Anchor is just the center of the bounding box
        return mAnchorPoint3D *
               mesh->meshProvider().transformMatrix().template cast<double>();
    }

    vcl::Matrix44d calculateNewTransform(
        const vcl::Point3d&   newPoint3D,
        const vcl::Point3d&   oldPoint3D,
        const vcl::Matrix44d& originalMatrix)
    {
        Point3d delta = newPoint3D - oldPoint3D;

        Matrix44d translation = Matrix44d::Identity();
        vcl::setTransformMatrixTranslation(translation, delta);

        return translation * originalMatrix;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_TRANSLATE_GIZMO_BGFX_H
