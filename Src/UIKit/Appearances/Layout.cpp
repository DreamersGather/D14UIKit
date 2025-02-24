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
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.background.color = D2D1::ColorF{ 0xf3f3f3 };
            light.stroke.color = D2D1::ColorF{ 0xe5e5e5 };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.background.color = D2D1::ColorF{ 0x202020 };
            dark.stroke.color = D2D1::ColorF{ 0x1d1d1d };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(Layout);

    void Layout::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _D14_UPDATE_THEME_DATA_1(background.color);
        _D14_UPDATE_THEME_DATA_1(stroke.color);
    }
}
