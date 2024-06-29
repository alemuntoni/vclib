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

#include <ranges>
#include <tuple>
#include <type_traits>
#include <vector>

export module vclib.mesh.components.bases;

import vclib.concepts.mesh.components.component;
import vclib.concepts.mesh.mesh_concept;
import vclib.concepts.ranges.range;
import vclib.iterators.const_pointer_iterator;
import vclib.iterators.mesh;
import vclib.mesh.components.detail;
import vclib.space.vector;
import vclib.types;
import vclib.views.view;

export {
#include <vclib/mesh/components/bases/component.h>
#include <vclib/mesh/components/bases/container_component.h>
#include <vclib/mesh/components/bases/index_container_component.h>
#include <vclib/mesh/components/bases/pointer_container_component.h>

#include <vclib/mesh/components/bases/reference_container_component.h>
}
