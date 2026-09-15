// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_GUI_TOOLBAR_FRAMES_TRANSFORM_EDITOR_FRAME_H
#define VCL_IMGUI_GUI_TOOLBAR_FRAMES_TRANSFORM_EDITOR_FRAME_H

#include "../editor_frame.h"

#include <vclib/render/editors/transform_editor.h>

#include <imgui/imgui.h>

#include <memory>

namespace vcl::imgui {

/**
 * @brief Frame that allows the user to interact with a TransformEditor.
 *
 * @ingroup imgui_toolbar_frames
 */
template<typename ViewerType>
class TransformEditorFrameImgui : public EditorFrameImgui
{
    std::shared_ptr<vcl::TransformEditor<ViewerType>> mEditor;

public:
    explicit TransformEditorFrameImgui(
        std::shared_ptr<vcl::TransformEditor<ViewerType>> editor) :
            mEditor(editor)
    {
    }

    void draw() override
    {
        if (!mEditor)
            return;

        bool active = mEditor->isActive();

        if (ImGui::Button(active ? "[TRS]" : " TRS ")) {
            active = !active;
            mEditor->setActive(active);
            mEditor->refreshSettings();
        }
        
        ImGui::SameLine(0, 2);
        if (ImGui::Button("v##TransformSettings")) {
            ImGui::OpenPopup("##TransformSettingsPopup");
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Transform Settings");

        if (ImGui::BeginPopup("##TransformSettingsPopup")) {
            drawTransformSettings();
            ImGui::EndPopup();
        }
    }
    
private:
    void drawTransformSettings()
    {
        vcl::TransformEditorSettings& sts = mEditor->settings();

        static const char* modeNames[] = {
            "Translate", "Rotate", "Scale"};
        int currentMode = static_cast<int>(sts.mode);
        ImGui::Text("Mode:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("##TransformMode", modeNames[currentMode])) {
            for (int n = 0; n < IM_ARRAYSIZE(modeNames); n++) {
                bool selected = (n == currentMode);
                if (ImGui::Selectable(modeNames[n], selected)) {
                    sts.mode = static_cast<vcl::TransformEditorSettings::Mode>(n);
                    mEditor->refreshSettings();
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    

};

template<typename ViewerType>
struct EditorFrameTraits<vcl::TransformEditor, ViewerType>
{
    using ToolbarFrameType = TransformEditorFrameImgui<ViewerType>;
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_GUI_TOOLBAR_FRAMES_TRANSFORM_EDITOR_FRAME_H
