#include "Common/Precompile.h"

#include "UIKit/Appearances/LabelArea.h"

namespace d14engine::uikit::appearance
{
    LabelArea::Appearance::Appearance()
    {
        hiliteRange.stroke.opacity = 0.0f;
    }

    void LabelArea::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.hiliteRange.background.color = D2D1::ColorF{ 0xadd6ff };
            light.indicator.background.color = D2D1::ColorF{ 0x000000 };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.hiliteRange.background.color = D2D1::ColorF{ 0x264f78 };
            dark.indicator.background.color = D2D1::ColorF{ 0xffffff };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(LabelArea);

    void LabelArea::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _D14_UPDATE_THEME_DATA_1(hiliteRange.background.color);
        _D14_UPDATE_THEME_DATA_1(indicator.background.color);
    }
}
