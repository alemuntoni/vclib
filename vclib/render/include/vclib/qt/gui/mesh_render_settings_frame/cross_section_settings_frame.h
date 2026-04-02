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

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_CROSS_SECTION_SETTINGS_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_CROSS_SECTION_SETTINGS_FRAME_H

#include <vclib/render/drawable/mesh/cross_section_settings.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class CrossSectionSettingsFrame;
} // namespace Ui

class CrossSectionSettingsFrame : public QFrame
{
    Q_OBJECT

    Ui::CrossSectionSettingsFrame* mUI;

    CrossSectionSettings mCSS;

public:
    explicit CrossSectionSettingsFrame(
        const CrossSectionSettings css,
        QWidget*                   parent = nullptr);

    explicit CrossSectionSettingsFrame(QWidget* parent = nullptr);

    ~CrossSectionSettingsFrame();

     const CrossSectionSettings& crossSectionSettings() const;

     void setCrossSectionSettings(const CrossSectionSettings& settings);

signals:
    void crossSectionSettingsUpdated();

private:
    void updateFrameFromSettings();

private slots:
    void onCrossSectionEnabledChanged(Qt::CheckState arg1);
    void onPerVertexToggled(bool checked);
    void onPerFragmentToggled(bool checked);
    void onXFloatRangeSliderLowerValueChanged(float value);
    void onXFloatRangeSliderUpperValueChanged(float value);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_CROSS_SECTION_SETTINGS_FRAME_H
