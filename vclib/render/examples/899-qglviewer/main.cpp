/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <iostream>

#include <QApplication>

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

#include <vclib/ext/qglviewer/viewer_main_window.h>
#include <vclib/ext/qt/gui/mesh_render_settings_frame.h>
#include <vclib/render/drawable/drawable_mesh.h>

int main(int argc, char** argv)
{
    // Read command lines arguments.
    QApplication application(argc, argv);

    // Instantiate the viewer.
    vcl::qgl::ViewerMainWindow viewer;

    vcl::MeshInfo loadedInfo;

    vcl::PolyMesh tm = vcl::load<vcl::PolyMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply", loadedInfo);
    vcl::updatePerFaceNormals(tm);
    vcl::updatePerVertexNormals(tm);
    tm.enablePerVertexColor();
    tm.enablePerFaceColor();
    vcl::setPerVertexColor(tm, vcl::Color::DarkMagenta);
    vcl::setPerFaceColor(tm, vcl::Color::LightGreen);
    vcl::setMeshColor(tm, vcl::Color::Yellow);

    vcl::TriMesh m = vcl::loadObj<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");
    m.enablePerVertexColor();
    vcl::updatePerFaceNormals(m);
    vcl::updatePerVertexNormals(m);
    vcl::setPerVertexColor(m, vcl::Color::DarkGreen);

    vcl::DrawableMesh<vcl::PolyMesh> dtm(tm);
    vcl::DrawableMesh<vcl::TriMesh>  dm(m);

    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();
    vector->pushBack(dtm);
    vector->pushBack(dm);

    viewer.setDrawableObjectVector(vector);

    viewer.setWindowTitle("simpleViewer");

    // Make the viewer window visible on screen.
    viewer.show();

    // Run main loop.
    return application.exec();
}
