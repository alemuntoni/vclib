// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_UNDO_REDO_TRANSFORM_UNDO_REDO_ACTION_H
#define VCL_RENDER_UNDO_REDO_TRANSFORM_UNDO_REDO_ACTION_H

#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/space/core.h>

#include <memory>
#include <vector>

namespace vcl {

/**
 * @brief An action that encapsulates a change in the transformation matrix of a
 * mesh.
 */
class TransformUndoRedoAction : public UndoRedoAction
{
public:
    struct MeshState
    {
        std::weak_ptr<vcl::DrawableObject> obj;
        vcl::Matrix44d                     transformMatrix;
    };

private:
    std::vector<MeshState> mStates;

public:
    TransformUndoRedoAction(std::vector<MeshState> states) :
            mStates(std::move(states))
    {
    }

    void undo() override { swapState(); }

    void redo() override { swapState(); }

    std::string name() const override { return "Transform Matrix Changed"; }

private:
    void swapState()
    {
        for (auto& state : mStates) {
            if (auto lock = state.obj.lock()) {
                if (auto* mesh =
                        dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                    auto current = mesh->meshProvider().transformMatrix();
                    mesh->meshProvider().setTransformMatrix(
                        state.transformMatrix);
                    state.transformMatrix = current;
                    mesh->notifyMeshUpdated();
                }
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_UNDO_REDO_TRANSFORM_UNDO_REDO_ACTION_H
