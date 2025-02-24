#include "Common/Precompile.h"

#include "UIKit/FlatButton.h"

#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    FlatButton::FlatButton(
        ShrdPtrParam<IconLabel> content,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        Button(content, roundRadius, rect) { }

    FlatButton::FlatButton(
        WstrParam text,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        FlatButton(
            IconLabel::uniformLayout(text),
            roundRadius,
            rect) { }

    void FlatButton::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        auto& dstSetting = Button::getAppearance();
        auto& srcSetting = getAppearance().main[(size_t)m_currState];

        dstSetting.foreground = srcSetting.foreground;
        dstSetting.background = srcSetting.background;
        dstSetting.stroke = srcSetting.stroke;

        Button::onRendererDrawD2d1ObjectHelper(rndr);
    }

    void FlatButton::onChangeThemeStyleHelper(const ThemeStyle& style)
    {
        Button::onChangeThemeStyleHelper(style);

        getAppearance().changeTheme(style.name);
    }
}
