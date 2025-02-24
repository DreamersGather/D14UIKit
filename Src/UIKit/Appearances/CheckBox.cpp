#include "Common/Precompile.h"

#include "UIKit/Appearances/CheckBox.h"

namespace d14engine::uikit::appearance
{
    void CheckBox::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.icon.background[(size_t)CheckBoxState::Flag::UncheckedIdle] =
            light.icon.background[(size_t)CheckBoxState::Flag::UncheckedHover] =
            light.icon.background[(size_t)CheckBoxState::Flag::UncheckedDown] =
            light.icon.background[(size_t)CheckBoxState::Flag::UncheckedDisabled] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            };
            light.icon.background[(size_t)CheckBoxState::Flag::IntermediateIdle] =
            light.icon.background[(size_t)CheckBoxState::Flag::CheckedIdle] =
            light.icon.background[(size_t)CheckBoxState::Flag::IntermediateHover] =
            light.icon.background[(size_t)CheckBoxState::Flag::CheckedHover] =
            light.icon.background[(size_t)CheckBoxState::Flag::IntermediateDown] =
            light.icon.background[(size_t)CheckBoxState::Flag::CheckedDown] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                1.0f // opacity
            };
            light.icon.background[(size_t)CheckBoxState::Flag::IntermediateDisabled] =
            light.icon.background[(size_t)CheckBoxState::Flag::CheckedDisabled] =
            {
                D2D1::ColorF{ 0xe5e5e5 }, // color
                1.0f // opacity
            };
            light.button[(size_t)CheckBoxState::Flag::UncheckedIdle] =
            {
                // background
                {
                    D2D1::ColorF{ 0xf3f3f3 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.5f, // width
                    D2D1::ColorF{ 0x8f8f8f }, // color
                    1.0f // opacity
                }
            };
            light.button[(size_t)CheckBoxState::Flag::UncheckedHover] =
            {
                // background
                {
                    D2D1::ColorF{ 0xeaeaea }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x8f8f8f }, // color
                    1.0f // opacity
                }
            };
            light.button[(size_t)CheckBoxState::Flag::UncheckedDown] =
            {
                // background
                {
                    D2D1::ColorF{ 0xe2e2e2 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0xbcbcbc }, // color
                    1.0f // opacity
                }
            };
            light.button[(size_t)CheckBoxState::Flag::UncheckedDisabled] =
            {
                // background
                {
                    D2D1::ColorF{ 0xf3f3f3 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.5f, // width
                    D2D1::ColorF{ 0xbfbfbf }, // color
                    1.0f // opacity
                }
            };
            light.button[(size_t)CheckBoxState::Flag::IntermediateDisabled] =
            light.button[(size_t)CheckBoxState::Flag::CheckedDisabled] =
            {
                // background
                {
                    D2D1::ColorF{ 0xbfbfbf }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.icon.background[(size_t)CheckBoxState::Flag::UncheckedIdle] =
            dark.icon.background[(size_t)CheckBoxState::Flag::UncheckedHover] =
            dark.icon.background[(size_t)CheckBoxState::Flag::UncheckedDown] =
            dark.icon.background[(size_t)CheckBoxState::Flag::UncheckedDisabled] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            };
            dark.icon.background[(size_t)CheckBoxState::Flag::IntermediateIdle] =
            dark.icon.background[(size_t)CheckBoxState::Flag::CheckedIdle] =
            dark.icon.background[(size_t)CheckBoxState::Flag::IntermediateHover] =
            dark.icon.background[(size_t)CheckBoxState::Flag::CheckedHover] =
            dark.icon.background[(size_t)CheckBoxState::Flag::IntermediateDown] =
            dark.icon.background[(size_t)CheckBoxState::Flag::CheckedDown] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                1.0f // opacity
            };
            dark.icon.background[(size_t)CheckBoxState::Flag::IntermediateDisabled] =
            dark.icon.background[(size_t)CheckBoxState::Flag::CheckedDisabled] =
            {
                D2D1::ColorF{ 0xa8a8a8 }, // color
                1.0f // opacity
            };
            dark.button[(size_t)CheckBoxState::Flag::UncheckedIdle] =
            {
                // background
                {
                    D2D1::ColorF{ 0x232323 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.5f, // width
                    D2D1::ColorF{ 0x7a7a7a }, // color
                    1.0f // opacity
                }
            };
            dark.button[(size_t)CheckBoxState::Flag::UncheckedHover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x303030 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x7a7a7a }, // color
                    1.0f // opacity
                }
            };
            dark.button[(size_t)CheckBoxState::Flag::UncheckedDown] =
            {
                // background
                {
                    D2D1::ColorF{ 0x363636 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x4c4c4c }, // color
                    1.0f // opacity
                }
            };
            dark.button[(size_t)CheckBoxState::Flag::UncheckedDisabled] =
            {
                // background
                {
                    D2D1::ColorF{ 0x202020 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.5f, // width
                    D2D1::ColorF{ 0x434343 }, // color
                    1.0f // opacity
                }
            };
            dark.button[(size_t)CheckBoxState::Flag::IntermediateDisabled] =
            dark.button[(size_t)CheckBoxState::Flag::CheckedDisabled] =
            {
                // background
                {
                    D2D1::ColorF{ 0x434343 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(CheckBox);

    void CheckBox::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _ref.button[(size_t)CheckBoxState::Flag::IntermediateIdle] =
        _ref.button[(size_t)CheckBoxState::Flag::CheckedIdle] =
        {
            // background
            {
                g_colorGroup.primary, // color
                1.0f // opacity
            },
            // stroke
            {
                0.0f, // width
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            }
        };
        _ref.button[(size_t)CheckBoxState::Flag::IntermediateHover] =
        _ref.button[(size_t)CheckBoxState::Flag::CheckedHover] =
        {
            // background
            {
                g_colorGroup.secondary, // color
                1.0f // opacity
            },
            // stroke
            {
                0.0f, // width
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            }
        };
        _ref.button[(size_t)CheckBoxState::Flag::IntermediateDown] =
        _ref.button[(size_t)CheckBoxState::Flag::CheckedDown] =
        {
            // background
            {
                g_colorGroup.tertiary, // color
                1.0f // opacity
            },
            // stroke
            {
                0.0f, // width
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            }
        };
        _D14_UPDATE_THEME_DATA_ARRAY_1(icon.background);
        _D14_UPDATE_THEME_DATA_ARRAY_1(button);
    }
}
