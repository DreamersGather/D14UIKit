#include "Common/Precompile.h"

#include "UIKit/Appearances/ViewItem.h"

namespace d14engine::uikit::appearance
{
    void ViewItem::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.main[(size_t)State::Idle] =
            {
                // background
                {
                    D2D1::ColorF{ 0xf3f3f3 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)State::Hover] =
            {
                // background
                {
                    D2D1::ColorF{ 0xebebeb }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)State::ActiveSelected] =
            {
                // background
                {
                    D2D1::ColorF{ 0xe1e1e1 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)State::InactiveSelected] =
            {
                // background
                {
                    D2D1::ColorF{ 0xebebeb }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.main[(size_t)State::ActiveSelectedHover] =
            {
                // background
                {
                    D2D1::ColorF{ 0xe1e1e1 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x808080 }, // color
                    1.0f // opacity
                }
            };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.main[(size_t)State::Idle] =
            {
                // background
                {
                    D2D1::ColorF{ 0x202020 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)State::Hover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x2d2d2d }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)State::ActiveSelected] =
            {
                // background
                {
                    D2D1::ColorF{ 0x323232 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)State::InactiveSelected] =
            {
                // background
                {
                    D2D1::ColorF{ 0x2d2d2d }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    0.0f, // width
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.main[(size_t)State::ActiveSelectedHover] =
            {
                // background
                {
                    D2D1::ColorF{ 0x323232 }, // color
                    1.0f // opacity
                },
                // stroke
                {
                    1.0f, // width
                    D2D1::ColorF{ 0x707070 }, // color
                    1.0f // opacity
                }
            };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(ViewItem);

    void ViewItem::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _D14_UPDATE_THEME_DATA_ARRAY_1(main);
    }
}
