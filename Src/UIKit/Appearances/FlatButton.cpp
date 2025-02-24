#include "Common/Precompile.h"

#include "UIKit/Appearances/FlatButton.h"

namespace d14engine::uikit::appearance
{
    void FlatButton::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.main[(size_t)uikit::Button::State::Idle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
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
            light.main[(size_t)uikit::Button::State::Hover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.06f // opacity
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
                    D2D1::ColorF{ 0x000000 }, // color
                    0.65f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.03f // opacity
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
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.main[(size_t)uikit::Button::State::Idle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    1.0f // opacity
                },
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
            dark.main[(size_t)uikit::Button::State::Hover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    0.06f // opacity
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
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    0.55f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    0.03f // opacity
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
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(FlatButton);

    void FlatButton::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _D14_UPDATE_THEME_DATA_ARRAY_1(main);
    }
}
