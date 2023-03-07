#include "Common/Precompile.h"

#include "UIKit/Appearances/Button.h"

namespace d14engine::uikit::appearance
{
    Button::Appearance::Appearance()
    {
        background.opacity = 0.0f;
        stroke.opacity = 0.0f;
    }

    void Button::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.foreground.color = D2D1::ColorF{ 0x000000 };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.foreground.color = D2D1::ColorF{ 0xe5e5e5 };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(Button);

    void Button::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_1(foreground.color);
    }
}
