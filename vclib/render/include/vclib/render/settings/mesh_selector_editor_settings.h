// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_MESH_SELECTOR_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_MESH_SELECTOR_EDITOR_SETTINGS_H

#include <vclib/render/input/input_action_binding_map.h>
#include <vclib/render/input/input.h>
#include <vclib/render/settings/editor_settings.h>

namespace vcl {

enum class MeshSelectorAction { SELECT_MESH };

inline std::string toString(MeshSelectorAction action)
{
    switch (action) {
    case MeshSelectorAction::SELECT_MESH: return "Select Mesh";
    default: return "Unknown Action";
    }
}

inline void fromString(const std::string& str, MeshSelectorAction& out)
{
    if (str == "Select Mesh" || str == "SELECT_MESH")
        out = MeshSelectorAction::SELECT_MESH;
    else {
        throw std::invalid_argument(
            "MeshSelectorAction fromString failed to parse: '" + str + "'");
    }
}

template<>
inline std::vector<MeshSelectorAction> availableActions()
{
    return {MeshSelectorAction::SELECT_MESH};
}

struct MeshSelectorEditorSettings : public EditorSettings
{
    using MouseMap = InputActionBindingMap<
        std::pair<MouseButton::Enum, KeyModifiers>,
        MeshSelectorAction>;

    MouseMap mouseBindings = {
        {{MouseButton::RIGHT, {KeyModifier::NO_MODIFIER}},
         MeshSelectorAction::SELECT_MESH}
    };

    template <typename Visitor>
    void visitInputBindings(Visitor&& visitor)
    {
        visitor("Mouse Actions", mouseBindings);
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_MESH_SELECTOR_EDITOR_SETTINGS_H
