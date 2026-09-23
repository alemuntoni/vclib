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

        auto& settings = mEditor->settings();

        QIcon ic(":/icons/bbox.png"); // Using bbox icon as placeholder

        QPushButton* translateBtn = Base::addButton(ic);
        translateBtn->setToolTip("Translate Object");

        QPushButton* rotateBtn = Base::addButton(ic);
        rotateBtn->setToolTip("Rotate Object");

        QPushButton* scaleBtn = Base::addButton(ic);
        scaleBtn->setToolTip("Scale Object");

        auto onTranslateClicked = [&](bool checked) {
            settings.enableTranslate = checked;
            mEditor->setActive(settings.isAnyTransformEnabled());
        };

        auto onRotateClicked = [&](bool checked) {
            settings.enableRotate = checked;
            mEditor->setActive(settings.isAnyTransformEnabled());
        };

        auto onScaleClicked = [&](bool checked) {
            settings.enableScale = checked;
            mEditor->setActive(settings.isAnyTransformEnabled());
        };

        connect(translateBtn, &QPushButton::clicked, this, onTranslateClicked);
        connect(rotateBtn, &QPushButton::clicked, this, onRotateClicked);
        connect(scaleBtn, &QPushButton::clicked, this, onScaleClicked);

        TransformEditorSettingsFrame* sf =
            Base::setSettingsFrame<TransformEditorSettingsFrame>(settings);

        connect(sf, SIGNAL(settingsUpdated()), this, SLOT(refreshSettings()));

        mEditor->setOnStateUpdatedCallback(
            [this, translateBtn, rotateBtn, scaleBtn]() {
                translateBtn->setChecked(mEditor->settings().enableTranslate);
                rotateBtn->setChecked(mEditor->settings().enableRotate);
                scaleBtn->setChecked(mEditor->settings().enableScale);
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
