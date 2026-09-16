// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_TRANSLATE_GIZMO_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_TRANSLATE_GIZMO_BGFX_H

#include <vclib/algorithms/core.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

namespace vcl {

class TranslateGizmoBGFX
{
    // For now, no visual primitives.
    // In Step 6 or later, we can add the 3-axis arrows here.

    // We can store states if needed
    vcl::Point3d mAnchorPoint3D = vcl::Point3d::Zero();

public:
    TranslateGizmoBGFX() = default;

    void draw(uint /*viewId*/, const vcl::Matrix44f& /*gizmoTransform*/)
    {
        // No visualization yet
    }

    void drawId(uint /*viewId*/, const vcl::Matrix44f& /*gizmoTransform*/)
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
