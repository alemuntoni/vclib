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
#include <any>
#include <memory>
#include <string>
#include <vector>

export module vclib.processing:parameters;

import vclib.space.core;
import vclib.types;

import :settings;

export {
#include <vclib/processing/parameters/parameter.h>

#include <vclib/processing/parameters/bool_parameter.h>
#include <vclib/processing/parameters/enum_parameter.h>
#include <vclib/processing/parameters/int_parameter.h>
#include <vclib/processing/parameters/mesh_parameter.h>
#include <vclib/processing/parameters/scalar_parameter.h>
#include <vclib/processing/parameters/string_parameter.h>
#include <vclib/processing/parameters/uint_parameter.h>
#include <vclib/processing/parameters/uscalar_parameter.h>

#include <vclib/processing/parameter_vector.h>
}
