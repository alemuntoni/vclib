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

#ifndef COMMON_H
#define COMMON_H

#include <vclib/space/core.h>

#include <webgpu/webgpu.h>

// Vertex structure - same as bgfx/opengl2
struct Vertex
{
    float    pos[2];
    uint32_t color; // ABGR format
};

static const Vertex vertices[] = {
    {{-1.0f, -1.0f}, vcl::Color(vcl::Color::Red).abgr()  },
    {{1.0f, -1.0f},  vcl::Color(vcl::Color::Green).abgr()},
    {{0.0f, 1.0f},   vcl::Color(vcl::Color::Blue).abgr() },
};

// Vertex shader - reads from vertex buffer
static const char* vertexShaderCode = R"(
struct VertexInput {
    @location(0) position: vec2f,
    @location(1) color: vec4f,
}

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
}

@vertex
fn vs_main(input: VertexInput) -> VertexOutput {
    var output: VertexOutput;
    output.position = vec4f(input.position, 0.0, 1.0);
    output.color = input.color;
    return output;
}
)";

// Fragment shader WGSL code
static const char* fragmentShaderCode = R"(
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec4f,
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4f {
    return input.color;
}
)";

struct WebGPUShaders
{
    WGPUShaderModule   vertexShader;
    WGPUShaderModule   fragmentShader;
    WGPURenderPipeline pipeline;
    WGPUBuffer         vertexBuffer;
};

inline bool createShaders(
    WGPUDevice        device,
    WGPUTextureFormat format,
    WebGPUShaders&    shaders)
{
    // Create vertex buffer
    WGPUBufferDescriptor bufferDesc = {};
    bufferDesc.label                = "Vertex Buffer";
    bufferDesc.size                 = sizeof(vertices);
    bufferDesc.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;
    bufferDesc.mappedAtCreation = false;

    shaders.vertexBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
    if (!shaders.vertexBuffer) {
        return false;
    }

    // Upload vertex data
    WGPUQueue queue = wgpuDeviceGetQueue(device);
    wgpuQueueWriteBuffer(
        queue, shaders.vertexBuffer, 0, vertices, sizeof(vertices));

    // Create vertex shader
    WGPUShaderModuleWGSLDescriptor vertexShaderCodeDesc = {};
    vertexShaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    vertexShaderCodeDesc.code        = vertexShaderCode;

    WGPUShaderModuleDescriptor vertexShaderDesc = {};
    vertexShaderDesc.nextInChain                = &vertexShaderCodeDesc.chain;
    vertexShaderDesc.label                      = "Vertex Shader";

    shaders.vertexShader =
        wgpuDeviceCreateShaderModule(device, &vertexShaderDesc);
    if (!shaders.vertexShader) {
        wgpuBufferRelease(shaders.vertexBuffer);
        return false;
    }

    // Create fragment shader
    WGPUShaderModuleWGSLDescriptor fragmentShaderCodeDesc = {};
    fragmentShaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    fragmentShaderCodeDesc.code        = fragmentShaderCode;

    WGPUShaderModuleDescriptor fragmentShaderDesc = {};
    fragmentShaderDesc.nextInChain = &fragmentShaderCodeDesc.chain;
    fragmentShaderDesc.label       = "Fragment Shader";

    shaders.fragmentShader =
        wgpuDeviceCreateShaderModule(device, &fragmentShaderDesc);
    if (!shaders.fragmentShader) {
        wgpuShaderModuleRelease(shaders.vertexShader);
        wgpuBufferRelease(shaders.vertexBuffer);
        return false;
    }

    // Define vertex attributes
    WGPUVertexAttribute attributes[2] = {};

    // Position attribute (location 0)
    attributes[0].format         = WGPUVertexFormat_Float32x2;
    attributes[0].offset         = 0;
    attributes[0].shaderLocation = 0;

    // Color attribute (location 1) - RGBA8Unorm for ABGR uint32
    attributes[1].format         = WGPUVertexFormat_Unorm8x4;
    attributes[1].offset         = 8; // after 2 floats (position)
    attributes[1].shaderLocation = 1;

    // Vertex buffer layout
    WGPUVertexBufferLayout vertexBufferLayout = {};
    vertexBufferLayout.arrayStride            = sizeof(Vertex);
    vertexBufferLayout.stepMode               = WGPUVertexStepMode_Vertex;
    vertexBufferLayout.attributeCount         = 2;
    vertexBufferLayout.attributes             = attributes;

    // Create render pipeline
    WGPUColorTargetState colorTarget = {};
    colorTarget.format               = format;
    colorTarget.writeMask            = WGPUColorWriteMask_All;

    WGPUFragmentState fragmentState = {};
    fragmentState.module            = shaders.fragmentShader;
    fragmentState.entryPoint        = "fs_main";
    fragmentState.targetCount       = 1;
    fragmentState.targets           = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.label                        = "Triangle Pipeline";

    pipelineDesc.vertex.module      = shaders.vertexShader;
    pipelineDesc.vertex.entryPoint  = "vs_main";
    pipelineDesc.vertex.bufferCount = 1;
    pipelineDesc.vertex.buffers     = &vertexBufferLayout;

    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
    pipelineDesc.primitive.frontFace        = WGPUFrontFace_CCW;
    pipelineDesc.primitive.cullMode         = WGPUCullMode_None;

    pipelineDesc.multisample.count                  = 1;
    pipelineDesc.multisample.mask                   = ~0u;
    pipelineDesc.multisample.alphaToCoverageEnabled = false;

    pipelineDesc.fragment = &fragmentState;

    pipelineDesc.depthStencil = nullptr;

    shaders.pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
    if (!shaders.pipeline) {
        wgpuShaderModuleRelease(shaders.vertexShader);
        wgpuShaderModuleRelease(shaders.fragmentShader);
        wgpuBufferRelease(shaders.vertexBuffer);
        return false;
    }

    return true;
}

inline void destroyShaders(WebGPUShaders& shaders)
{
    if (shaders.pipeline)
        wgpuRenderPipelineRelease(shaders.pipeline);
    if (shaders.fragmentShader)
        wgpuShaderModuleRelease(shaders.fragmentShader);
    if (shaders.vertexShader)
        wgpuShaderModuleRelease(shaders.vertexShader);
    if (shaders.vertexBuffer)
        wgpuBufferRelease(shaders.vertexBuffer);
}

#endif // COMMON_H
