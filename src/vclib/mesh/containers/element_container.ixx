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
#include <typeindex>
#include <vector>

export module vclib.mesh.containers.element_container;

import vclib.concepts;
import vclib.io;
import vclib.mesh.containers.custom_component_vector_handle;
import vclib.mesh.containers.custom_components_vector_map;
import vclib.mesh.containers.vertical_components_vector_tuple;
import vclib.mesh.iterators;
import vclib.types;

export {
#include <vclib/mesh/containers/element_container.h>
}


