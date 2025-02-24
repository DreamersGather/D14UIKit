#include "Common/Precompile.h"

#include "UIKit/OutlinedButton.h"

#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"

namespace d14engine::uikit
{
    OutlinedButton::OutlinedButton(
        ShrdPtrParam<IconLabel> content,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        FlatButton(content, roundRadius, rect) { }

    OutlinedButton::OutlinedButton(
        WstrParam text,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        OutlinedButton(
            IconLabel::uniformLayout(text),
            roundRadius,
            rect) { }

    void OutlinedButton::onChangeThemeStyleHelper(const ThemeStyle& style)
    {
        Button::onChangeThemeStyleHelper(style);

        getAppearance().changeTheme(FlatButton::getAppearance(), style.name);
    }
}
