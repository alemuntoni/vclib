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

#include <vclib/space/core/color.h>
#include <vclib/space/core/point.h>

#include <webgpu/webgpu.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#undef KeyPress // avoid conflict with X.h
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#endif

#include <iostream>

namespace vcl {

/**
 * @brief The CanvasWebGPU class - simplified version for basic rendering
 */
template<typename DerivedRenderApp>
class CanvasWebGPU
{
public:
    using CallbackReadBuffer = ReadBufferTypes::CallbackReadBuffer;

private:
    GLFWwindow* mGLFWWindow = nullptr;
    Point2<uint> mSize = {0, 0};
    vcl::Color mDefaultClearColor = vcl::Color::Black;

    // WebGPU objects - minimal set
    WGPUInstance  mInstance  = nullptr;
    WGPUSurface   mSurface   = nullptr;
    WGPUAdapter   mAdapter   = nullptr;
    WGPUDevice    mDevice    = nullptr;
    WGPUQueue     mQueue     = nullptr;
    WGPUTextureFormat mSwapChainFormat = WGPUTextureFormat_BGRA8Unorm;
    
    WGPURenderPassEncoder mCurrentRenderPass = nullptr;

public:
    CanvasWebGPU(
        void* winId,
        uint  width,
        uint  height,
        void* displayId = nullptr) : 
            mGLFWWindow(static_cast<GLFWwindow*>(displayId)),
            mSize(width, height)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");
    }

    ~CanvasWebGPU()
    {
        if (mQueue) wgpuQueueRelease(mQueue);
        if (mDevice) wgpuDeviceRelease(mDevice);
        if (mAdapter) wgpuAdapterRelease(mAdapter);
        if (mSurface) wgpuSurfaceRelease(mSurface);
        if (mInstance) wgpuInstanceRelease(mInstance);
    }

    Point2<uint> size() const { return mSize; }
    uint viewId() const { return 0; }
    WGPUDevice device() const { return mDevice; }
    WGPUQueue queue() const { return mQueue; }
    WGPUTextureFormat swapChainFormat() const { return mSwapChainFormat; }
    WGPURenderPassEncoder getCurrentRenderPass() const { return mCurrentRenderPass; }

    /**
     * @brief Request a screenshot of the canvas.
     *     The screenshot will be saved asynchronously.
     * @param filename The filename where the screenshot will be saved.
     * @param multiplier The multiplier applied to the canvas image.
     * @return true if the screenshot is requested, false otherwise.
     */
    bool screenshot(const std::string& filename, uint multiplier = 1)
    {
        // todo
        return false;
    }

    void setDefaultClearColor(const Color& color)
    {
        mDefaultClearColor = color;
    }

    void onInit()
    {
        std::cout << "WebGPU Canvas: Creating instance..." << std::endl;
        
        // 1. Create instance
        WGPUInstanceDescriptor instanceDesc = {};
        mInstance = wgpuCreateInstance(&instanceDesc);
        if (!mInstance) {
            std::cerr << "Failed to create WebGPU instance" << std::endl;
            return;
        }

        // 2. Create surface
        if (!mGLFWWindow) {
            std::cerr << "GLFW window is null" << std::endl;
            return;
        }
        
        mSurface = createSurface();
        if (!mSurface) {
            std::cerr << "Failed to create WebGPU surface" << std::endl;
            return;
        }

        // 3. Request adapter (synchronous for simplicity)
        WGPURequestAdapterOptions adapterOpts = {};
        adapterOpts.compatibleSurface = mSurface;
        
        struct UserData {
            WGPUAdapter adapter;
            bool done;
        } userData = {nullptr, false};

        wgpuInstanceRequestAdapter(
            mInstance,
            &adapterOpts,
            [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
               char const* message, void* userdata) {
                UserData* data = (UserData*)userdata;
                if (status == WGPURequestAdapterStatus_Success) {
                    data->adapter = adapter;
                } else {
                    std::cerr << "Adapter request failed: " << (message ? message : "unknown") << std::endl;
                }
                data->done = true;
            },
            &userData);

        // Wait for callback
        while (!userData.done) {
            // In a real app, use proper event loop
        }

        mAdapter = userData.adapter;
        if (!mAdapter) {
            std::cerr << "Failed to get adapter" << std::endl;
            return;
        }

        // 4. Request device
        WGPUDeviceDescriptor deviceDesc = {};
        deviceDesc.label = "VCLib Device";

        struct DeviceUserData {
            WGPUDevice device;
            bool done;
        } deviceUserData = {nullptr, false};

        wgpuAdapterRequestDevice(
            mAdapter,
            &deviceDesc,
            [](WGPURequestDeviceStatus status, WGPUDevice device,
               char const* message, void* userdata) {
                DeviceUserData* data = (DeviceUserData*)userdata;
                if (status == WGPURequestDeviceStatus_Success) {
                    data->device = device;
                } else {
                    std::cerr << "Device request failed: " << (message ? message : "unknown") << std::endl;
                }
                data->done = true;
            },
            &deviceUserData);

        while (!deviceUserData.done) {
            // Wait for callback
        }

        mDevice = deviceUserData.device;
        if (!mDevice) {
            std::cerr << "Failed to get device" << std::endl;
            return;
        }

        // 5. Get queue
        mQueue = wgpuDeviceGetQueue(mDevice);

        std::cout << "WebGPU Canvas initialized successfully" << std::endl;
    }

    void onResize(uint width, uint height)
    {
        mSize = {width, height};
        // Per ora ignoriamo il resize - lo implementeremo dopo
    }

    void onPaint()
    {
        if (!mDevice || !mSurface) {
            return;
        }

        // Configure surface
        WGPUSurfaceConfiguration config = {};
        config.device = mDevice;
        config.format = mSwapChainFormat;
        config.usage = WGPUTextureUsage_RenderAttachment;
        config.width = mSize.x();
        config.height = mSize.y();
        config.presentMode = WGPUPresentMode_Fifo;
        config.alphaMode = WGPUCompositeAlphaMode_Opaque;
        
        wgpuSurfaceConfigure(mSurface, &config);

        // Get current texture
        WGPUSurfaceTexture surfaceTexture;
        wgpuSurfaceGetCurrentTexture(mSurface, &surfaceTexture);
        
        if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
            std::cerr << "Failed to get surface texture" << std::endl;
            return;
        }

        // Create view for the texture
        WGPUTextureViewDescriptor viewDesc = {};
        viewDesc.format = mSwapChainFormat;
        viewDesc.dimension = WGPUTextureViewDimension_2D;
        viewDesc.baseMipLevel = 0;
        viewDesc.mipLevelCount = 1;
        viewDesc.baseArrayLayer = 0;
        viewDesc.arrayLayerCount = 1;
        viewDesc.aspect = WGPUTextureAspect_All;
        
        WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDesc);

        // Create command encoder
        WGPUCommandEncoderDescriptor encoderDesc = {};
        encoderDesc.label = "Command Encoder";
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(mDevice, &encoderDesc);

        // Setup render pass
        WGPURenderPassColorAttachment colorAttachment = {};
        colorAttachment.view = targetView;
        colorAttachment.loadOp = WGPULoadOp_Clear;
        colorAttachment.storeOp = WGPUStoreOp_Store;
        colorAttachment.clearValue.r = mDefaultClearColor.redF();
        colorAttachment.clearValue.g = mDefaultClearColor.greenF();
        colorAttachment.clearValue.b = mDefaultClearColor.blueF();
        colorAttachment.clearValue.a = mDefaultClearColor.alphaF();

        WGPURenderPassDescriptor renderPassDesc = {};
        renderPassDesc.colorAttachmentCount = 1;
        renderPassDesc.colorAttachments = &colorAttachment;

        mCurrentRenderPass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);

        // Call drawer
        DerivedRenderApp::CNV::draw(derived());

        wgpuRenderPassEncoderEnd(mCurrentRenderPass);
        wgpuRenderPassEncoderRelease(mCurrentRenderPass);
        mCurrentRenderPass = nullptr;

        // Submit
        WGPUCommandBufferDescriptor cmdBufferDesc = {};
        cmdBufferDesc.label = "Command Buffer";
        WGPUCommandBuffer commands = wgpuCommandEncoderFinish(encoder, &cmdBufferDesc);
        wgpuQueueSubmit(mQueue, 1, &commands);

        // Present
        wgpuSurfacePresent(mSurface);

        // Cleanup
        wgpuCommandBufferRelease(commands);
        wgpuCommandEncoderRelease(encoder);
        wgpuTextureViewRelease(targetView);

        DerivedRenderApp::CNV::postDraw(derived());
    }

    // Stub implementations - not needed for basic triangle
    bool onReadDepth(const Point2i& point, CallbackReadBuffer callback = nullptr)
    {
        return false;
    }

    bool onScreenshot(const std::string& filename, uint multiplier = 1)
    {
        return false;
    }

    bool onReadId(const Point2i& point, CallbackReadBuffer callback = nullptr)
    {
        return false;
    }

private:
    WGPUSurface createSurface()
    {
#if defined(__linux__)
        Display* x11Display = glfwGetX11Display();
        Window x11Window = glfwGetX11Window(mGLFWWindow);

        WGPUSurfaceDescriptorFromXlibWindow x11SurfaceDesc = {};
        x11SurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromXlibWindow;
        x11SurfaceDesc.display = x11Display;
        x11SurfaceDesc.window = x11Window;

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = (const WGPUChainedStruct*)&x11SurfaceDesc;

        return wgpuInstanceCreateSurface(mInstance, &surfaceDesc);
#elif defined(_WIN32)
        HWND hwnd = glfwGetWin32Window(mGLFWWindow);
        HINSTANCE hinstance = GetModuleHandle(NULL);

        WGPUSurfaceDescriptorFromWindowsHWND win32SurfaceDesc = {};
        win32SurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;
        win32SurfaceDesc.hinstance = hinstance;
        win32SurfaceDesc.hwnd = hwnd;

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = (const WGPUChainedStruct*)&win32SurfaceDesc;

        return wgpuInstanceCreateSurface(mInstance, &surfaceDesc);
#elif defined(__APPLE__)
        id nsWindow = glfwGetCocoaWindow(mGLFWWindow);
        // TODO: implementare per macOS
        return nullptr;
#else
        return nullptr;
#endif
    }

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }
    const auto* derived() const { return static_cast<const DerivedRenderApp*>(this); }
};

} // namespace vcl

#endif // VCL_WEBGPU_CANVAS_H
