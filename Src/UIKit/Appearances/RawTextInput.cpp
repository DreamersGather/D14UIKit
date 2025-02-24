#include "Common/Precompile.h"

#include "UIKit/Appearances/RawTextInput.h"

namespace d14engine::uikit::appearance
{
    void RawTextInput::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.background.color = D2D1::ColorF{ 0xfdfdfd };
            light.stroke.color = D2D1::ColorF{ 0xe5e5e5 };

            light.bottomLine.background =
            {
                D2D1::ColorF{ 0x8f8f8f }, // color
                1.0f // opacity
            };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.background.color = D2D1::ColorF{ 0x343434 };
            dark.stroke.color = D2D1::ColorF{ 0x1d1d1d };

            dark.bottomLine.background =
            {
                D2D1::ColorF{ 0x5c5c5c }, // color
                1.0f // opacity
            };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(RawTextInput);

    void RawTextInput::Appearance::changeTheme(Label::Appearance& appearance, WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _D14_UPDATE_THEME_DATA_2(background);
        _D14_UPDATE_THEME_DATA_2(stroke);
        _D14_UPDATE_THEME_DATA_1(bottomLine.background);
    }
}
