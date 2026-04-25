/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_EMSCRIPTEN_WINDOW_MANAGER_H
#define VCL_EMSCRIPTEN_WINDOW_MANAGER_H

#include "input.h"

#include <vclib/render/concepts/render_app.h>
#include <vclib/render/window_managers.h>
#include <vclib/space/core/point.h>

#include <emscripten.h>
#include <emscripten/html5.h>

#include <string>

namespace vcl::emscripten {

/**
 * @brief Emscripten/WebAssembly window manager for vclib's RenderApp.
 *
 * Satisfies the @ref vcl::WindowManagerConcept by managing the browser canvas
 * element instead of a native OS window.  The main loop is driven by the
 * browser's animation-frame scheduler via
 * `emscripten_request_animation_frame_loop`, so `show()` returns immediately
 * and never blocks.
 *
 * Input events (mouse, wheel, keyboard) are forwarded to the RenderApp via
 * the HTML5 Emscripten callback API.
 *
 * @tparam DerivedRenderApp The concrete RenderApp type (CRTP).
 */
template<typename DerivedRenderApp>
class WindowManager
{
    std::string mTitle;

    // CSS selector for the canvas element (default matches the Emscripten
    // HTML shell which creates <canvas id="canvas">)
    std::string mCanvasTarget = "#canvas";

    // Cached logical (CSS) size in pixels, updated when the canvas resizes.
    uint mWidth  = 1024;
    uint mHeight = 768;

    // Device pixel ratio (physical pixels / CSS pixel).
    float mDpiScale = 1.0f;

public:
    /**
     * @brief The parent type is void: Emscripten windows have no native
     * parent widget.
     */
    using ParentType = void;

    static const uint WINDOW_MANAGER_ID = WindowManagerId::EMSCRIPTEN_CANVAS;

    WindowManager(ParentType* parent = nullptr) :
            WindowManager("vclib", 1024, 768, parent)
    {
    }

    WindowManager(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        ParentType*               = nullptr) :
            mTitle(windowTitle), mWidth(width), mHeight(height)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");

        mDpiScale =
            static_cast<float>(emscripten_get_device_pixel_ratio());
    }

    virtual ~WindowManager() = default;

    // -----------------------------------------------------------------------
    // WindowManagerConcept interface
    // -----------------------------------------------------------------------

    const std::string& windowTitle() const { return mTitle; }

    void setWindowTitle(const std::string& title) { mTitle = title; }

    uint width() const
    {
        double w, h;
        emscripten_get_element_css_size(mCanvasTarget.c_str(), &w, &h);
        return static_cast<uint>(w);
    }

    uint height() const
    {
        double w, h;
        emscripten_get_element_css_size(mCanvasTarget.c_str(), &w, &h);
        return static_cast<uint>(h);
    }

    bool isMinimized() const { return false; }

    Point2f dpiScale() const { return Point2f(mDpiScale, mDpiScale); }

    /**
     * @brief Returns the canvas CSS selector as the window handle.
     *
     * bgfx on Emscripten passes this string to
     * @c emscripten_webgl_create_context to locate the canvas element.
     * Returning a non-null, stable pointer is also required so that
     * @ref Context does not treat the rendering context as headless and
     * correctly targets the WebGL default framebuffer.
     */
    void* winId() const
    {
        return static_cast<void*>(const_cast<char*>(mCanvasTarget.c_str()));
    }

    /**
     * @brief Returns nullptr: no display handle is needed on Emscripten.
     */
    void* displayId() const { return nullptr; }

    /**
     * @brief Called by the Canvas when it needs the window to repaint.
     *
     * Nothing to do here because the animation-frame loop managed by show()
     * drives painting independently.
     */
    void update() {}

    /**
     * @brief Starts the render loop.
     *
     * Sets the canvas backing store to physical pixels, calls
     * `DerivedRenderApp::WM::init`, registers all HTML5 input callbacks, then
     * schedules `emscripten_request_animation_frame_loop` and returns
     * immediately.
     */
    void show()
    {
        // Resize the canvas backing store to physical pixels so the GPU
        // framebuffer matches the screen resolution.
        const uint physW = static_cast<uint>(mWidth * mDpiScale);
        const uint physH = static_cast<uint>(mHeight * mDpiScale);
        emscripten_set_canvas_element_size(
            mCanvasTarget.c_str(), physW, physH);

        DerivedRenderApp::WM::init(derived());

        setCallbacks();

        emscripten_request_animation_frame_loop(frameCallback, this);
        // Returns immediately; the browser owns the main loop from here.
    }

private:
    // -----------------------------------------------------------------------
    // Frame callback
    // -----------------------------------------------------------------------

    static EM_BOOL frameCallback(double /*timestamp*/, void* userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        self->checkResize();
        DerivedRenderApp::WM::paint(self->derived());
        return EM_TRUE; // keep the loop running
    }

    /**
     * @brief Polls the canvas CSS size every frame and triggers a resize
     * event when it changes (e.g. the browser window was resized).
     */
    void checkResize()
    {
        double cssW, cssH;
        emscripten_get_element_css_size(mCanvasTarget.c_str(), &cssW, &cssH);
        const auto uw = static_cast<uint>(cssW);
        const auto uh = static_cast<uint>(cssH);

        if (uw != mWidth || uh != mHeight) {
            mWidth  = uw;
            mHeight = uh;
            const uint physW = static_cast<uint>(uw * mDpiScale);
            const uint physH = static_cast<uint>(uh * mDpiScale);
            emscripten_set_canvas_element_size(
                mCanvasTarget.c_str(), physW, physH);
            DerivedRenderApp::WM::resize(derived(), physW, physH);
        }
    }

    // -----------------------------------------------------------------------
    // Callback registration
    // -----------------------------------------------------------------------

    void setCallbacks()
    {
        const char* canvas = mCanvasTarget.c_str();

        emscripten_set_mousedown_callback(
            canvas, this, true, mouseDownCallback);
        emscripten_set_mouseup_callback(canvas, this, true, mouseUpCallback);
        emscripten_set_mousemove_callback(
            canvas, this, true, mouseMoveCallback);
        emscripten_set_dblclick_callback(
            canvas, this, true, mouseDblClickCallback);
        emscripten_set_wheel_callback(canvas, this, true, wheelCallback);

        // Keyboard events are captured on the window so they fire regardless
        // of which element has focus.
        emscripten_set_keydown_callback(
            EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, keyDownCallback);
        emscripten_set_keyup_callback(
            EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, keyUpCallback);
    }

    // -----------------------------------------------------------------------
    // Static HTML5 callbacks
    // -----------------------------------------------------------------------

    static EM_BOOL mouseDownCallback(
        int,
        const EmscriptenMouseEvent* e,
        void*                       userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        auto  btn  = emscripten::fromEmscriptenButton(e->button);
        auto  mods = emscripten::fromEmscriptenModifiers(*e);
        DerivedRenderApp::WM::setModifiers(self->derived(), mods);
        DerivedRenderApp::WM::mousePress(
            self->derived(), btn, e->targetX, e->targetY);
        return EM_TRUE;
    }

    static EM_BOOL mouseUpCallback(
        int,
        const EmscriptenMouseEvent* e,
        void*                       userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        auto  btn  = emscripten::fromEmscriptenButton(e->button);
        auto  mods = emscripten::fromEmscriptenModifiers(*e);
        DerivedRenderApp::WM::setModifiers(self->derived(), mods);
        DerivedRenderApp::WM::mouseRelease(
            self->derived(), btn, e->targetX, e->targetY);
        return EM_TRUE;
    }

    static EM_BOOL mouseMoveCallback(
        int,
        const EmscriptenMouseEvent* e,
        void*                       userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        DerivedRenderApp::WM::mouseMove(
            self->derived(), e->targetX, e->targetY);
        return EM_FALSE; // don't prevent default (allows text selection etc.)
    }

    static EM_BOOL mouseDblClickCallback(
        int,
        const EmscriptenMouseEvent* e,
        void*                       userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        auto  btn  = emscripten::fromEmscriptenButton(e->button);
        auto  mods = emscripten::fromEmscriptenModifiers(*e);
        DerivedRenderApp::WM::setModifiers(self->derived(), mods);
        DerivedRenderApp::WM::mouseDoubleClick(
            self->derived(), btn, e->targetX, e->targetY);
        return EM_TRUE;
    }

    static EM_BOOL wheelCallback(
        int,
        const EmscriptenWheelEvent* e,
        void*                       userData)
    {
        auto* self  = static_cast<WindowManager*>(userData);
        auto  mods  = emscripten::fromEmscriptenModifiers(*e);
        DerivedRenderApp::WM::setModifiers(self->derived(), mods);

        // deltaMode: 0=pixels, 1=lines (~10px each), 2=pages (~100px each)
        double factor = 1.0;
        if (e->deltaMode == DOM_DELTA_LINE)
            factor = 10.0;
        else if (e->deltaMode == DOM_DELTA_PAGE)
            factor = 100.0;

        DerivedRenderApp::WM::mouseScroll(
            self->derived(), -e->deltaX * factor, -e->deltaY * factor);
        return EM_TRUE;
    }

    static EM_BOOL keyDownCallback(
        int,
        const EmscriptenKeyboardEvent* e,
        void*                          userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        auto  mods = emscripten::fromEmscriptenModifiers(*e);
        DerivedRenderApp::WM::setModifiers(self->derived(), mods);
        DerivedRenderApp::WM::keyPress(
            self->derived(), emscripten::fromEmscripten(*e));
        return EM_FALSE; // don't consume: allow browser shortcuts (F5, etc.)
    }

    static EM_BOOL keyUpCallback(
        int,
        const EmscriptenKeyboardEvent* e,
        void*                          userData)
    {
        auto* self = static_cast<WindowManager*>(userData);
        auto  mods = emscripten::fromEmscriptenModifiers(*e);
        DerivedRenderApp::WM::setModifiers(self->derived(), mods);
        DerivedRenderApp::WM::keyRelease(
            self->derived(), emscripten::fromEmscripten(*e));
        return EM_FALSE;
    }

    // -----------------------------------------------------------------------
    // CRTP helpers
    // -----------------------------------------------------------------------

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

} // namespace vcl::emscripten

#endif // VCL_EMSCRIPTEN_WINDOW_MANAGER_H
