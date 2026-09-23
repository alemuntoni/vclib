// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_TRANSFORM_EDITOR_H
#define VCL_RENDER_EDITORS_TRANSFORM_EDITOR_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/editors/transform_editor_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include "editor.h"

#include <vclib/render/settings/transform_editor_settings.h>
#endif

namespace vcl {

/**
 * @brief The TransformEditor class provides a viewer editor for interactively
 * modifying the transformation matrix of a mesh.
 *
 * It provides three modes: Translate, Rotate, and Scale.
 *
 * @ingroup render_editors
 */
#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename ViewerDrawer>
using TransformEditor = TransformEditorBGFX<ViewerDrawer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// TODO: implement TransformEditorOpenGL2
template<typename ViewerDrawer>
class TransformEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;
    TransformEditorSettings mSettings;

public:
    TransformEditor() = default;

    std::string name() const override { return "Transform"; }

    TransformEditorSettings& settings() override { return mSettings; }

    const TransformEditorSettings& settings() const override { return mSettings; }

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
    }

    void draw(uint) override {}
    void drawId(uint) override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_TRANSFORM_EDITOR_H
