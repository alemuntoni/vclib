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

#ifndef HELLO_TRIANGLE_DRAWER_H
#define HELLO_TRIANGLE_DRAWER_H

#include "common.h"

#include <vclib/render/drawers/plain_drawer.h>

template<typename DerivedRenderApp>
class HelloTriangleDrawer : public vcl::PlainDrawer<DerivedRenderApp>
{
    WebGPUShaders mShaders = {};
public:
    HelloTriangleDrawer(vcl::uint width = 1024, vcl::uint height = 768) {}

    ~HelloTriangleDrawer()
    {
        destroyShaders(mShaders);
    }

    void onInit(vcl::uint viewId) override
    {
        // Get device from canvas
        auto* canvas = static_cast<DerivedRenderApp*>(this);
        WGPUDevice device = canvas->device();
        WGPUTextureFormat format = canvas->swapChainFormat();

        if (!createShaders(device, format, mShaders)) {
            std::cerr << "Failed to create shaders" << std::endl;
        }
        else {
            std::cout << "WebGPU shaders created successfully" << std::endl;
        }
    }

    void onResize(vcl::uint width, vcl::uint height) override
    {
        std::cout << "Resize: " << width << "; " << height
                  << ". Nothing to do\n";
    }

    void onDrawContent(vcl::uint viewId) override
    {
        if (!mShaders.pipeline || !mShaders.vertexBuffer) {
            return;
        }

        // Get canvas to access WebGPU render pass
        auto* canvas = static_cast<DerivedRenderApp*>(this);
        WGPURenderPassEncoder renderPass = canvas->getCurrentRenderPass();
        
        if (renderPass) {
            // Set pipeline
            wgpuRenderPassEncoderSetPipeline(renderPass, mShaders.pipeline);
            
            // Bind vertex buffer
            wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, mShaders.vertexBuffer, 0, sizeof(vertices));
            
            // Draw 3 vertices
            wgpuRenderPassEncoderDraw(renderPass, 3, 1, 0, 0);
        }
    }

    void onDraw(vcl::uint viewId) override { onDrawContent(viewId); }

    WGPURenderPipeline getPipeline() const { return mShaders.pipeline; }
};

#endif // HELLO_TRIANGLE_DRAWER_H
