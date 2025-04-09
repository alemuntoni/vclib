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
#include <set>
#include <string>
#include <typeindex>
#include <vector>

#ifdef VCLIB_WITH_QT
#include <QImage>
#endif

#ifdef VCLIB_WITH_STB
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#endif

export module vclib.io:image;

import vclib.concepts;
import vclib.exceptions;
import vclib.misc;
import vclib.space.core;
import vclib.types;

import :common;

export {
#include <vclib/io/image/bmp/save.h>

#ifdef VCLIB_WITH_QT
#include <vclib/io/image/qt/load.h>
#include <vclib/io/image/qt/save.h>
#endif

#ifdef VCLIB_WITH_STB
#include <vclib/io/image/stb/load.h>
#include <vclib/io/image/stb/save.h>
#endif

#include <vclib/io/image/load.h>
#include <vclib/io/image/save.h>
}
