// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H

#include <vclib/render/editors/editor.h>
#include <vclib/render/settings/transform_editor_settings.h>

namespace vcl {

/**
 * @brief The TransformEditorBGFX class provides a viewer editor for interactively
 * modifying the transformation matrix of a mesh.
 *
 * @ingroup bgfx_editors
 */
template<typename ViewerDrawer>
class TransformEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;
    TransformEditorSettings mSettings;

public:
    TransformEditorBGFX() = default;

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

    void draw(uint viewId) override
    {
        if (!Base::isActive()) return;
        // Gizmos drawing will go here
    }

    void drawId(uint viewId) override
    {
        if (!Base::isActive()) return;
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

        // Translation logic will go here
        return false;
    }

    bool onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        if (!Base::isActive()) return false;
        return false;
    }

    bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        if (!Base::isActive() || button != vcl::MouseButton::LEFT) {
            return false;
        }
        return false;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_TRANSFORM_EDITOR_BGFX_H
