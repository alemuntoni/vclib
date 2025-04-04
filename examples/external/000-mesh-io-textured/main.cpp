/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/load_save.h>
#include <vclib/meshes.h>

#include <vclib/load_save/texture_images.h>

int main()
{
    vcl::MeshInfo loadedInfo;
    vcl::TriMesh  m = vcl::loadObj<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.obj", loadedInfo);
    vcl::loadTextureImages(m);


    m.texture(0).path() = "texture_0.jpg";
#ifdef VCLIB_WITH_QT
    m.texture(1).path() = "texture_1.xpm";
#else
    m.texture(1).path() = "texture_1.bmp";
#endif

    vcl::save(m, VCLIB_RESULTS_PATH "/TwoTextures.obj");

    // two ways to save the textures:
    // 1) give the output path to the vcl::saveTextureImages
    vcl::saveTextureImages(m, VCLIB_RESULTS_PATH "/");

    // 2) set the mesh base path to the mesh
    //m.meshBasePath() = VCLIB_RESULTS_PATH "/";
    //vcl::saveTextureImages(m);

    return 0;
}
