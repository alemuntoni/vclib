// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_TRANSFORM_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_TRANSFORM_EDITOR_SETTINGS_H

#include <vclib/render/settings/editor_settings.h>
#include <vclib/space/core.h>
#include <nlohmann/json.hpp>

namespace vcl {

struct TransformEditorSettings : public EditorSettings
{
    bool enableTranslate = false;
    bool enableRotate    = false;
    bool enableScale     = false;

    /**
     * @brief Checks if any of the transform operations (translate, rotate,
     * scale) is enabled.
     * @return true if at least one operation is enabled, false otherwise.
     */
    bool isAnyTransformEnabled() const
    {
        return enableTranslate || enableRotate || enableScale;
    }

    /**
     * @brief Resets the settings to their default values.
     */
    void resetDefaults()
    {
    }

    /**
     * @brief Loads the settings from a JSON object.
     * @param[in] j: the JSON object to read from.
     */
    void loadSettings(const nlohmann::json& j)
    {
    }

    /**
     * @brief Saves the settings to a JSON object.
     * @param[out] j: the JSON object to write to.
     */
    void saveSettings(nlohmann::json& j) const
    {
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_TRANSFORM_EDITOR_SETTINGS_H
