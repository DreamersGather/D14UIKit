#include "Common/Precompile.h"

#include "UIKit/Appearances/TabCaption.h"

namespace d14engine::uikit::appearance
{
    void TabCaption::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.closeX.icon.background[(size_t)ButtonState::Idle] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                1.0f // opacity
            };
            light.closeX.icon.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                1.0f // opacity
            };
            light.closeX.icon.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.65f // opacity
            };
            light.closeX.button.background[(size_t)ButtonState::Idle] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            };
            light.closeX.button.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.1f // opacity
            };
            light.closeX.button.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.2f // opacity
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.closeX.icon.background[(size_t)ButtonState::Idle] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                1.0f // opacity
            };
            dark.closeX.icon.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                1.0f // opacity
            };
            dark.closeX.icon.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.55f // opacity
            };
            dark.closeX.button.background[(size_t)ButtonState::Idle] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.0f // opacity
            };
            dark.closeX.button.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.2f // opacity
            };
            dark.closeX.button.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.4f // opacity
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(TabCaption);

    void TabCaption::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(closeX.icon.background);
        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(closeX.button.background);
    }
}
