// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_INPUT_BINDING_MAP_WIDGET_H
#define VCL_QT_GUI_INPUT_BINDING_MAP_WIDGET_H

#include <vclib/render/concepts/settings.h>
#include <vclib/render/input/input.h>

#include <QComboBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace vcl::qt {

template<typename MapType>
class InputBindingMapWidget : public QWidget
{
public:
    using InputType  = typename MapType::InputType;
    using ActionType = typename MapType::ActionType;

    InputBindingMapWidget(
        MapType&                       map,
        const std::vector<ActionType>& availableCallbacks = {},
        QWidget* parent = nullptr) : QWidget(parent), mMap(map)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        mTable = new QTableWidget(this);
        mTable->setColumnCount(2);
        mTable->setHorizontalHeaderLabels({"Input", "Action"});
        mTable->horizontalHeader()->setStretchLastSection(true);
        mTable->verticalHeader()->setVisible(false);
        mTable->setSelectionMode(QAbstractItemView::NoSelection);
        layout->addWidget(mTable);

        // Fetch available actions
        std::vector<ActionType> actions = availableCallbacks;
        if constexpr (ActionConcept<ActionType>) {
            if (actions.empty()) {
                actions = vcl::availableActions<ActionType>();
            }
        }

        // Populate table from map
        mTable->setRowCount(map.map().size());
        int row = 0;
        for (const auto& [input, action] : map.map()) {
            // Input string
            std::string inputStr;
            if constexpr (Stringifiable<InputType>) {
                inputStr = vcl::toString(input);
            }
            else {
                inputStr = "Unknown Input";
            }

            QTableWidgetItem* inputItem =
                new QTableWidgetItem(QString::fromStdString(inputStr));
            inputItem->setFlags(inputItem->flags() & ~Qt::ItemIsEditable);
            mTable->setItem(row, 0, inputItem);

            QComboBox* combo = new QComboBox(this);
            for (size_t i = 0; i < actions.size(); ++i) {
                const auto& a = actions[i];
                std::string actionStr;
                if constexpr (Stringifiable<ActionType>) {
                    actionStr = vcl::toString(a);
                }
                else {
                    actionStr = "Unknown";
                }
                combo->addItem(QString::fromStdString(actionStr));

                // Store the index to retrieve it later, but QComboBox index
                // matches actions[i]
            }

            // Set current index
            std::string currActionStr;
            if constexpr (Stringifiable<ActionType>) {
                currActionStr = vcl::toString(action);
            }
            combo->setCurrentText(QString::fromStdString(currActionStr));

            connect(
                combo,
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                this,
                [this, input, actions, combo](int index) {
                    if (index >= 0 && index < actions.size()) {
                        mMap.setBinding(actions[index], input);
                    }
                });

            mTable->setCellWidget(row, 1, combo);
            row++;
        }
    }

private:
    MapType&      mMap;
    QTableWidget* mTable;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_INPUT_BINDING_MAP_WIDGET_H
