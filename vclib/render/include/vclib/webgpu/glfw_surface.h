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

#ifndef VCL_WEBGPU_GLFW_SURFACE_H
#define VCL_WEBGPU_GLFW_SURFACE_H

#include <webgpu/webgpu.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3native.h>
#include <wayland-client.h>
#else
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#endif
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#endif

namespace vcl::webgpu {

/**
 * @brief Create a WebGPU surface from a GLFW window
 * @param instance The WebGPU instance
 * @param window The GLFW window
 * @return The created WebGPU surface, or nullptr on failure
 */
inline WGPUSurface createSurfaceFromGLFW(WGPUInstance instance, GLFWwindow* window)
{
    if (!instance || !window) {
        return nullptr;
    }

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
    // Wayland
    struct wl_display* waylandDisplay = glfwGetWaylandDisplay();
    struct wl_surface* waylandSurface = glfwGetWaylandWindow(window);

    WGPUSurfaceDescriptorFromWaylandSurface waylandSurfaceDesc = {};
    waylandSurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromWaylandSurface;
    waylandSurfaceDesc.display = waylandDisplay;
    waylandSurfaceDesc.surface = waylandSurface;

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &waylandSurfaceDesc.chain;

    return wgpuInstanceCreateSurface(instance, &surfaceDesc);
#else
    // X11
    Display* x11Display = glfwGetX11Display();
    Window x11Window = glfwGetX11Window(window);

    WGPUSurfaceDescriptorFromXlibWindow x11SurfaceDesc = {};
    x11SurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromXlibWindow;
    x11SurfaceDesc.display = x11Display;
    x11SurfaceDesc.window = x11Window;

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &x11SurfaceDesc.chain;

    return wgpuInstanceCreateSurface(instance, &surfaceDesc);
#endif
#elif defined(_WIN32)
    // Windows
    HWND hwnd = glfwGetWin32Window(window);
    HINSTANCE hinstance = GetModuleHandle(NULL);

    WGPUSurfaceDescriptorFromWindowsHWND win32SurfaceDesc = {};
    win32SurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;
    win32SurfaceDesc.hinstance = hinstance;
    win32SurfaceDesc.hwnd = hwnd;

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &win32SurfaceDesc.chain;

    return wgpuInstanceCreateSurface(instance, &surfaceDesc);
#elif defined(__APPLE__)
    // macOS
    id metalLayer = nullptr;
    NSWindow* nsWindow = glfwGetCocoaWindow(window);
    [nsWindow.contentView setWantsLayer:YES];
    metalLayer = [CAMetalLayer layer];
    [nsWindow.contentView setLayer:metalLayer];

    WGPUSurfaceDescriptorFromMetalLayer metalSurfaceDesc = {};
    metalSurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromMetalLayer;
    metalSurfaceDesc.layer = metalLayer;

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &metalSurfaceDesc.chain;

    return wgpuInstanceCreateSurface(instance, &surfaceDesc);
#else
    return nullptr;
#endif
}

} // namespace vcl::webgpu

#endif // VCL_WEBGPU_GLFW_SURFACE_H
