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

#ifdef VCLIB_WITH_STB
#include <vclib/stb/load_save_image.h>
#endif

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
            std::string path = meshBasePath + t.path();
            // TODO:
            // 1- check for format;
            // 2- check the external library that supports it
            // 3- use the library to load it
#ifdef VCLIB_WITH_STB
            int w, h;
            auto data = vcl::stb::loadImageData(path, w, h);
            if (data) {
                t.image() = vcl::Image(data.get(), w, h);
            }
            else {
                log.log("Cannot load texture " + t.path(), LogType::WARNING_LOG);
            }
#endif
        }
    }
}

/**
 * @brief Save the texture images of a mesh to their respective files.
 *
 * @param[in] mesh
 *
 * @ingroup save
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveTextureImages(const MeshType& mesh, LogType& log = nullLogger)
{
    if constexpr (HasTextureImages<MeshType>) {
        std::string meshBasePath = mesh.meshBasePath();
        for (vcl::Texture& t: mesh.textures()) {
            try {
                std::string path = meshBasePath + t.path();
                // TODO:
                // 1- check for format;
                // 2- check the external library that supports it
                // 3- use the library to save it
    #ifdef VCLIB_WITH_STB
                vcl::stb::saveImageData(
                    path, t.image().width(), t.image().height(), t.image().data());
    #endif
            }
            catch (const std::runtime_error& e) {
                log.log(e.what(), LogType::WARNING_LOG);
            }
        }
    }
}

} // namespace vcl

#endif // VCL_LOAD_SAVE_TEXTURE_IMAGES_H
