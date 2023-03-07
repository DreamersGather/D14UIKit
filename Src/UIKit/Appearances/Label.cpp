#include "Common/Precompile.h"

#include "UIKit/Appearances/Label.h"

namespace d14engine::uikit::appearance
{
    Label::Appearance::Appearance()
    {
        background.opacity = 0.0f;
        stroke.opacity = 0.0f;
    }

    void Label::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.foreground.color = D2D1::ColorF{ 0x000000 };
            light.secondaryForeground.color = D2D1::ColorF{ 0x9e9e9e };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.foreground.color = D2D1::ColorF{ 0xe5e5e5 };
            dark.secondaryForeground.color = D2D1::ColorF{ 0x777777 };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(Label);

    void Label::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_1(foreground.color);
        _D14_UPDATE_THEME_STYLE_DATA_1(secondaryForeground.color);
    }
}
