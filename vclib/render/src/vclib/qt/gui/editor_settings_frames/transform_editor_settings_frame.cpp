// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/editor_settings_frames/transform_editor_settings_frame.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace vcl::qt {

TransformEditorSettingsFrame::TransformEditorSettingsFrame(
    TransformEditorSettings& sts,
    QWidget*                 parent) : QFrame(parent), mSettings(sts)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mEditModeFrame = new EditModeSettingsFrame(this);
    mEditModeFrame->disableEditMode(EditorSettings::EditMode::NONE);
    mEditModeFrame->disableEditMode(EditorSettings::EditMode::ALL_OBJECTS);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* resetBtn  = new QPushButton("Reset Default", this);
    btnLayout->addStretch();
    btnLayout->addWidget(resetBtn);

    mainLayout->addWidget(mEditModeFrame);
    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch();

    updateGUI();

    connect(
        mEditModeFrame,
        &EditModeSettingsFrame::editModeChanged,
        this,
        &TransformEditorSettingsFrame::onEditModeChanged);
    connect(
        resetBtn,
        &QPushButton::clicked,
        this,
        &TransformEditorSettingsFrame::onResetDefaultClicked);
}

void TransformEditorSettingsFrame::updateGUI()
{
    bool b2 = mEditModeFrame->blockSignals(true);
    mEditModeFrame->setEditMode(mSettings.editMode);
    mEditModeFrame->blockSignals(b2);
}

void TransformEditorSettingsFrame::onEditModeChanged(int index)
{
    using enum EditorSettings::EditMode;
    mSettings.editMode = static_cast<EditorSettings::EditMode>(index);
    emit settingsUpdated();
}

void TransformEditorSettingsFrame::onResetDefaultClicked()
{
    mSettings.resetDefaults();
    updateGUI();
    emit settingsUpdated();
}

} // namespace vcl::qt
