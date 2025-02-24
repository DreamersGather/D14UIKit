#include "Common/Precompile.h"

#include "UIKit/Appearances/ResizablePanel.h"

namespace d14engine::uikit::appearance
{
    void ResizablePanel::Appearance::initialize()
    {
        g_themeData = { { L"Light", {} }, { L"Dark", {} } };
    }
    _D14_SET_THEME_DATA_MAP_IMPL(ResizablePanel);

    void ResizablePanel::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _ref.staticSizingGuideFrame.background =
        {
            g_colorGroup.primary, // color
            0.5f // opacity
        };
        _D14_UPDATE_THEME_DATA_1(staticSizingGuideFrame.background);
    }
}
