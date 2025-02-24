#include "Common/Precompile.h"

#include "UIKit/Appearances/Slider.h"

namespace d14engine::uikit::appearance
{
    Slider::Appearance::Appearance()
    {
        bar.filled.geometry =
        {
            6.0f, // height
            3.0f // round radius
        };
        bar.complete.geometry =
        {
            4.0f, // height
            2.0f // round radius
        };
    }

    void Slider::Appearance::initialize()
    {
        auto& light = (g_themeData[L"Light"] = {});
        {
            light.bar.filled.secondaryBackground =
            {
                D2D1::ColorF{ 0xbfbfbf }, // color
                1.0f // opacity
            };
            light.bar.complete.secondaryBackground =
            {
                D2D1::ColorF{ 0xbfbfbf }, // color
                0.38f // opacity
            };
            light.handle.secondaryBackground =
            {
                D2D1::ColorF{ 0xbfbfbf }, // color
                1.0f // opacity
            };
            light.handle.shadow.color = D2D1::ColorF{ 0x808080 };
            light.handle.shadow.secondaryColor = D2D1::ColorF{ 0x000000, 0.0f };

            light.valueLabel.mainRect.background =
            light.valueLabel.sideTriangle.background =
            {
                D2D1::ColorF{ 0xf9f9f9 }, // color
                1.0f // opacity
            };
            light.valueLabel.shadow.color = D2D1::ColorF{ 0x8c8c8c };
        }
        auto& dark = (g_themeData[L"Dark"] = {});
        {
            dark.bar.filled.secondaryBackground =
            {
                D2D1::ColorF{ 0x434343 }, // color
                1.0f // opacity
            };
            dark.bar.complete.secondaryBackground =
            {
                D2D1::ColorF{ 0x434343 }, // color
                0.38f // opacity
            };
            dark.handle.secondaryBackground =
            {
                D2D1::ColorF{ 0x434343 }, // color
                1.0f // opacity
            };
            dark.handle.shadow.color = D2D1::ColorF{ 0x000000 };
            dark.handle.shadow.secondaryColor = D2D1::ColorF{ 0x000000, 0.0f };

            dark.valueLabel.mainRect.background =
            dark.valueLabel.sideTriangle.background =
            {
                D2D1::ColorF{ 0x272727 }, // color
                1.0f // opacity
            };
            dark.valueLabel.shadow.color = D2D1::ColorF{ 0xa6a6a6 };
        }
    }
    _D14_SET_THEME_DATA_MAP_IMPL(Slider);

    void Slider::Appearance::changeTheme(WstrParam themeName)
    {
        _D14_FIND_THEME_DATA(themeName);

        _ref.bar.filled.background =
        {
            g_colorGroup.primary, // color
            1.0f // opacity
        };
        _ref.bar.complete.background =
        {
            g_colorGroup.primary, // color
            0.38f // opacity
        };
        _ref.handle.background =
        {
            g_colorGroup.primary, // color
            1.0f // opacity
        };
        _D14_UPDATE_THEME_DATA_1(bar.filled.background);
        _D14_UPDATE_THEME_DATA_1(bar.filled.secondaryBackground);
        _D14_UPDATE_THEME_DATA_1(bar.complete.background);
        _D14_UPDATE_THEME_DATA_1(bar.complete.secondaryBackground);

        _D14_UPDATE_THEME_DATA_1(handle.background);
        _D14_UPDATE_THEME_DATA_1(handle.secondaryBackground);
        _D14_UPDATE_THEME_DATA_1(handle.shadow.color);
        _D14_UPDATE_THEME_DATA_1(handle.shadow.secondaryColor);

        _D14_UPDATE_THEME_DATA_1(valueLabel.mainRect.background);
        _D14_UPDATE_THEME_DATA_1(valueLabel.sideTriangle.background);
        _D14_UPDATE_THEME_DATA_1(valueLabel.shadow.color);
    }
}
