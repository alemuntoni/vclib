// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_GENERIC_SETTINGS_WIDGET_H
#define VCL_QT_GUI_GENERIC_SETTINGS_WIDGET_H

#include <vclib/qt/gui/settings_dialog/input_binding_map_widget.h>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QWidget>

namespace vcl::qt {

template<typename SettingsType>
class GenericSettingsWidget : public QWidget
{
public:
    GenericSettingsWidget(SettingsType& settings, QWidget* parent = nullptr) :
            QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        auto visitor =
            [this, layout](
                const std::string& name, auto& map, const auto&... callbacks) {
                QGroupBox* groupBox =
                    new QGroupBox(QString::fromStdString(name), this);
                QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);

                using MapType = std::remove_reference_t<decltype(map)>;

                InputBindingMapWidget<MapType>* mapWidget = nullptr;
                if constexpr (sizeof...(callbacks) == 0) {
                    mapWidget = new InputBindingMapWidget<MapType>(
                        map,
                        std::vector<typename MapType::ActionType> {},
                        groupBox);
                }
                else {
                    mapWidget = new InputBindingMapWidget<MapType>(
                        map, callbacks..., groupBox);
                }
                groupLayout->addWidget(mapWidget);

                layout->addWidget(groupBox);
            };

        if constexpr (requires { settings.visitInputBindings(visitor); }) {
            settings.visitInputBindings(visitor);
        }
        else if constexpr (requires {
                               settings.visitInputBindings(
                                   visitor, std::vector<std::string> {});
                           }) {
            settings.visitInputBindings(visitor, std::vector<std::string> {});
        }

        layout->addStretch();
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_GENERIC_SETTINGS_WIDGET_H
