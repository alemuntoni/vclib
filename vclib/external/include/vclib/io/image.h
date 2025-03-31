/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_IO_IMAGE_H
#define VCL_IO_IMAGE_H

#include <vclib/io/image_bmp.h>

#ifdef VCLIB_WITH_STB
#include <vclib/stb/load_save_image.h>
#endif

#include <vclib/space/core/image.h>

namespace vcl {

inline std::shared_ptr<unsigned char> loadImageData(
    const std::string& filename,
    int&               w,
    int&               h)
{
#ifdef VCLIB_WITH_STB
    return stb::loadImageData(filename, w, h);
#else
    return nullptr;
#endif
}

inline Image loadImage(const std::string& filename)
{
    int w, h;
    auto data = loadImageData(filename, w, h);
    if (!data) {
        return Image();
    }
    return Image(data.get(), w, h);
}

inline void saveImageData(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data,
    uint                 quality = 90)
{
#ifdef VCLIB_WITH_STB
    stb::saveImageData(filename, w, h, data, quality);
#else
    // save rgb image data into bmp file
    saveImageToBmp(filename, w, h, data);
#endif
}

inline void saveImage(const std::string& filename, const Image& image)
{
    saveImageData(filename, image.width(), image.height(), image.data());
}

} // namespace vcl

#endif // VCL_IO_IMAGE_H
