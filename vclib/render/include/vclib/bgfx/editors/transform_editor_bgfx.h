// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H

#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>
#include <vclib/render/settings/transform_editor_settings.h>

namespace vcl {

/**
 * @brief The TransformEditorBGFX class provides a viewer editor for
 * interactively modifying the transformation matrix of a mesh.
 *
 * @ingroup bgfx_editors
 */
template<typename ViewerDrawer>
class TransformEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;
    TransformEditorSettings mSettings;

    bool      mTransformInProgress = false;
    Point2d   mStartMousePos;
    Point3d   mAnchorPoint3D;
    double    mAnchorDepth  = 0.0;
    ushort    mCurrentObjId = USHORT_NULL;
    Matrix44d mStartTransform;

public:
    TransformEditorBGFX() = default;

    std::string name() const override { return "Transform"; }

    TransformEditorSettings& settings() override { return mSettings; }

    const TransformEditorSettings& settings() const override
    {
        return mSettings;
    }

    void loadSettings(const nlohmann::json& j) override
    {
        mSettings.loadSettings(j);
    }

    void saveSettings(nlohmann::json& j) const override
    {
        mSettings.saveSettings(j);
    }

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
        if (!active) {
            mTransformInProgress = false;
            mCurrentObjId        = USHORT_NULL;
        }
    }

    void draw(uint viewId) override
    {
        if (!Base::isActive())
            return;
        // Gizmos drawing will go here
    }

    void drawId(uint viewId) override
    {
        if (!Base::isActive())
            return;
        // Gizmos ID drawing will go here
    }

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        if (!Base::isActive() || button != vcl::MouseButton::LEFT) {
            return false;
        }

        if (mSettings.mode != TransformEditorSettings::Mode::TRANSLATE) {
            return false;
        }

        mTransformInProgress = true;
        mStartMousePos       = Point2d(x, y);
        mCurrentObjId        = USHORT_NULL;

        this->viewerReadElementIdRequest(
            x, y, [this, x, y](ushort objId, ushort, uint) {
                if (!mTransformInProgress)
                    return; // aborted
                if (objId == USHORT_NULL) {
                    mTransformInProgress = false; // clicked on background
                    return;
                }

                auto mesh = findMesh(objId);
                if (!mesh) {
                    mTransformInProgress = false;
                    return;
                }

                auto& provider = mesh->meshProvider();
                if (!provider.hasTransformMatrix()) {
                    mTransformInProgress = false;
                    return;
                }

                if (mSettings.editMode == EditorSettings::EditMode::CURRENT_OBJECT) {
                    if (objId != this->drawList()->selectedObjectId()) {
                        mTransformInProgress = false;
                        return;
                    }
                }

                mCurrentObjId   = objId;
                mStartTransform = provider.transformMatrix();
                mAnchorPoint3D  = mesh->boundingBox().center();
                mAnchorDepth    = project(mAnchorPoint3D).z();
            });

        // Consume the event so we don't start rotating the trackball
        return true;
    }

    bool onMouseMove(double x, double y, const vcl::KeyModifiers& modifiers)
        override
    {
        if (!mTransformInProgress)
            return false;

        if (mCurrentObjId == USHORT_NULL) {
            // Still waiting for id callback, or clicked background.
            // If waiting, consume to prevent rotation.
            return true;
        }

        auto mesh = findMesh(mCurrentObjId);
        if (!mesh)
            return true;

        Point3d newPoint3D = unproject(x, y, mAnchorDepth);
        Point3d oldPoint3D =
            unproject(mStartMousePos.x(), mStartMousePos.y(), mAnchorDepth);
        Matrix44d view = this->viewerViewMatrix().template cast<double>();
        Matrix44d proj = this->viewerProjectionMatrix().template cast<double>();
        Matrix44d inv  = (proj * view).inverse();

        Point3d delta = newPoint3D - oldPoint3D;

        // Compute translation matrix
        Matrix44d translation = Matrix44d::Identity();
        translation(0, 3)     = delta.x();
        translation(1, 3)     = delta.y();
        translation(2, 3)     = delta.z();

        mesh->meshProvider().setTransformMatrix(translation * mStartTransform);
        this->viewerUpdate();

        return true;
    }

    bool onMouseRelease(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        if (mTransformInProgress && button == vcl::MouseButton::LEFT) {
            mTransformInProgress = false;

            auto mesh = findMesh(mCurrentObjId);
            if (mesh) {
                mesh->notifyMeshUpdated();
            }

            mCurrentObjId        = USHORT_NULL;
            return true;
        }
        return false;
    }

private:
    std::shared_ptr<AbstractDrawableMesh> findMesh(ushort objId) const
    {
        return std::dynamic_pointer_cast<AbstractDrawableMesh>(
            this->drawList()->at(objId));
    }

    Point3d project(const Point3d& pt) const
    {
        Matrix44d view = this->viewerViewMatrix().template cast<double>();
        Matrix44d proj = this->viewerProjectionMatrix().template cast<double>();
        auto      size = this->viewerCanvasSize();

        Point4d p(pt.x(), pt.y(), pt.z(), 1.0);
        p = proj * (view * p);
        if (p.w() != 0.0)
            p /= p.w();

        return Point3d(
            (p.x() + 1.0) * 0.5 * size.x(),
            (1.0 - p.y()) * 0.5 * size.y(),
            p.z());
    }

    Point3d unproject(double x, double y, double z) const
    {
        Matrix44d view = this->viewerViewMatrix().template cast<double>();
        Matrix44d proj = this->viewerProjectionMatrix().template cast<double>();
        auto      size = this->viewerCanvasSize();

        Point4d p(
            (x / size.x()) * 2.0 - 1.0,
            (1.0 - (y / size.y())) * 2.0 - 1.0,
            z,
            1.0);

        Matrix44d inv = (proj * view).inverse();
        Point4d   res = inv * p;
        if (res.w() != 0.0)
            res /= res.w();
        return res.head<3>();
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
