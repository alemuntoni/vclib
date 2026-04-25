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

#include <vclib/emscripten/input.h>

#include <cstring>

namespace vcl::emscripten {

vcl::MouseButton::Enum fromEmscriptenButton(unsigned short button)
{
    switch (button) {
    case 0: return vcl::MouseButton::LEFT;
    case 1: return vcl::MouseButton::MIDDLE;
    case 2: return vcl::MouseButton::RIGHT;
    default: return vcl::MouseButton::NO_BUTTON;
    }
}

vcl::Key::Enum fromEmscripten(const EmscriptenKeyboardEvent& event)
{
    const char* key = event.key;

    // Single-character keys: letters and digits map directly to vcl::Key
    // which uses the same values as ASCII uppercase letters and digit chars.
    if (key[0] != '\0' && key[1] == '\0') {
        const char c = key[0];
        if (c >= 'a' && c <= 'z')
            return static_cast<vcl::Key::Enum>(c - ('a' - 'A')); // to upper
        if (c >= 'A' && c <= 'Z')
            return static_cast<vcl::Key::Enum>(c);
        if (c >= '0' && c <= '9')
            return static_cast<vcl::Key::Enum>(c);
        if (c == ' ')
            return vcl::Key::SPACE;
    }

    // Multi-character keys
    if (strcmp(key, "ArrowLeft") == 0) return vcl::Key::LEFT;
    if (strcmp(key, "ArrowRight") == 0) return vcl::Key::RIGHT;
    if (strcmp(key, "ArrowUp") == 0) return vcl::Key::UP;
    if (strcmp(key, "ArrowDown") == 0) return vcl::Key::DOWN;

    if (strcmp(key, "Shift") == 0) return vcl::Key::SHIFT;
    if (strcmp(key, "Control") == 0) return vcl::Key::CONTROL;
    if (strcmp(key, "Alt") == 0) return vcl::Key::ALT;
    if (strcmp(key, "Meta") == 0) return vcl::Key::SUPER;

    if (strcmp(key, "CapsLock") == 0) return vcl::Key::CAPS_LOCK;
    if (strcmp(key, "NumLock") == 0) return vcl::Key::NUM_LOCK;
    if (strcmp(key, "ScrollLock") == 0) return vcl::Key::SCROLL_LOCK;

    if (strcmp(key, "F1") == 0) return vcl::Key::F1;
    if (strcmp(key, "F2") == 0) return vcl::Key::F2;
    if (strcmp(key, "F3") == 0) return vcl::Key::F3;
    if (strcmp(key, "F4") == 0) return vcl::Key::F4;
    if (strcmp(key, "F5") == 0) return vcl::Key::F5;
    if (strcmp(key, "F6") == 0) return vcl::Key::F6;
    if (strcmp(key, "F7") == 0) return vcl::Key::F7;
    if (strcmp(key, "F8") == 0) return vcl::Key::F8;
    if (strcmp(key, "F9") == 0) return vcl::Key::F9;
    if (strcmp(key, "F10") == 0) return vcl::Key::F10;
    if (strcmp(key, "F11") == 0) return vcl::Key::F11;
    if (strcmp(key, "F12") == 0) return vcl::Key::F12;

    return vcl::Key::UNKNOWN;
}

// Internal helper that builds a KeyModifiers bitset from individual flags.
static vcl::KeyModifiers modifiersFromFlags(
    bool shift,
    bool ctrl,
    bool alt,
    bool meta)
{
    vcl::KeyModifiers mods;
    if (shift) mods[vcl::KeyModifier::SHIFT]   = true;
    if (ctrl)  mods[vcl::KeyModifier::CONTROL] = true;
    if (alt)   mods[vcl::KeyModifier::ALT]     = true;
    if (meta)  mods[vcl::KeyModifier::SUPER]   = true;
    // NO_MODIFIER must be set when no real modifier is active.
    if (mods.none()) mods[vcl::KeyModifier::NO_MODIFIER] = true;
    return mods;
}

vcl::KeyModifiers fromEmscriptenModifiers(const EmscriptenMouseEvent& event)
{
    return modifiersFromFlags(
        event.shiftKey, event.ctrlKey, event.altKey, event.metaKey);
}

vcl::KeyModifiers fromEmscriptenModifiers(const EmscriptenKeyboardEvent& event)
{
    return modifiersFromFlags(
        event.shiftKey, event.ctrlKey, event.altKey, event.metaKey);
}

vcl::KeyModifiers fromEmscriptenModifiers(const EmscriptenWheelEvent& event)
{
    return modifiersFromFlags(
        event.mouse.shiftKey,
        event.mouse.ctrlKey,
        event.mouse.altKey,
        event.mouse.metaKey);
}

} // namespace vcl::emscripten
