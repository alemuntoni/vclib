// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/editor_settings_frames/transform_editor_settings_frame.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace vcl::qt {

TransformEditorSettingsFrame::TransformEditorSettingsFrame(
    TransformEditorSettings& sts,
    QWidget*                 parent) :
        QFrame(parent), mSettings(sts)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mEditModeFrame = new EditModeSettingsFrame(this);
    mEditModeFrame->disableEditMode(EditorSettings::EditMode::NONE);
    mEditModeFrame->disableEditMode(EditorSettings::EditMode::ALL_OBJECTS);

    QHBoxLayout* modeLayout = new QHBoxLayout();
    QLabel* modeLabel = new QLabel("Mode:", this);
    mModeComboBox = new QComboBox(this);
    mModeComboBox->addItem("Translate");
    mModeComboBox->addItem("Rotate");
    mModeComboBox->addItem("Scale");
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(mModeComboBox);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* resetBtn = new QPushButton("Reset Default", this);
    btnLayout->addStretch();
    btnLayout->addWidget(resetBtn);

    mainLayout->addWidget(mEditModeFrame);
    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch();

    updateGUI();

    connect(mModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransformEditorSettingsFrame::onModeChanged);
    connect(mEditModeFrame, &EditModeSettingsFrame::editModeChanged,
            this, &TransformEditorSettingsFrame::onEditModeChanged);
    connect(resetBtn, &QPushButton::clicked,
            this, &TransformEditorSettingsFrame::onResetDefaultClicked);
}

void TransformEditorSettingsFrame::updateGUI()
{
    bool b1 = mModeComboBox->blockSignals(true);
    bool b2 = mEditModeFrame->blockSignals(true);
    mModeComboBox->setCurrentIndex(static_cast<int>(mSettings.mode));
    mEditModeFrame->setEditMode(mSettings.editMode);
    mModeComboBox->blockSignals(b1);
    mEditModeFrame->blockSignals(b2);
}

void TransformEditorSettingsFrame::onModeChanged(int index)
{
    mSettings.mode = static_cast<TransformEditorSettings::Mode>(index);
    emit settingsUpdated();
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
