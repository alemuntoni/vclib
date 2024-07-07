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

#include <any>
#include <array>
#include <cassert>
#include <string>
#include <tuple>
#include <typeindex>
#include <vector>

export module vclib.mesh.containers;

import vclib.concepts;
import vclib.exceptions;
import vclib.io;
import vclib.misc;
import vclib.space.core;
import vclib.types;
import vclib.views;

import vclib.mesh.components;
import vclib.mesh.elements;
import vclib.mesh.element_container;

export {
#include <vclib/mesh/containers/edge_container.h>
#include <vclib/mesh/containers/face_container.h>
#include <vclib/mesh/containers/vertex_container.h>
}


