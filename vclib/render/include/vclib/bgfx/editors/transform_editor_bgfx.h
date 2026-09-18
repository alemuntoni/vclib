// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H

#include "transform_editor/rotate_gizmo_bgfx.h"
#include "transform_editor/scale_gizmo_bgfx.h"
#include "transform_editor/translate_gizmo_bgfx.h"

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
    double  mAnchorDepth  = 0.0;
    ushort  mCurrentObjId = USHORT_NULL;

    std::vector<TransformUndoRedoAction::MeshState> mPreTransformStates;

    TranslateGizmoBGFX mTranslateGizmo;
    ScaleGizmoBGFX     mScaleGizmo;
    RotateGizmoBGFX    mRotateGizmo;

    enum class ActiveTransform
    {
        NONE,
        TRANSLATE,
        ROTATE,
        SCALE
    };
    ActiveTransform mActiveTransform = ActiveTransform::NONE;

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

    void refreshSettings() override { Base::viewerUpdate(); }

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

        auto   dl         = this->drawList();
        ushort selectedId = dl->selectedObjectId();
        if (selectedId == USHORT_NULL || selectedId >= dl->size())
            return;

        auto mesh = findMesh(selectedId);
        if (!mesh || !mesh->isVisible())
            return;

        vcl::Matrix44f model =
            mesh->meshProvider().transformMatrix().template cast<float>();

        vcl::Box3d     bbox     = mesh->meshProvider().boundingBox();
        vcl::Matrix44f scaleMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixScale(scaleMat, bbox.size().cast<float>());
        vcl::Matrix44f transMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixTranslation(
            transMat, bbox.center().cast<float>());

        vcl::Matrix44f gizmoTransform = model * transMat * scaleMat;

        if (mSettings.enableTranslate) {
            mTranslateGizmo.draw(viewId, gizmoTransform);
        }
        if (mSettings.enableScale) {
            mScaleGizmo.draw(viewId, gizmoTransform);
        }
        if (mSettings.enableRotate) {
            vcl::Matrix44f rotScaleMat = vcl::Matrix44f::Identity();
            float r = bbox.diagonal() / 2.0f;
            vcl::setTransformMatrixScale(rotScaleMat, vcl::Point3f(r, r, r));
            vcl::Matrix44f rotGizmoTransform = model * transMat * rotScaleMat;
            mRotateGizmo.draw(viewId, rotGizmoTransform);
        }
    }

    void drawId(uint viewId) override
    {
        if (!Base::isActive())
            return;

        auto   dl         = this->drawList();
        ushort selectedId = dl->selectedObjectId();
        if (selectedId == USHORT_NULL || selectedId >= dl->size())
            return;

        auto mesh = findMesh(selectedId);
        if (!mesh || !mesh->isVisible())
            return;

        vcl::Matrix44f model =
            mesh->meshProvider().transformMatrix().template cast<float>();

        vcl::Box3d     bbox     = mesh->meshProvider().boundingBox();
        vcl::Matrix44f scaleMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixScale(scaleMat, bbox.size().cast<float>());
        vcl::Matrix44f transMat = vcl::Matrix44f::Identity();
        vcl::setTransformMatrixTranslation(
            transMat, bbox.center().cast<float>());

        vcl::Matrix44f gizmoTransform = model * transMat * scaleMat;

        if (mSettings.enableTranslate) {
            mTranslateGizmo.drawId(viewId, gizmoTransform);
        }
        if (mSettings.enableScale) {
            mScaleGizmo.drawId(viewId, gizmoTransform);
        }
        if (mSettings.enableRotate) {
            vcl::Matrix44f rotScaleMat = vcl::Matrix44f::Identity();
            float r = bbox.diagonal() / 2.0f;
            vcl::setTransformMatrixScale(rotScaleMat, vcl::Point3f(r, r, r));
            vcl::Matrix44f rotGizmoTransform = model * transMat * rotScaleMat;
            mRotateGizmo.drawId(viewId, rotGizmoTransform);
        }
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

        if (!mSettings.enableTranslate &&
            !mSettings.enableScale &&
            !mSettings.enableRotate) {
            return false;
        }

        mTransformInProgress = true;
        mStartMousePos       = Point2d(x, y);
        mCurrentObjId        = USHORT_NULL;

        this->viewerReadElementIdRequest(
            x,
            y,
            [this, x, y](ushort objId, ushort elemId, uint primitiveId) {
                if (!mTransformInProgress)
                    return; // aborted
                if (objId == USHORT_NULL) {
                    mTransformInProgress = false; // clicked on background
                    return;
                }

                auto   dl         = this->drawList();
                ushort selectedId = dl->selectedObjectId();

                if (objId == 0xFFFE) { // Scale Gizmo
                    if (selectedId != USHORT_NULL && selectedId < dl->size()) {
                        mCurrentObjId = selectedId;
                        auto mesh     = findMesh(mCurrentObjId);
                        if (mesh) {
                            if (mSettings.enableScale) {
                                mActiveTransform = ActiveTransform::SCALE;
                                mScaleGizmo.calculateAnchor(
                                    elemId, primitiveId, mesh);
                                mAnchorDepth =
                                    project(
                                        mScaleGizmo.anchorPointInWorld(mesh))
                                        .z();
                                savePreTransformStates(mCurrentObjId);
                            } else {
                                mTransformInProgress = false;
                                return;
                            }
                        }
                        else {
                            mTransformInProgress = false;
                            return;
                        }
                    }
                    else {
                        mTransformInProgress = false;
                        return;
                    }
                }
                else if (objId == 0xFFFD) { // Rotate Gizmo
                    if (selectedId != USHORT_NULL && selectedId < dl->size()) {
                        mCurrentObjId = selectedId;
                        auto mesh     = findMesh(mCurrentObjId);
                        if (mesh) {
                            if (mSettings.enableRotate) {
                                mActiveTransform = ActiveTransform::ROTATE;
                                mRotateGizmo.calculateAnchor(
                                    elemId, primitiveId, mesh);
                                mAnchorDepth =
                                    project(
                                        mRotateGizmo.anchorPointInWorld(mesh))
                                        .z();
                                savePreTransformStates(mCurrentObjId);
                            } else {
                                mTransformInProgress = false;
                                return;
                            }
                        }
                        else {
                            mTransformInProgress = false;
                            return;
                        }
                    }
                    else {
                        mTransformInProgress = false;
                        return;
                    }
                }
                else { // Clicked on mesh or background
                    auto mesh = findMesh(objId);
                    if (!mesh) {
                        mTransformInProgress = false;
                        return;
                    }

                    if (mSettings.editMode ==
                        EditorSettings::EditMode::CURRENT_OBJECT) {
                        if (objId != selectedId) {
                            mTransformInProgress = false;
                            return;
                        }
                    }

                    // Translate mode logic
                    mCurrentObjId = objId;
                    if (mSettings.enableTranslate) {
                        mActiveTransform = ActiveTransform::TRANSLATE;
                        mTranslateGizmo.calculateAnchor(mesh);
                        mAnchorDepth =
                            project(mTranslateGizmo.anchorPointInWorld(mesh)).z();
                        savePreTransformStates(mCurrentObjId);
                    } else {
                        mTransformInProgress = false;
                        return;
                    }
                }
            },
            3);

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

        if (mActiveTransform == ActiveTransform::TRANSLATE) {
            for (auto& state : mPreTransformStates) {
                if (auto lock = state.obj.lock()) {
                    if (auto* m =
                            dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                        Matrix44d newTrans =
                            mTranslateGizmo.calculateNewTransform(
                                newPoint3D, oldPoint3D, state.transformMatrix);
                        m->meshProvider().setTransformMatrix(newTrans);
                        m->notifyMeshUpdated();
                    }
                }
            }
        }
        else if (mActiveTransform == ActiveTransform::SCALE) {
            for (auto& state : mPreTransformStates) {
                if (auto lock = state.obj.lock()) {
                    if (auto* m =
                            dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                        Matrix44d newTrans = mScaleGizmo.calculateNewTransform(
                            newPoint3D, oldPoint3D, state.transformMatrix);
                        m->meshProvider().setTransformMatrix(newTrans);
                        m->notifyMeshUpdated();
                    }
                }
            }
        }
        else if (mActiveTransform == ActiveTransform::ROTATE) {
            for (auto& state : mPreTransformStates) {
                if (auto lock = state.obj.lock()) {
                    if (auto* m =
                            dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                        Matrix44d newTrans = mRotateGizmo.calculateNewTransform(
                            newPoint3D, oldPoint3D, state.transformMatrix);
                        m->meshProvider().setTransformMatrix(newTrans);
                        m->notifyMeshUpdated();
                    }
                }
            }
        }

        Base::viewerUpdate();

        return true;
    }

    bool onMouseRelease(
        vcl::MouseButton::Enum button,
        double,
        double,
        const vcl::KeyModifiers& modifiers) override
    {
        if (!mTransformInProgress || button != vcl::MouseButton::LEFT)
            return false;

        if (mCurrentObjId == USHORT_NULL) {
            mTransformInProgress = false;
            return true;
        }

        mTransformInProgress = false;
        mActiveTransform = ActiveTransform::NONE;

        auto dl = this->drawList();
        finalizeTransformAction();

        return true;
    }

private:
    std::shared_ptr<AbstractDrawableMesh> findMesh(ushort objId) const
    {
        auto dl = this->drawList();
        if (!dl || objId >= dl->size())
            return nullptr;

        return std::dynamic_pointer_cast<AbstractDrawableMesh>(dl->at(objId));
    }

    void savePreTransformStates(ushort activeObjId)
    {
        mPreTransformStates.clear();

        auto dl = this->drawList();
        if (!dl)
            return;

        if (mSettings.editMode == EditorSettings::EditMode::CURRENT_OBJECT ||
            mSettings.editMode == EditorSettings::EditMode::VISIBLE_OBJECTS) {
            auto mesh = findMesh(activeObjId);
            if (mesh) {
                mPreTransformStates.push_back(
                    {mesh, mesh->meshProvider().transformMatrix()});
            }
        }
        else if (mSettings.editMode == EditorSettings::EditMode::ALL_OBJECTS) {
            for (uint i = 0; i < dl->size(); ++i) {
                if (auto mesh = findMesh(i)) {
                    mPreTransformStates.push_back(
                        {mesh, mesh->meshProvider().transformMatrix()});
                }
            }
        }
    }

    void finalizeTransformAction()
    {
        if (mPreTransformStates.empty())
            return;

        bool changed = false;
        for (const auto& state : mPreTransformStates) {
            if (auto lock = state.obj.lock()) {
                if (auto* m = dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                    if (m->meshProvider().transformMatrix() !=
                        state.transformMatrix) {
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

    vcl::Point3d unproject(double x, double y, double depth) const
    {
        Matrix44d view = this->viewerViewMatrix().template cast<double>();
        Matrix44d proj = this->viewerProjectionMatrix().template cast<double>();
        Matrix44d pv   = proj * view;
        auto      size = this->viewerCanvasSize();

        Point4d viewport(0.0, 0.0, size.x(), size.y());
        Point3d screenPos(x, size.y() - y, depth);

        return unprojectScreenPosition(screenPos, pv, viewport, false);
    }

    vcl::Point3d project(const vcl::Point3d& p) const
    {
        Matrix44d view = this->viewerViewMatrix().template cast<double>();
        Matrix44d proj = this->viewerProjectionMatrix().template cast<double>();
        Matrix44d pv   = proj * view;
        auto      size = this->viewerCanvasSize();

        Point4d viewport(0.0, 0.0, size.x(), size.y());
        return projectScreenPosition(p, pv, viewport, false);
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
