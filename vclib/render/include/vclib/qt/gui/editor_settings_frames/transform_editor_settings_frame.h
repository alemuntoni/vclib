// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_TRANSFORM_EDITOR_SETTINGS_FRAME_H
#define VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_TRANSFORM_EDITOR_SETTINGS_FRAME_H

#include <vclib/render/editors/transform_editor.h>

#include <vclib/qt/gui/editor_settings_frames/edit_mode_settings_frame.h>

#include <QFrame>
#include <QComboBox>

namespace vcl::qt {

class TransformEditorSettingsFrame : public QFrame
{
    Q_OBJECT

    TransformEditorSettings& mSettings;
    QComboBox* mModeComboBox;
    EditModeSettingsFrame* mEditModeFrame;

public:
    explicit TransformEditorSettingsFrame(
        TransformEditorSettings& sts,
        QWidget*                 parent = nullptr);
    ~TransformEditorSettingsFrame() = default;

    void updateGUI();

signals:
    void settingsUpdated();

private slots:
    void onModeChanged(int index);
    void onEditModeChanged(int index);
    void onResetDefaultClicked();
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_EDITOR_SETTINGS_FRAMES_TRANSFORM_EDITOR_SETTINGS_FRAME_H
