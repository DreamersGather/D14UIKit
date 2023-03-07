#include "Common/Precompile.h"

#include "UIKit/Appearances/OnOffSwitch.h"

namespace d14engine::uikit::appearance
{
    OnOffSwitch::Appearance::Appearance()
    {
        handle.geometry[(size_t)OnOffSwitchState::Flag::OnIdle] =
        handle.geometry[(size_t)OnOffSwitchState::Flag::OnDisabled] =
        {
            { 16.0f, 16.0f }, // size
            8.0f, // round radius

            std::nullopt, // left offset
            4.0f // right offset
        };
        handle.geometry[(size_t)OnOffSwitchState::Flag::OnHover] =
        {
            { 18.0f, 18.0f }, // size
            9.0f, // round radius

            std::nullopt, // left offset
            4.0f // right offset
        };
        handle.geometry[(size_t)OnOffSwitchState::Flag::OnDown] =
        {
            { 20.0f, 18.0f }, // size
            9.0f, // round radius

            std::nullopt, // left offset
            4.0f // right offset
        };
        handle.geometry[(size_t)OnOffSwitchState::Flag::OffIdle] =
        handle.geometry[(size_t)OnOffSwitchState::Flag::OffDisabled] =
        {
            { 16.0f, 16.0f }, // size
            8.0f, // round radius

            4.0f, // left offset
            std::nullopt // right offset
        };
        handle.geometry[(size_t)OnOffSwitchState::Flag::OffHover] =
        {
            { 18.0f, 18.0f }, // size
            9.0f, // round radius

            4.0f, // left offset
            std::nullopt // right offset
        };
        handle.geometry[(size_t)OnOffSwitchState::Flag::OffDown] =
        {
            { 20.0f, 18.0f }, // size
            9.0f, // round radius

            4.0f, // left offset
            std::nullopt // right offset
        };
    }

    void OnOffSwitch::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.main[(size_t)OnOffSwitchState::Flag::OnDisabled] =
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
            light.main[(size_t)OnOffSwitchState::Flag::OffIdle] =
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
            light.main[(size_t)OnOffSwitchState::Flag::OffHover] =
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
            light.main[(size_t)OnOffSwitchState::Flag::OffDown] =
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
            light.main[(size_t)OnOffSwitchState::Flag::OffDisabled] =
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
            light.handle.background[(size_t)OnOffSwitchState::Flag::OnIdle] =
            light.handle.background[(size_t)OnOffSwitchState::Flag::OnHover] =
            light.handle.background[(size_t)OnOffSwitchState::Flag::OnDown] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                1.0f // opacity
            };
            light.handle.background[(size_t)OnOffSwitchState::Flag::OnDisabled] =
            {
                D2D1::ColorF{ 0xe5e5e5 }, // color
                1.0f // opacity
            };
            light.handle.background[(size_t)OnOffSwitchState::Flag::OffIdle] =
            {
                D2D1::ColorF{ 0x5c5c5c }, // color
                1.0f // opacity
            };
            light.handle.background[(size_t)OnOffSwitchState::Flag::OffHover] =
            {
                D2D1::ColorF{ 0x595959 }, // color
                1.0f // opacity
            };
            light.handle.background[(size_t)OnOffSwitchState::Flag::OffDown] =
            {
                D2D1::ColorF{ 0x565656 }, // color
                1.0f // opacity
            };
            light.handle.background[(size_t)OnOffSwitchState::Flag::OffDisabled] =
            {
                D2D1::ColorF{ 0xbfbfbf }, // color
                1.0f // opacity
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.main[(size_t)OnOffSwitchState::Flag::OnDisabled] =
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
            dark.main[(size_t)OnOffSwitchState::Flag::OffIdle] =
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
            dark.main[(size_t)OnOffSwitchState::Flag::OffHover] =
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
            dark.main[(size_t)OnOffSwitchState::Flag::OffDown] =
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
            dark.main[(size_t)OnOffSwitchState::Flag::OffDisabled] =
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
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OnIdle] =
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OnHover] =
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OnDown] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                1.0f // opacity
            };
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OnDisabled] =
            {
                D2D1::ColorF{ 0xa8a8a8 }, // color
                1.0f // opacity
            };
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OffIdle] =
            {
                D2D1::ColorF{ 0xcdcdcd }, // color
                1.0f // opacity
            };
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OffHover] =
            {
                D2D1::ColorF{ 0xd0d0d0 }, // color
                1.0f // opacity
            };
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OffDown] =
            {
                D2D1::ColorF{ 0xd1d1d1 }, // color
                1.0f // opacity
            };
            dark.handle.background[(size_t)OnOffSwitchState::Flag::OffDisabled] =
            {
                D2D1::ColorF{ 0x434343 }, // color
                1.0f // opacity
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(OnOffSwitch);

    void OnOffSwitch::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _ref.main[(size_t)OnOffSwitchState::Flag::OnIdle] =
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
        _ref.main[(size_t)OnOffSwitchState::Flag::OnHover] =
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
        _ref.main[(size_t)OnOffSwitchState::Flag::OnDown] =
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
        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(main);
        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(handle.background);
    }
}
