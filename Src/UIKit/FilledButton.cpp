#include "Common/Precompile.h"

#include "UIKit/FilledButton.h"

#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"

namespace d14engine::uikit
{
    FilledButton::FilledButton(
        ShrdPtrParam<IconLabel> content,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        FlatButton(content, roundRadius, rect) { }

    FilledButton::FilledButton(
        WstrParam text,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        FilledButton(
            IconLabel::uniformLayout(text),
            roundRadius,
            rect) { }

    void FilledButton::onChangeThemeHelper(WstrParam themeName)
    {
        Button::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(FlatButton::getAppearance(), themeName);
    }
}
