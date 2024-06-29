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

module;

#include <string>

export module vclib.io.mesh.load;

import vclib.concepts;
import vclib.exceptions;
import vclib.io.file_info;
import vclib.io.mesh.settings;
import vclib.mesh.utils.mesh_info;
import vclib.misc;

export import vclib.io.mesh.obj.load;
export import vclib.io.mesh.off.load;
export import vclib.io.mesh.ply.load;
export import vclib.io.mesh.stl.load;

export {
#include <vclib/io/mesh/load.h>
}
