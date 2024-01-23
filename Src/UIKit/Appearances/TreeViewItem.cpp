#include "Common/Precompile.h"

#include "UIKit/Appearances/TreeViewItem.h"

namespace d14engine::uikit::appearance
{
    void TreeViewItem::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.arrow.background.color = D2D1::ColorF{ 0x000000 };
            light.arrow.secondaryBackground.color = D2D1::ColorF{ 0x9e9e9e };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.arrow.background.color = D2D1::ColorF{ 0xffffff };
            dark.arrow.secondaryBackground.color = D2D1::ColorF{ 0x777777 };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(TreeViewItem);

    void TreeViewItem::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_1(arrow.background.color);
        _D14_UPDATE_THEME_STYLE_DATA_1(arrow.secondaryBackground.color);
    }
}
