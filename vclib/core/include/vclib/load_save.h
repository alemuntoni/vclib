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

#ifndef VCL_LOAD_SAVE_H
#define VCL_LOAD_SAVE_H

#include "load_save/capability.h"
#include "load_save/formats.h"
#include "load_save/load.h"
#include "load_save/save.h"

/**
 * @defgroup load_save Load/Save functions
 *
 * @brief List of functions that allow to load from file or save to a file an
 * input Mesh.
 *
 * @note Since the library does not support native loading and saving of images,
 * eventual texture images are not loaded/saved. Only the paths to the texture
 * images are loaded/saved. If the MeshType supports texture images, the user
 * can load/save the textures by calling the [load/save]TextureImages function
 * provided in the external module of the library.
 */

#endif // VCL_LOAD_SAVE_H
