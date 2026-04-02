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
}

} // namespace vcl::qt
