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

#ifndef VCL_LOAD_SAVE_SETTINGS_H
#define VCL_LOAD_SAVE_SETTINGS_H

#include <vclib/space/complex/mesh_info.h>

namespace vcl {

/**
 * @brief The LoadSettings structure contains the settings that can be used to
 * load a mesh from a stream/file.
 */
struct LoadSettings
{
    /**
     * @brief If true, some eventual optional components of the mesh that were
     * not enabled and that can be loaded from the stream/file, will be enabled
     * before loading the stream/file.
     *
     * It applies to all the loading functions.
     */
    bool enableOptionalComponents = true;
};

/**
 * @brief The SaveSettings structure contains the settings that can be used to
 * save a mesh to a stream/file.
 */
struct SaveSettings
{
    /**
     * @brief If true, the mesh will be saved in binary format. Otherwise, it
     * will be saved in ASCII format.
     *
     * It applies to all the saving functions that save to a file format that
     * supports both binary and ASCII format.
     */
    bool binary = true;

    /**
     * @brief Applied only to STL binary files. Magics mode is a specific file
     * format used by the Magics software, which includes more information than
     * the standard STL format, like face colors. The magicsMode works only when
     * binary flag is set to `true`.
     */
    bool magicsMode = false;

    /**
     * @brief Data structure that tells the saving functions which components of
     * the mesh should be saved. Only the components that can be saved in the
     * file format will be saved.
     * If the info is uninitialized, all the components that can be saved will
     * be saved.
     */
    MeshInfo info;
};

} // namespace vcl

#endif // VCL_LOAD_SAVE_SETTINGS_H
