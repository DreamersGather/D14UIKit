#include "Common/Precompile.h"

#include "UIKit/Appearances/ElevatedButton.h"

namespace d14engine::uikit::appearance
{
    void ElevatedButton::Appearance::initialize()
    {
        auto& light = (g_themeStyles[L"Light"] = {});
        {
            light.shadow.color[(size_t)uikit::Button::State::Idle] =
            light.shadow.color[(size_t)uikit::Button::State::Hover] = D2D1::ColorF
            {
                0x808080, // rgb
                1.0f // alpha
            };
            light.shadow.color[(size_t)uikit::Button::State::Down] =
            light.shadow.color[(size_t)uikit::Button::State::Disabled] = D2D1::ColorF
            {
                0x000000, // rgb
                0.0f // alpha
            };
        }
        auto& dark = (g_themeStyles[L"Dark"] = {});
        {
            dark.shadow.color[(size_t)uikit::Button::State::Idle] =
            dark.shadow.color[(size_t)uikit::Button::State::Hover] = D2D1::ColorF
            {
                0x000000, // rgb
                1.0f // alpha
            };
            dark.shadow.color[(size_t)uikit::Button::State::Down] =
            dark.shadow.color[(size_t)uikit::Button::State::Disabled] = D2D1::ColorF
            {
                0x000000, // rgb
                0.0f // alpha
            };
        }
    }
    _D14_SET_THEME_STYLE_MAP_IMPL(ElevatedButton);

    void ElevatedButton::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_STYLE(themeName);

        _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(shadow.color);
    }
}
