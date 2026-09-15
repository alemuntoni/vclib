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

    QHBoxLayout* modeLayout = new QHBoxLayout();
    QLabel* modeLabel = new QLabel("Mode:", this);
    mModeComboBox = new QComboBox(this);
    mModeComboBox->addItem("Translate");
    mModeComboBox->addItem("Rotate");
    mModeComboBox->addItem("Scale");
    modeLayout->addWidget(modeLabel);
    modeLayout->addWidget(mModeComboBox);

    QPushButton* resetBtn = new QPushButton("Reset Default", this);

    mainLayout->addLayout(modeLayout);
    mainLayout->addWidget(resetBtn);
    mainLayout->addStretch();

    updateGUI();

    connect(mModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransformEditorSettingsFrame::onModeChanged);
    connect(resetBtn, &QPushButton::clicked,
            this, &TransformEditorSettingsFrame::onResetDefaultClicked);
}

void TransformEditorSettingsFrame::updateGUI()
{
    bool b1 = mModeComboBox->blockSignals(true);
    mModeComboBox->setCurrentIndex(static_cast<int>(mSettings.mode));
    mModeComboBox->blockSignals(b1);
}

void TransformEditorSettingsFrame::onModeChanged(int index)
{
    mSettings.mode = static_cast<TransformEditorSettings::Mode>(index);
    emit settingsUpdated();
}

void TransformEditorSettingsFrame::onResetDefaultClicked()
{
    mSettings.resetDefaults();
    updateGUI();
    emit settingsUpdated();
}

} // namespace vcl::qt
