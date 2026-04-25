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

#ifndef VCL_EMSCRIPTEN_INPUT_H
#define VCL_EMSCRIPTEN_INPUT_H

#include <vclib/render/input.h>

#include <emscripten/html5.h>

namespace vcl::emscripten {

/**
 * @brief Converts an HTML5 mouse button index to a vcl::MouseButton::Enum.
 *
 * HTML5 button values: 0=left, 1=middle, 2=right.
 */
vcl::MouseButton::Enum fromEmscriptenButton(unsigned short button);

/**
 * @brief Extracts a vcl::Key from an Emscripten keyboard event.
 *
 * Uses the event's `key` string (e.g. "a", "ArrowLeft", "F1").
 */
vcl::Key::Enum fromEmscripten(const EmscriptenKeyboardEvent& event);

/**
 * @brief Extracts the active vcl::KeyModifiers from an Emscripten mouse event.
 */
vcl::KeyModifiers fromEmscriptenModifiers(const EmscriptenMouseEvent& event);

/**
 * @brief Extracts the active vcl::KeyModifiers from an Emscripten keyboard
 * event.
 */
vcl::KeyModifiers fromEmscriptenModifiers(const EmscriptenKeyboardEvent& event);

/**
 * @brief Extracts the active vcl::KeyModifiers from an Emscripten wheel event.
 */
vcl::KeyModifiers fromEmscriptenModifiers(const EmscriptenWheelEvent& event);

} // namespace vcl::emscripten

#endif // VCL_EMSCRIPTEN_INPUT_H
