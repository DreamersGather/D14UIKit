#include "Common/Precompile.h"

#include "UIKit/Appearances/ResizablePanel.h"

namespace d14engine::uikit::appearance
{
    void ResizablePanel::Appearance::initialize()
    {
        g_themeStyles = { { L"Light", {} }, { L"Dark", {} } };
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(ResizablePanel);

    void ResizablePanel::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _ref.staticSizingGuideFrame.background =
        {
            g_colorGroup.primary, // color
            0.5f // opacity
        };
        _D14_UPDATE_THEME_STYLE_DATA_1(staticSizingGuideFrame.background);
    }
}
