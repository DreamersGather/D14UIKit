#include "Common/Precompile.h"

#include "UIKit/Appearances/TabGroup.h"

namespace d14engine::uikit::appearance
{
    TabGroup::Appearance::Appearance()
    {
        tabBar.card.main[(size_t)CardState::Active].geometry =
        {
            { 136.0f, 32.0f }, // size
            8.0f // round radius
        };
        tabBar.card.main[(size_t)CardState::Hover].geometry =
        tabBar.card.main[(size_t)CardState::Dormant].geometry =
        {
            { 120.0f, 24.0f }, // size
            8.0f // round radius
        };
    }

    void TabGroup::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.background.color = D2D1::ColorF{ 0xf3f3f3 };
            light.stroke.color = D2D1::ColorF{ 0xe5e5e5 };

            light.tabBar.card.main[(size_t)CardState::Active].background =
            {
                D2D1::ColorF{ 0xf3f3f3 }, // color
                1.0f // opacity
            };
            light.tabBar.card.main[(size_t)CardState::Hover].background =
            {
                D2D1::ColorF{ 0xd9d9d9 }, // color
                1.0f // opacity
            };
            light.tabBar.card.main[(size_t)CardState::Dormant].background =
            {
                D2D1::ColorF{ 0xe5e5e5 }, // color
                1.0f // opacity
            };
            light.tabBar.card.activeShadowColor = D2D1::ColorF{ 0x8c8c8c };

            light.tabBar.moreCards.control.button.background[(size_t)ButtonState::Idle] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.0f // opacity
            };
            light.tabBar.moreCards.control.button.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.1f // opacity
            };
            light.tabBar.moreCards.control.button.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.2f // opacity
            };
            light.tabBar.moreCards.control.icon.background[(size_t)ButtonState::Idle] =
            light.tabBar.moreCards.control.icon.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.8f // opacity
            };
            light.tabBar.moreCards.control.icon.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0x000000 }, // color
                0.65f // opacity
            };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.background.color = D2D1::ColorF{ 0x202020 };
            dark.stroke.color = D2D1::ColorF{ 0x1d1d1d };

            dark.tabBar.card.main[(size_t)CardState::Active].background =
            {
                D2D1::ColorF{ 0x202020 }, // color
                1.0f // opacity
            };
            dark.tabBar.card.main[(size_t)CardState::Hover].background =
            {
                D2D1::ColorF{ 0x373737 }, // color
                1.0f // opacity
            };
            dark.tabBar.card.main[(size_t)CardState::Dormant].background =
            {
                D2D1::ColorF{ 0x1d1d1d }, // color
                1.0f // opacity
            };
            dark.tabBar.card.activeShadowColor = D2D1::ColorF{ 0xa6a6a6 };

            dark.tabBar.moreCards.control.button.background[(size_t)ButtonState::Idle] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.0f // opacity
            };
            dark.tabBar.moreCards.control.button.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.2f // opacity
            };
            dark.tabBar.moreCards.control.button.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.4f // opacity
            };
            dark.tabBar.moreCards.control.icon.background[(size_t)ButtonState::Idle] =
            dark.tabBar.moreCards.control.icon.background[(size_t)ButtonState::Hover] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.8f // opacity
            };
            dark.tabBar.moreCards.control.icon.background[(size_t)ButtonState::Down] =
            {
                D2D1::ColorF{ 0xffffff }, // color
                0.55f // opacity
            };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(TabGroup);

    void TabGroup::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _ref.maskWhenBelowDragWindow =
        {
            g_colorGroup.primary, // color
            0.5f // opacity
        };
        _D14_UPDATE_THEME_DATA_1(background.color);
        _D14_UPDATE_THEME_DATA_1(stroke.color);

        for (size_t i = 0; i < cpp_lang_utils::enumCount<CardState>; ++i)
        {
            _D14_UPDATE_THEME_DATA_1(tabBar.card.main[i].background);
            _D14_UPDATE_THEME_DATA_1(tabBar.card.activeShadowColor);

            _D14_UPDATE_THEME_DATA_ARRAY_1(tabBar.moreCards.control.button.background);
            _D14_UPDATE_THEME_DATA_ARRAY_1(tabBar.moreCards.control.icon.background);

            _D14_UPDATE_THEME_DATA_1(maskWhenBelowDragWindow);
        }
    }
}
