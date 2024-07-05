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

#include <cassert>
#include <concepts>
#include <limits>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <utility>
#include <zip_view.hpp>

export module vclib.types;

export {
#include <vclib/types/base.h>
#include <vclib/types/const_correctness.h>
#include <vclib/types/inheritance.h>
#include <vclib/types/mesh_components.h>
#include <vclib/types/mesh_elements.h>
#include <vclib/types/pointers.h>
#include <vclib/types/variadic_templates.h>
#include <vclib/types/view.h>
}
