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

#ifndef VCL_PROCESSING_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H
#define VCL_PROCESSING_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H

#include <vclib/processing/engine.h>

#ifdef VCLIB_WITH_STB
#include <vclib/io/image.h>
#endif

namespace vcl::proc {

class BaseImageIO : public ImageIOAction
{
public:
    std::string name() const final { return "Base IO Image"; }

    IOSupport ioSupport() const final
    {
#ifdef VCLIB_WITH_STB
        return IOSupport::BOTH;
#else
        return IOIOSupport::SAVE;
#endif
    }

    std::vector<FileFormat> supportedFormats() const final
    {
        std::vector<FileFormat> formats;
        formats.push_back(FileFormat("bmp", "Bitmap"));
#ifdef VCLIB_WITH_STB
        formats.push_back(FileFormat("png", "Portable Network Graphics"));
        formats.push_back(FileFormat("tga", "Truevision TGA"));
        formats.push_back(FileFormat(
            std::vector<std::string> {"jpg", "jpeg"},
            "Joint Photographic Experts Group"));
#endif
        return formats;
    }

    Image load(const std::string& filename, AbstractLogger& log = logger())
        const final
    {
#ifdef VCLIB_WITH_STB
        int w, h;
        std::shared_ptr<unsigned char> data = loadImageData(filename, w, h);
        return Image(data.get(), w, h);
#else
        throw std::runtime_error("Image loading is not supported");
#endif
    }

    void save(
        const std::string& filename,
        const Image&       image,
        AbstractLogger&    log = logger()) const final
    {
        assert(!image.isNull());
#ifdef VCLIB_WITH_STB
        saveImageData(filename, image.width(), image.height(), image.data());
#else
        saveImageToBmp(filename, image.width(), image.height(), image.data());
#endif
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_IMAGE_IO_BASE_IMAGE_IO_H
