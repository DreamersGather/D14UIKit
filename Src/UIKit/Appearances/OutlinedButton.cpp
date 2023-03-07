#include "Common/Precompile.h"

#include "UIKit/Appearances/OutlinedButton.h"

namespace d14engine::uikit::appearance
{
    void OutlinedButton::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.main[(size_t)uikit::Button::State::Disabled] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x9e9e9e }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0xbfbfbf }, // color
                    1.0f // opacity
                }
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.main[(size_t)uikit::Button::State::Disabled] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x777777 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x434343 }, // color
                    1.0f // opacity
                }
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(OutlinedButton);

    void OutlinedButton::Appearance::changeTheme(FlatButton::Appearance& appearance, WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _ref.main[(size_t)uikit::Button::State::Idle] =
        {
            // foreground
            {
                g_colorGroup.primary, // color
                1.0f // opacity
            },
            // background
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            },
            // stroke
            {
                1.0f, // width
                g_colorGroup.primary, // color
                1.0f // opacity
            }
        };
        _ref.main[(size_t)uikit::Button::State::Hover] =
        {
            // foreground
            {
                g_colorGroup.secondary, // color
                1.0f // opacity
            },
            // background
            {
                g_colorGroup.secondary, // color
                0.05f // opacity
            },
            // stroke
            {
                1.0f, // width
                g_colorGroup.secondary, // color
                1.0f // opacity
            }
        };
        _ref.main[(size_t)uikit::Button::State::Down] =
        {
            // foreground
            {
                g_colorGroup.tertiary, // color
                0.65f // opacity
            },
            // background
            {
                g_colorGroup.tertiary, // color
                0.12f // opacity
            },
            // stroke
            {
                1.0f, // width
                g_colorGroup.tertiary, // color
                1.0f // opacity
            }
        };
        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_2(main);
    }
}
