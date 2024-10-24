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
#include <array>
#include <bit>
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <typeindex>
#include <vector>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

export module vclib.io;

import vclib.concepts;
import vclib.exceptions;
import vclib.misc;
import vclib.types;

export {
// serialization
#include <vclib/io/serialization/endian.h>

#include <vclib/io/serialization/deserialize.h>
#include <vclib/io/serialization/serialize.h>

// file_info and file_format
#include <vclib/io/file_format.h>
#include <vclib/io/file_info.h>
#include <vclib/io/file_type.h>

// image, read, write
#include <vclib/io/image.h>
#include <vclib/io/read.h>
#include <vclib/io/write.h>
}
