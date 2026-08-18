// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_TRACKBALL_SETTINGS_H
#define VCL_RENDER_SETTINGS_TRACKBALL_SETTINGS_H

#include <vclib/render/concepts/settings.h>
#include <vclib/render/input.h>
#include <vclib/render/viewer/trackball.h>

namespace vcl {

inline std::string toString(TrackballMotionType motion)
{
    switch (motion) {
    case TrackballMotionType::ARC: return "Arcball Rotation";
    case TrackballMotionType::PAN: return "Pan";
    case TrackballMotionType::ZMOVE: return "Z-Move";
    case TrackballMotionType::ROLL: return "Roll";
    case TrackballMotionType::SCALE: return "Scale";
    case TrackballMotionType::FOV: return "Field of View";
    case TrackballMotionType::FOCUS: return "Focus";
    case TrackballMotionType::DIR_LIGHT_ARC: return "Directional Light Arc";
    default: return "Unknown";
    }
}

inline void fromString(const std::string& str, TrackballMotionType& out)
{
    if (str == "Arcball Rotation") out = TrackballMotionType::ARC;
    else if (str == "Pan") out = TrackballMotionType::PAN;
    else if (str == "Z-Move") out = TrackballMotionType::ZMOVE;
    else if (str == "Roll") out = TrackballMotionType::ROLL;
    else if (str == "Scale") out = TrackballMotionType::SCALE;
    else if (str == "Field of View") out = TrackballMotionType::FOV;
    else if (str == "Focus") out = TrackballMotionType::FOCUS;
    else if (str == "Directional Light Arc")
        out = TrackballMotionType::DIR_LIGHT_ARC;
    else out = TrackballMotionType::MOTION_COUNT;
}

template<>
inline std::vector<TrackballMotionType> availableActions()
{
    return {
        TrackballMotionType::ARC,
        TrackballMotionType::PAN,
        TrackballMotionType::ZMOVE,
        TrackballMotionType::ROLL,
        TrackballMotionType::SCALE,
        TrackballMotionType::FOV,
        TrackballMotionType::FOCUS,
        TrackballMotionType::DIR_LIGHT_ARC
    };
}

/**
 * @brief Contains the settings and bindings for the trackball.
 */
struct TrackballSettings
{
    using DragMotionMap = BindingMap<
        std::pair<MouseButton::Enum, KeyModifiers>,
        TrackballMotionType>;
    using ScrollAtomicMap = BindingMap<
        std::pair<ScrollAxis::Enum, KeyModifiers>,
        TrackballMotionType>;
    using KeyAtomicMap =
        BindingMap<std::pair<Key::Enum, KeyModifiers>, std::string>;

    DragMotionMap   dragMotionMap   = defaultDragMotionMap();
    ScrollAtomicMap scrollAtomicMap = defaultScrollMotionMap();
    KeyAtomicMap    keyAtomicMap    = defaultKeyAtomicMap();

    template <typename Visitor>
    void visitInputBindings(Visitor&& visitor, const std::vector<std::string>& availableCallbacks = {})
    {
        visitor("Drag Motions", dragMotionMap);
        visitor("Scroll Actions", scrollAtomicMap);
        visitor("Key Actions", keyAtomicMap, availableCallbacks);
    }


private:
    static DragMotionMap defaultDragMotionMap()
    {
        using enum MouseButton::Enum;
        using enum KeyModifier::Enum;
        using enum TrackballMotionType;

        return DragMotionMap {
            {{LEFT, {NO_MODIFIER}},    ARC          },
            {{LEFT, {CONTROL}},        PAN          },
            {{LEFT, {ALT}},            ZMOVE        },
            {{LEFT, {SHIFT}},          SCALE        },
            {{MIDDLE, {NO_MODIFIER}},  PAN          },
            {{MIDDLE, {CONTROL}},      ROLL         },
            {{LEFT, {SHIFT, CONTROL}}, DIR_LIGHT_ARC},
        };
    }

    static ScrollAtomicMap defaultScrollMotionMap()
    {
        using enum KeyModifier::Enum;
        using enum ScrollAxis::Enum;
        using enum TrackballMotionType;

        return ScrollAtomicMap {
            {{VERTICAL, {NO_MODIFIER}}, SCALE},
            {{VERTICAL, {CONTROL}},     ROLL },
            {{VERTICAL, {SHIFT}},       FOV  },
#ifdef __APPLE__
            {{HORIZONTAL, {SHIFT}},     FOV  },
#endif
        };
    }

    static KeyAtomicMap defaultKeyAtomicMap()
    {
        using enum Key::Enum;
        using enum KeyModifier::Enum;

        return KeyAtomicMap {
            {{R, {NO_MODIFIER}},       "Reset Trackball"        },
            {{R, {CONTROL, SHIFT}},    "Reset Directional Light"},

            // rotate
            {{NP_2, {NO_MODIFIER}},    "Rotate X+"              },
            {{NP_4, {NO_MODIFIER}},    "Rotate Y-"              },
            {{NP_6, {NO_MODIFIER}},    "Rotate Y+"              },
            {{NP_8, {NO_MODIFIER}},    "Rotate X-"              },

            // translate
            {{UP, {NO_MODIFIER}},      "Translate Y+"           },
            {{DOWN, {NO_MODIFIER}},    "Translate Y-"           },
            {{LEFT, {NO_MODIFIER}},    "Translate X-"           },
            {{RIGHT, {NO_MODIFIER}},   "Translate X+"           },

            // set view
            {{NP_1, {NO_MODIFIER}},    "View Front"             },
            {{NP_7, {NO_MODIFIER}},    "View Top"               },
            {{NP_3, {NO_MODIFIER}},    "View Right"             },
            {{NP_1, {CONTROL}},        "View Back"              },
            {{NP_7, {CONTROL}},        "View Bottom"            },
            {{NP_3, {CONTROL}},        "View Left"              },

            // projection mode
            {{NP_5, {NO_MODIFIER}},    "Toggle Projection"      },

            // rotate light
            {{NP_2, {CONTROL, SHIFT}}, "Rotate Light X+"        },
            {{NP_4, {CONTROL, SHIFT}}, "Rotate Light Y-"        },
            {{NP_6, {CONTROL, SHIFT}}, "Rotate Light Y+"        },
            {{NP_8, {CONTROL, SHIFT}}, "Rotate Light X-"        },
        };
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_TRACKBALL_SETTINGS_H
