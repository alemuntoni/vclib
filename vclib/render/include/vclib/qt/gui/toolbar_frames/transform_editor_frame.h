// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_TRANSFORM_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_TRANSFORM_EDITOR_FRAME_H

#include "generic_editor_frame.h"

#include <vclib/render/editors/transform_editor.h>

#include <vclib/qt/gui/editor_settings_frames/transform_editor_settings_frame.h>

namespace vcl::qt {

template<typename ViewerType>
class TransformEditorFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    std::shared_ptr<vcl::TransformEditor<ViewerType>> mEditor;

public:
    explicit TransformEditorFrame(
        std::shared_ptr<vcl::TransformEditor<ViewerType>> ptr,
        QWidget* parent = nullptr) : GenericEditorFrame(parent)
    {
        mEditor = ptr;

        QIcon ic(":/icons/bbox.png"); // Using bbox icon as placeholder

        QPushButton* editorButton = Base::addButton(ic);
        editorButton->setToolTip("Transform Object");

        connect(editorButton, &QPushButton::clicked, this, [this]() {
            if (mEditor) {
                mEditor->setActive(!mEditor->isActive());
            }
        });

        TransformEditorSettingsFrame* sf =
            Base::setSettingsFrame<TransformEditorSettingsFrame>(
                mEditor->settings());

        connect(sf, SIGNAL(settingsUpdated()), this, SLOT(refreshSettings()));

        mEditor->setOnStateUpdatedCallback([this, editorButton]() {
            editorButton->setChecked(mEditor->isActive());
        });
    }

private slots:

    void refreshSettings() override
    {
        if (mEditor) {
            mEditor->refreshSettings();
        }
    }
};

template<typename ViewerType>
struct EditorFrameTraits<vcl::TransformEditor, ViewerType>
{
    using ToolbarFrameType  = TransformEditorFrame<ViewerType>;
    using SettingsFrameType = TransformEditorSettingsFrame;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_TRANSFORM_EDITOR_FRAME_H
