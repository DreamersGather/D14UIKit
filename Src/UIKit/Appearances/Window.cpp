#include "Common/Precompile.h"

#include "UIKit/Appearances/Window.h"

namespace d14engine::uikit::appearance
{
    void Window::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.background.color = D2D1::ColorF{ 0xf9f9f9 };
            light.shadow.color = D2D1::ColorF{ 0x808080 };

            light.captionPanel.background =
            {
                D2D1::ColorF{ 0xf3f3f3 } // color
            };
            light.threeBrothers[(size_t)ThreeBrosState::Idle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.8f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            light.threeBrothers[(size_t)ThreeBrosState::Hover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.8f // opacity
                }
            };
            light.threeBrothers[(size_t)ThreeBrosState::Down] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    0.65f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.65f // opacity
                }
            };
            light.threeBrothers[(size_t)ThreeBrosState::CloseIdle] =
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
                }
            };
            light.threeBrothers[(size_t)ThreeBrosState::CloseHover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0xe81123 }, // color
                    0.8f // opacity
                }
            };
            light.threeBrothers[(size_t)ThreeBrosState::CloseDown] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.65f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0xf1707a }, // color
                    0.65f // opacity
                }
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.background.color = D2D1::ColorF{ 0x272727 };
            dark.shadow.color = D2D1::ColorF{ 0x000000 };

            dark.captionPanel.background =
            {
                D2D1::ColorF{ 0x202020 } // color
            };
            dark.threeBrothers[(size_t)ThreeBrosState::Idle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    0.8f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.threeBrothers[(size_t)ThreeBrosState::Hover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xe5e5e5 }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x666666 }, // color
                    0.8f // opacity
                }
            };
            dark.threeBrothers[(size_t)ThreeBrosState::Down] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    0.55f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x666666 }, // color
                    0.55f // opacity
                }
            };
            dark.threeBrothers[(size_t)ThreeBrosState::CloseIdle] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.0f // opacity
                }
            };
            dark.threeBrothers[(size_t)ThreeBrosState::CloseHover] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0xffffff }, // color
                    1.0f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0xe81123 }, // color
                    0.8f // opacity
                }
            };
            dark.threeBrothers[(size_t)ThreeBrosState::CloseDown] =
            {
                // foreground
                {
                    D2D1::ColorF{ 0x000000 }, // color
                    0.55f // opacity
                },
                // background
                {
                    D2D1::ColorF{ 0xf1707a }, // color
                    0.55f // opacity
                }
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(Window);

    void Window::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _ref.decorativeBar.gradientColor =
        {
            g_colorGroup.tertiary,// _0_0
            g_colorGroup.primary, // _0_5
            g_colorGroup.tertiary // _1_0
        };
        _D14_UPDATE_THEME_STYLE_DATA_1(background.color);
        _D14_UPDATE_THEME_STYLE_DATA_1(shadow.color);

        _D14_UPDATE_THEME_STYLE_DATA_1(captionPanel.background.color);

        _D14_UPDATE_THEME_STYLE_DATA_1(decorativeBar.gradientColor._0_0);
        _D14_UPDATE_THEME_STYLE_DATA_1(decorativeBar.gradientColor._0_5);
        _D14_UPDATE_THEME_STYLE_DATA_1(decorativeBar.gradientColor._1_0);

        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(threeBrothers);
    }
}
