#include "Common/Precompile.h"

#include "UIKit/Appearances/TextInput.h"

namespace d14engine::uikit::appearance
{
    void TextInput::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.main[(size_t)State::Idle] =
            {
                // background
                {
                    D2D1::ColorF{ 0xfdfdfd }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.5f, // width
                    D2D1::ColorF{ 0xc2c2c2 }, // color
                    1.0f // opacity
                }
            };
            light.main[(size_t)State::Hover] =
            {
                // background
                {
                    D2D1::ColorF{ 0xf9f9f9 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0xc2c2c2 }, // color
                    1.0f // opacity
                }
            };
            light.main[(size_t)State::Active] =
            {
                // background
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0xc2c2c2 }, // color
                    1.0f // opacity
                }
            };
            light.main[(size_t)State::Disabled] =
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
            light.bottomLine.background.Static[(size_t)State::Idle] =
            {
                D2D1::ColorF{ 0x8f8f8f }, // color
                1.0f // opacity
            };
            light.bottomLine.background.Static[(size_t)State::Hover] =
            {
                D2D1::ColorF{ 0xa8a8a8 }, // color
                1.0f // opacity
            };
            light.bottomLine.background.Static[(size_t)State::Active] =
            {
                D2D1::ColorF{ 0xa8a8a8 }, // color
                1.0f // opacity
            };
            light.bottomLine.background.Static[(size_t)State::Disabled] =
            {
                D2D1::ColorF{ 0xbfbfbf }, // color
                1.0f // opacity
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.main[(size_t)State::Idle] =
            {
                // background
                {
                    D2D1::ColorF{ 0x343434 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.5f, // width
                    D2D1::ColorF{ 0x3d3d3d }, // color
                    1.0f // opacity
                }
            };
            dark.main[(size_t)State::Hover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x393939 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x3d3d3d }, // color
                    1.0f // opacity
                }
            };
            dark.main[(size_t)State::Active] =
            {
                // background
                {
                    D2D1::ColorF{ 0x1d1d1d }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x3d3d3d }, // color
                    1.0f // opacity
                }
            };
            dark.main[(size_t)State::Disabled] =
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
            dark.bottomLine.background.Static[(size_t)State::Idle] =
            {
                D2D1::ColorF{ 0x5c5c5c }, // color
                1.0f // opacity
            };
            dark.bottomLine.background.Static[(size_t)State::Hover] =
            {
                D2D1::ColorF{ 0x4c4c4c }, // color
                1.0f // opacity
            };
            dark.bottomLine.background.Static[(size_t)State::Active] =
            {
                D2D1::ColorF{ 0x4c4c4c }, // color
                1.0f // opacity
            };
            dark.bottomLine.background.Static[(size_t)State::Disabled] =
            {
                D2D1::ColorF{ 0x434343 }, // color
                1.0f // opacity
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(TextInput);

    void TextInput::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _ref.bottomLine.background.Dynamic =
        {
            g_colorGroup.primary, // color
            1.0f // opacity
        };
        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(main);

        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(bottomLine.background.Static);
        _D14_UPDATE_THEME_STYLE_DATA_1(bottomLine.background.Dynamic);
    }
}
