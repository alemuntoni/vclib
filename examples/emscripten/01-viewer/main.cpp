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

#include <vclib/algorithms/mesh/create/sphere.h>
#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/emscripten/viewer_window.h>
#include <vclib/meshes.h>
#include <vclib/render/drawable/drawable_mesh.h>

// The ViewerWindow must outlive main() because show() returns immediately and
// the browser drives the render loop via requestAnimationFrame.
// A global unique_ptr is the standard Emscripten pattern for this.
static std::unique_ptr<vcl::emscripten::ViewerWindow> gViewer;

int main()
{
    gViewer = std::make_unique<vcl::emscripten::ViewerWindow>("vclib viewer");

    // Build a unit sphere programmatically (no file I/O needed in the browser)
    vcl::TriMesh sphere = vcl::createSphere<vcl::TriMesh>();
    vcl::updatePerFaceNormals(sphere);
    vcl::updatePerVertexNormalsFromFaceNormals(sphere);
    vcl::setPerVertexColor(sphere, vcl::Color(vcl::Color::LightBlue));

    vcl::DrawableMesh<vcl::TriMesh> drawable(sphere);
    gViewer->pushDrawableObject(drawable);
    gViewer->fitScene();

    // Starts the rAF loop and returns immediately.
    gViewer->show();

    return 0;
}
