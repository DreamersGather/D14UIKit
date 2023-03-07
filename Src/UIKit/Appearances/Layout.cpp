#include "Common/Precompile.h"

#include "UIKit/Appearances/Layout.h"

namespace d14engine::uikit::appearance
{
    Layout::Appearance::Appearance()
    {
        background.opacity = 0.0f;
        stroke.opacity = 0.0f;
    }

    void Layout::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.background.color = D2D1::ColorF{ 0xf3f3f3 };
            light.stroke.color = D2D1::ColorF{ 0xe5e5e5 };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.background.color = D2D1::ColorF{ 0x202020 };
            dark.stroke.color = D2D1::ColorF{ 0x1d1d1d };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(Layout);

    void Layout::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_1(background.color);
        _D14_UPDATE_THEME_STYLE_DATA_1(stroke.color);
    }
}
