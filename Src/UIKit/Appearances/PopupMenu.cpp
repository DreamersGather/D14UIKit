#include "Common/Precompile.h"

#include "UIKit/Appearances/PopupMenu.h"

namespace d14engine::uikit::appearance
{
    void PopupMenu::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.background =
            {
                D2D1::ColorF{ 0xf9f9f9 }, // color
                1.0f // opacity
            };
            light.shadow.color = D2D1::ColorF{ 0x808080 };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.background =
            {
                D2D1::ColorF{ 0x272727 }, // color
                1.0f // opacity
            };
            dark.shadow.color = D2D1::ColorF{ 0x000000 };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(PopupMenu);

    void PopupMenu::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_1(background);
        _D14_UPDATE_THEME_STYLE_DATA_1(shadow.color);
    }
}
