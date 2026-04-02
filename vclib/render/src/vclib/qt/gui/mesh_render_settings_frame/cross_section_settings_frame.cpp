/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/qt/gui/mesh_render_settings_frame/cross_section_settings_frame.h>

#include "ui_cross_section_settings_frame.h"

namespace vcl::qt {

CrossSectionSettingsFrame::CrossSectionSettingsFrame(
    const CrossSectionSettings css,
    QWidget*                   parent) :
        QFrame(parent), mUI(new Ui::CrossSectionSettingsFrame), mCSS(css)
{
    mUI->setupUi(this);
    updateFrameFromSettings();

    connect(
        mUI->crosSectionEnabledCheckBox,
        SIGNAL(checkStateChanged(Qt::CheckState)),
        this,
        SLOT(onCrossSectionEnabledChanged(Qt::CheckState)));

    connect(
        mUI->perVertexRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onPerVertexToggled(bool)));

    connect(
        mUI->perFragmentRadioButton,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onPerFragmentToggled(bool)));

    connect(
        mUI->xFloatRangeSlider,
        SIGNAL(lowerValueChanged(float)),
        this,
        SLOT(onXFloatRangeSliderLowerValueChanged(float)));

    connect(
        mUI->xFloatRangeSlider,
        SIGNAL(upperValueChanged(float)),
        this,
        SLOT(onXFloatRangeSliderUpperValueChanged(float)));
}

CrossSectionSettingsFrame::CrossSectionSettingsFrame(QWidget* parent) :
        CrossSectionSettingsFrame(CrossSectionSettings(), parent)
{
}

CrossSectionSettingsFrame::~CrossSectionSettingsFrame()
{
    delete mUI;
}

const CrossSectionSettings& CrossSectionSettingsFrame::crossSectionSettings()
    const
{
    return mCSS;
}

void CrossSectionSettingsFrame::setCrossSectionSettings(
    const CrossSectionSettings& settings)
{
    mCSS = settings;
    updateFrameFromSettings();
}

void CrossSectionSettingsFrame::updateFrameFromSettings()
{
    using enum CrossSectionSettings::CrossSectionType;

    // radio buttons
    if (mCSS.type() == PER_VERTEX)
        mUI->perVertexRadioButton->setChecked(true);
    else if (mCSS.type() == PER_FRAGMENT)
        mUI->perFragmentRadioButton->setChecked(true);

    mUI->xFloatRangeSlider->blockSignals(true);
    auto bb = mCSS.boundingBox();
    mUI->xFloatRangeSlider->setRange(bb.min().x(), bb.max().x());

    auto l = mCSS.lower();
    auto u = mCSS.upper();

    mUI->xFloatRangeSlider->setLowerValue(l.x());
    mUI->xFloatRangeSlider->setUpperValue(u.x());
    mUI->xFloatRangeSlider->blockSignals(false);

    // checkbox, last thing to do
    if (mCSS.type() == NONE)
        mUI->crosSectionEnabledCheckBox->setCheckState(
            Qt::CheckState::Unchecked);
    else
        mUI->crosSectionEnabledCheckBox->setCheckState(Qt::CheckState::Checked);
}

void CrossSectionSettingsFrame::onCrossSectionEnabledChanged(
    Qt::CheckState arg1)
{
    using enum CrossSectionSettings::CrossSectionType;

    if (arg1 != Qt::CheckState::Checked) {
        mCSS.type() = NONE;
    }
    else {
        if (mUI->perVertexRadioButton->isChecked())
            mCSS.type() = PER_VERTEX;
        else
            mCSS.type() = PER_FRAGMENT;
    }

    mUI->controlsFrame->setEnabled(arg1 == Qt::CheckState::Checked);
    emit crossSectionSettingsUpdated();
}

void CrossSectionSettingsFrame::onPerVertexToggled(bool checked)
{
    using enum CrossSectionSettings::CrossSectionType;
    if (checked) {
        mCSS.type() = PER_VERTEX;
        emit crossSectionSettingsUpdated();
    }
}

void CrossSectionSettingsFrame::onPerFragmentToggled(bool checked)
{
    using enum CrossSectionSettings::CrossSectionType;
    if (checked) {
        mCSS.type() = PER_FRAGMENT;
        emit crossSectionSettingsUpdated();
    }
}

void CrossSectionSettingsFrame::onXFloatRangeSliderLowerValueChanged(
    float value)
{
    auto l = mCSS.lower();
    l.x() = value;
    mCSS.setLower(l);
    emit crossSectionSettingsUpdated();
}

void CrossSectionSettingsFrame::onXFloatRangeSliderUpperValueChanged(
    float value)
{
    auto u = mCSS.upper();
    u.x() = value;
    mCSS.setUpper(u);
    emit crossSectionSettingsUpdated();
}

} // namespace vcl::qt
