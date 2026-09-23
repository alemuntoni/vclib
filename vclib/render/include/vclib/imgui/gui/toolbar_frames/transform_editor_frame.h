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

        vcl::TransformEditorSettings& sts = mEditor->settings();

        bool tEna = sts.enableTranslate;
        bool rEna = sts.enableRotate;
        bool sEna = sts.enableScale;

        if (ImGui::Button(tEna ? "[T]" : " T ")) {
            tEna                = !tEna;
            sts.enableTranslate = tEna;
            mEditor->setActive(sts.isAnyTransformEnabled());
            mEditor->refreshSettings();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Translate Object");

        ImGui::SameLine(0, 2);
        if (ImGui::Button(rEna ? "[R]" : " R ")) {
            rEna             = !rEna;
            sts.enableRotate = rEna;
            mEditor->setActive(sts.isAnyTransformEnabled());
            mEditor->refreshSettings();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Rotate Object");

        ImGui::SameLine(0, 2);
        if (ImGui::Button(sEna ? "[S]" : " S ")) {
            sEna            = !sEna;
            sts.enableScale = sEna;
            mEditor->setActive(sts.isAnyTransformEnabled());
            mEditor->refreshSettings();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            ImGui::SetTooltip("Scale Object");

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

        // Edit mode
        static const char* editModeNames[] = {
            "None", "Selected Object", "Visible Objects", "All Objects"};
        int currentEditMode = vcl::toUnderlying(sts.editMode);
        ImGui::Text("Apply to:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(130);
        if (ImGui::BeginCombo(
                "##TransformEditMode", editModeNames[currentEditMode])) {
            for (int n = 0; n < IM_ARRAYSIZE(editModeNames); n++) {
                bool selected = (n == currentEditMode);
                if (n == 0 || n == 3)
                    ImGui::BeginDisabled();
                if (ImGui::Selectable(editModeNames[n], selected)) {
                    sts.editMode =
                        static_cast<vcl::EditorSettings::EditMode>(n);
                    mEditor->refreshSettings();
                }
                if (n == 0 || n == 3)
                    ImGui::EndDisabled();
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        if (ImGui::Button("Reset Default", ImVec2(-1, 0))) {
            sts.resetDefaults();
            mEditor->refreshSettings();
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
