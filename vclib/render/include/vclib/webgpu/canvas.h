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

#ifndef VCL_WEBGPU_CANVAS_H
#define VCL_WEBGPU_CANVAS_H

#include <vclib/render/concepts/render_app.h>
#include <vclib/render/read_buffer_types.h>

#include <vclib/space.h>
#include <vclib/types.h>

namespace vcl {

template<typename DerivedRenderApp>
class CanvasWebGPU
{
    Point2<uint> mSize = {0, 0};

public:
    using CallbackReadBuffer = ReadBufferTypes::CallbackReadBuffer;

    CanvasWebGPU(
        void* winId,
        uint  width,
        uint  height,
        void* displayId = nullptr)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");
    }

    Point2<uint> size() const { return mSize; }

    uint viewId() const { return 0; }

    void setDefaultClearColor(Color color) {}

    void onInit() {}

    void onResize(uint width, uint height) {}

    void onPaint() {}

    bool onReadDepth(const Point2i& point, CallbackReadBuffer callback)
    {
        return false;
    }

    bool onScreenshot(
        const std::string& filename,
        uint               width  = 0,
        uint               height = 0)
    {
        return false;
    }
};

} // namespace vcl

#endif // VCL_WEBGPU_CANVAS_H
