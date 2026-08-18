// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CONCEPTS_SETTINGS_H
#define VCL_RENDER_CONCEPTS_SETTINGS_H

#include <vclib/base.h>

#include <nlohmann/json.hpp>

namespace vcl {

template<typename T>
inline std::vector<T> availableActions()
{
    return std::vector<T>();
}

/**
 * @brief Concept that checks if a type T provides settings serialization
 * methods.
 *
 * A type T satisfies `HasSettings` if it provides both `loadSettings` and
 * `saveSettings` methods that take `nlohmann::json` objects.
 */
template<typename T>
concept HasSettings =
    requires (T t, nlohmann::json& j, const nlohmann::json& cj) {
        { t.loadSettings(cj) };
        { t.saveSettings(j) };
    };

template<typename T>
concept ActionConcept =
    Stringifiable<T> && requires (T obj, const std::string& str) {
        requires (requires { { fromString<T>(str) } -> std::convertible_to<T>; } || 
                  requires { fromString(str, obj); });
        { availableActions<T>() } -> std::convertible_to<std::vector<T>>;
    };

namespace detail {
    constexpr auto dummyVisitor = [](auto&&...) {};
    using DummyVisitorType = decltype(dummyVisitor);
}

/**
 * @brief Concept that checks if a type T exposes input bindings
 * through a visitInputBindings method.
 */
template<typename T>
concept HasInputBindings =
    requires (T s, detail::DummyVisitorType v) {
        { s.visitInputBindings(v) };
    } ||
    requires (T s, detail::DummyVisitorType v) {
        { s.visitInputBindings(v, std::vector<std::string>{}) };
    };

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_SETTINGS_H
