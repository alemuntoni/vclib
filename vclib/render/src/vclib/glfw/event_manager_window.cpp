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

#include <vclib/glfw/event_manager_window.h>

#include <iostream>

#include <vclib/glfw/input.h>

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
#define GLFW_EXPOSE_NATIVE_WAYLAND
#else
#define GLFW_EXPOSE_NATIVE_X11
#endif
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

namespace vcl::glfw {

namespace detail {

void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error: " << error << ": " << description << std::endl;
}

} // namespace detail

EventManagerWindow::EventManagerWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height) :
        mTitle(windowTitle)
{
    glfwSetErrorCallback(detail::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#endif

    mWindow =
        glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if (!mWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    glfwMakeContextCurrent(mWindow);
#endif

    glfwSetWindowUserPointer(mWindow, this);

    setCallbacks();
}

const std::string& EventManagerWindow::windowTitle() const
{
    return mTitle;
}

void EventManagerWindow::setWindowTitle(const std::string& title)
{
    mTitle = title;
    glfwSetWindowTitle(mWindow, mTitle.c_str());
}

uint EventManagerWindow::width() const
{
    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);
    return width;
}

uint EventManagerWindow::height() const
{
    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);
    return height;
}

void* EventManagerWindow::winId()
{
    void* nwh = nullptr;

#if defined(__linux__)
#ifdef VCLIB_RENDER_WITH_WAYLAND
    nwh = (void*) (uintptr_t) glfwGetWaylandWindow(mWindow);
#else
    nwh = (void*) (uintptr_t) glfwGetX11Window(mWindow);
#endif
#elif defined(_WIN32)
    nwh = glfwGetWin32Window(mWindow);
#elif defined(__APPLE__)
    nwh = glfwGetCocoaWindow(mWindow);
#endif

    return nwh;
}

void* EventManagerWindow::displayId()
{
    void* ndt = nullptr;
#ifdef __linux__
#ifdef VCLIB_RENDER_WITH_WAYLAND
    ndt = (void*) (uintptr_t) glfwGetWaylandDisplay();
#else
    ndt = (void*) (uintptr_t) glfwGetX11Display();
#endif
#endif
    return ndt;
}

void EventManagerWindow::glfwWindowSizeCallback(
    GLFWwindow*,
    int width,
    int height)
{
    onResize(width, height);
}

void EventManagerWindow::glfwKeyCallback(
    GLFWwindow*,
    int key,
    int,
    int action,
    int)
{
    vcl::Key::Enum k = glfw::fromGLFW((glfw::Key) key);
    // GLFW modifier does not work as expected: modifiers are not updated
    // when a key modifier is released. We have to handle this manually.
    if (action == GLFW_PRESS) {
        if (isModifierKey(k)) {
            KeyModifiers mods                 = modifiers();
            mods.at(KeyModifier::NO_MODIFIER) = false;
            mods.at(keyToModifier(k))         = true;
            setModifiers(mods);
        }
        onKeyPress(k);
    }
    else if (action == GLFW_RELEASE) {
        if (isModifierKey(k)) {
            KeyModifiers mods      = modifiers();
            mods[keyToModifier(k)] = false;
            if (mods.none())
                mods[KeyModifier::NO_MODIFIER] = true;
            setModifiers(mods);
        }
        onKeyRelease(k);
    }
}

void EventManagerWindow::glfwMouseButtonCallback(
    GLFWwindow*,
    int button,
    int action,
    int)
{
    glfw::MouseButton btn = (glfw::MouseButton) button;

    if (action == GLFW_PRESS) {
        onMousePress(glfw::fromGLFW(btn));
    }
    else if (action == GLFW_RELEASE) {
        onMouseRelease(glfw::fromGLFW(btn));
    }
}

void EventManagerWindow::glfwCursorPosCallback(
    GLFWwindow*,
    double xpos,
    double ypos)
{
    onMouseMove(xpos, ypos);
}

void EventManagerWindow::glfwScrollCallback(
    GLFWwindow*,
    double xoffset,
    double yoffset)
{
    onMouseScroll(xoffset * 120, yoffset * 120);
}

void EventManagerWindow::setCallbacks()
{
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    glfwSetWindowSizeCallback(
        mWindow, [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwWindowSizeCallback(window, width, height);
        });
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    glfwSetFramebufferSizeCallback(
        mWindow, [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwWindowSizeCallback(window, width, height);
        });
#endif

    // key callback
    glfwSetKeyCallback(
        mWindow,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwKeyCallback(window, key, scancode, action, mods);
        });

    // mouse position callback
    glfwSetCursorPosCallback(
        mWindow, [](GLFWwindow* window, double xpos, double ypos) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwCursorPosCallback(window, xpos, ypos);
        });

    // mouse button callback
    glfwSetMouseButtonCallback(
        mWindow, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwMouseButtonCallback(window, button, action, mods);
        });

    // scroll callback
    glfwSetScrollCallback(
        mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* self = static_cast<EventManagerWindow*>(
                glfwGetWindowUserPointer(window));
            self->glfwScrollCallback(window, xoffset, yoffset);
        });
}

} // namespace vcl::glfw
