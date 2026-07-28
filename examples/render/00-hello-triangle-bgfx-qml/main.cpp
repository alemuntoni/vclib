// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../00-hello-triangle-bgfx-common/hello_triangle_drawer.h"
#include "qml_item_manager.h"

#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>
#include <vclib/qt/utils.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);
    
    // Register our custom QML item
    using MyQmlApp =
        vcl::RenderApp<QmlItemManager, vcl::Canvas, HelloTriangleDrawer>;

    qmlRegisterType<MyQmlApp>("Vclib", 1, 0, "VclBgfxItem");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        
    engine.load(url);

    return app.exec();
}
