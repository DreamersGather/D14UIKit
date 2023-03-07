#include "Common/Precompile.h"

#include "UIKit/Appearances/MenuSeparator.h"

namespace d14engine::uikit::appearance
{
    void MenuSeparator::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.background =
            {
                D2D1::ColorF{ 0xe5e5e5 }, // color
                1.0f // opacity
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.background =
            {
                D2D1::ColorF{ 0x3d3d3d }, // color
                1.0f // opacity
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(MenuSeparator);

    void MenuSeparator::Appearance::changeTheme(ViewItem::Appearance& appearance, WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_1(background);
    }
}
