#include "Common/Precompile.h"

#include "UIKit/Appearances/ToggleButton.h"

namespace d14engine::uikit::appearance
{
    void ToggleButton::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.main[(size_t)uikit::Button::State::Idle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)uikit::Button::State::Hover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)uikit::Button::State::Down] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    0.65f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)uikit::Button::State::Disabled] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    1.0f // opacity
                },
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
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.main[(size_t)uikit::Button::State::Idle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)uikit::Button::State::Hover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)uikit::Button::State::Down] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.55f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)uikit::Button::State::Disabled] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xa8a8a8 }, // color
                    1.0f // opacity
                },
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
    _D14_SET_THEME_STYLE_MAP_IMPL(ToggleButton);

    void ToggleButton::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

#define UPDATE_MAIN_BKGN_COLOR(Btn_State, Btn_Color) \
_ref.main[(size_t)uikit::Button::State::Btn_State].background.color = g_colorGroup.Btn_Color

        UPDATE_MAIN_BKGN_COLOR(Idle, primary);
        UPDATE_MAIN_BKGN_COLOR(Hover, secondary);
        UPDATE_MAIN_BKGN_COLOR(Down, tertiary);

#undef MAIN_BKGN_COLOR
        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(main);
    }
}
