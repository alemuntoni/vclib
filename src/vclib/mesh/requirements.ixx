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

export module vclib.mesh.requirements;

export import vclib.mesh.requirements.element_requirements;

import vclib.concepts.mesh.containers;
import vclib.concepts.mesh.element_mesh_concept;
import vclib.concepts.mesh.mesh_concept;
import vclib.concepts.mesh.per_element;
import vclib.exceptions;
import vclib.types;

export {
#include <vclib/mesh/requirements/edge_requirements.h>
#include <vclib/mesh/requirements/face_requirements.h>
#include <vclib/mesh/requirements/mesh_requirements.h>
#include <vclib/mesh/requirements/vertex_requirements.h>
}

