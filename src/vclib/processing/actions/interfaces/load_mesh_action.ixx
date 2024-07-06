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

#include <memory>
#include <string>
#include <vector>

#include <Eigen/Core>

export module vclib.processing.actions.interfaces.load_mesh_action;

import vclib.algorithms.mesh.update;
import vclib.concepts;
import vclib.misc;
import vclib.processing.actions.common.file_format;
import vclib.processing.actions.common.parameter_vector;
import vclib.processing.actions.interfaces.action;
import vclib.processing.actions.interfaces.mesh_action;
import vclib.processing.meshes;
import vclib.processing.settings;
import vclib.space.complex;

export {
#include <vclib/processing/actions/interfaces/load_mesh_action.h>
}
