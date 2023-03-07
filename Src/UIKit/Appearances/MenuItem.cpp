#include "Common/Precompile.h"

#include "UIKit/Appearances/MenuItem.h"

namespace d14engine::uikit::appearance
{
    void MenuItem::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.main[(size_t)ViewItem::State::Idle] =
            {
                // background
                {
                    D2D1::ColorF{ 0xf9f9f9 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)ViewItem::State::Hover] =
            {
                // background
                {
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)ViewItem::State::ActiveSelected] =
            light.main[(size_t)ViewItem::State::InactiveSelected] =
            light.main[(size_t)ViewItem::State::ActiveSelectedHover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.arrow.background =
            {
                D2D1::ColorF{ 0x000000 }, // color
                1.0f // opacity
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.main[(size_t)ViewItem::State::Idle] =
            {
                // background
                {
                    D2D1::ColorF{ 0x272727 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)ViewItem::State::Hover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x3d3d3d }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)ViewItem::State::ActiveSelected] =
            dark.main[(size_t)ViewItem::State::InactiveSelected] =
            dark.main[(size_t)ViewItem::State::ActiveSelectedHover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.arrow.background =
            {
                D2D1::ColorF{ 0xffffff }, // color
                1.0f // opacity
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(MenuItem);

    void MenuItem::Appearance::changeTheme(ViewItem::Appearance& appearance, WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_2(main);
        _D14_UPDATE_THEME_STYLE_DATA_1(arrow.background);
    }
}
