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
#include <vclib/render/undo_redo/transform_undo_redo_action.h>

#include <vclib/algorithms/core.h>

#include <vector>

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

    bool    mTransformInProgress = false;
    Point2d mStartMousePos;
    Point3d mAnchorPoint3D;
    double  mAnchorDepth  = 0.0;
    ushort  mCurrentObjId = USHORT_NULL;

    std::vector<TransformUndoRedoAction::MeshState> mPreTransformStates;

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

                auto dl = this->drawList();
                if (mSettings.editMode ==
                    EditorSettings::EditMode::CURRENT_OBJECT) {
                    if (objId != dl->selectedObjectId()) {
                        mTransformInProgress = false;
                        return;
                    }
                }

                mCurrentObjId  = objId;
                mAnchorPoint3D = mesh->boundingBox().center();
                mAnchorDepth   = project(mAnchorPoint3D).z();

                savePreTransformStates(mCurrentObjId);
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

        for (auto& state : mPreTransformStates) {
            if (auto lock = state.obj.lock()) {
                if (auto* m = dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                    m->meshProvider().setTransformMatrix(
                        translation * state.transformMatrix);
                }
            }
        }

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

            for (auto& state : mPreTransformStates) {
                if (auto lock = state.obj.lock()) {
                    if (auto* mesh =
                            dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                        mesh->notifyMeshUpdated();
                    }
                }
            }

            finalizeTransformAction();

            mCurrentObjId = USHORT_NULL;
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
        Matrix44d pv   = proj * view;
        auto      size = this->viewerCanvasSize();

        Point4d viewport(0.0, 0.0, size.x(), size.y());
        Point3d res = projectScreenPosition(pt, pv, viewport, false);

        res.y() = size.y() - res.y();

        return res;
    }

    Point3d unproject(double x, double y, double z) const
    {
        Matrix44d view = this->viewerViewMatrix().template cast<double>();
        Matrix44d proj = this->viewerProjectionMatrix().template cast<double>();
        Matrix44d pv   = proj * view;
        auto      size = this->viewerCanvasSize();

        Point4d viewport(0.0, 0.0, size.x(), size.y());
        Point3d screenPos(x, size.y() - y, z);

        return unprojectScreenPosition(screenPos, pv, viewport, false);
    }

    void savePreTransformStates(ushort activeObjId)
    {
        mPreTransformStates.clear();
        auto dl = Base::drawList();

        if (activeObjId != USHORT_NULL && activeObjId < dl->size()) {
            auto el = dl->at(activeObjId);
            if (auto p = dynamic_cast<AbstractDrawableMesh*>(el.get())) {
                if (p->meshProvider().hasTransformMatrix()) {
                    TransformUndoRedoAction::MeshState state;
                    state.obj             = el;
                    state.transformMatrix = p->meshProvider().transformMatrix();
                    mPreTransformStates.push_back(std::move(state));
                }
            }
        }
    }

    void finalizeTransformAction()
    {
        bool changed = false;
        for (auto& state : mPreTransformStates) {
            if (auto lock = state.obj.lock()) {
                if (auto* mesh =
                        dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                    if (state.transformMatrix !=
                        mesh->meshProvider().transformMatrix()) {
                        changed = true;
                        break;
                    }
                }
            }
        }

        if (changed) {
            auto action = std::make_unique<TransformUndoRedoAction>(
                std::move(mPreTransformStates));
            Base::pushUndoRedoAction(std::move(action));
            Base::viewerUpdate();
        }
        mPreTransformStates.clear();
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
