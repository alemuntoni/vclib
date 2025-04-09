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

#include <algorithm>
#include <bit>
#include <cassert>
#include <fstream>
#include <list>
#include <ostream>
#include <string>
#include <vector>

#include <Eigen/Core>

export module vclib.io:mesh_ply;

import vclib.concepts;
import vclib.exceptions;
import vclib.mesh;
import vclib.misc;
import vclib.space.complex;
import vclib.space.core;
import vclib.types;

import :common;
import :image;
import :mesh_settings;

export {
#include <vclib/io/mesh/ply/detail/ply.h>

#include <vclib/io/mesh/ply/detail/header.h>

#include <vclib/io/mesh/ply/detail/edge.h>
#include <vclib/io/mesh/ply/detail/extra.h>
#include <vclib/io/mesh/ply/detail/face.h>
#include <vclib/io/mesh/ply/detail/tristrip.h>
#include <vclib/io/mesh/ply/detail/vertex.h>

#include <vclib/io/mesh/ply/capability.h>
#include <vclib/io/mesh/ply/load.h>
#include <vclib/io/mesh/ply/save.h>
}
