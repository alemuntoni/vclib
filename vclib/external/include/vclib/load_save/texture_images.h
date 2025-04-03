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

#ifndef VCL_LOAD_SAVE_TEXTURE_IMAGES_H
#define VCL_LOAD_SAVE_TEXTURE_IMAGES_H

#include <vclib/io/image.h>

#include <vclib/concepts/mesh.h>
#include <vclib/misc/logger.h>
#include <vclib/space/core/texture.h>

namespace vcl {

/**
 * @brief Load the texture images of a mesh, populating the image field of each
 * texture with the image data.
 *
 * @param[in] mesh
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadTextureImages(MeshType& mesh, LogType& log = nullLogger)
{
    if constexpr (HasTextureImages<MeshType>) {
        std::string meshBasePath = mesh.meshBasePath();
        for (vcl::Texture& t: mesh.textures()) {
            try {
                std::string path = meshBasePath + t.path();
                int w, h;
                auto data = vcl::loadImageData(path, w, h);
                if (data) {
                    t.image() = vcl::Image(data.get(), w, h);
                }
            }
            catch (const std::runtime_error& e) {
                log.log(e.what(), LogType::WARNING_LOG);
            }
        }
    }
}

/**
 * @brief Save the texture images of a mesh to their respective files.
 *
 * The images are saved by default in the directory given by the meshBasePath of
 * the mesh (`mesh.meshBasePath()`). This path can be overridden by
 * providing a different `meshFilePath` trough the @par meshFilePath argument.
 *
 * @param[in] mesh: the mesh to save the texture images from.
 * @param[in] meshFilePath: the path to the directory where the images will be
 * saved. If empty, the meshBasePath of the mesh will be used.
 *
 * @ingroup save
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveTextureImages(
    const MeshType&    mesh,
    const std::string& meshFilePath = "",
    LogType&           log      = nullLogger)
{
    if constexpr (HasTextureImages<MeshType>) {
        std::string meshBasePath = mesh.meshBasePath();
        if (!meshFilePath.empty()) {
            meshBasePath = meshFilePath;
        }
        for (vcl::Texture& t : mesh.textures()) {
            try {
                std::string path = meshBasePath + t.path();
                vcl::saveImageData(
                    path,
                    t.image().width(),
                    t.image().height(),
                    t.image().data());
            }
            catch (const std::runtime_error& e) {
                log.log(e.what(), LogType::WARNING_LOG);
            }
        }
    }
}

} // namespace vcl

#endif // VCL_LOAD_SAVE_TEXTURE_IMAGES_H
