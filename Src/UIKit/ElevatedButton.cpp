#include "Common/Precompile.h"

#include "UIKit/ElevatedButton.h"

#include "Common/MathUtils/2D.h"

#include "Renderer/Renderer.h"

#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    ElevatedButton::ElevatedButton(
        ShrdPtrParam<IconLabel> content,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        FilledButton(content, roundRadius, rect),
        shadow(math_utils::roundu(size())) { }

    ElevatedButton::ElevatedButton(
        WstrParam text,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        ElevatedButton(
            IconLabel::uniformLayout(text),
            roundRadius,
            rect) { }

    void ElevatedButton::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        FilledButton::onRendererDrawD2d1LayerHelper(rndr);

        shadow.beginShadowDraw(rndr->d2d1DeviceContext());
        {
            auto& shadowSetting = getAppearance().shadow;

            resource_utils::g_solidColorBrush->SetOpacity(1.0f);

            D2D1_ROUNDED_RECT roundedRect =
            {
                math_utils::moveVertex(selfCoordRect(), shadowSetting.offset),
                roundRadiusX, roundRadiusY
            };
            rndr->d2d1DeviceContext()->FillRoundedRectangle(
                roundedRect, resource_utils::g_solidColorBrush.Get());
        }
        shadow.endShadowDraw(rndr->d2d1DeviceContext());
    }

    void ElevatedButton::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Shadow
        auto& shadowSetting = getAppearance().shadow;

        shadow.color = shadowSetting.color[(size_t)m_currState];
        shadow.standardDeviation = shadowSetting.standardDeviation;

        shadow.configShadowEffectInput(
            resource_utils::g_shadowEffect.Get());

        rndr->d2d1DeviceContext()->DrawImage(
            resource_utils::g_shadowEffect.Get(),
            math_utils::roundf(absolutePosition()));

        // Entity
        FilledButton::onRendererDrawD2d1ObjectHelper(rndr);
    }

    void ElevatedButton::onSizeHelper(SizeEvent& e)
    {
        FilledButton::onSizeHelper(e);

        shadow.loadShadowBitmap(math_utils::roundu(e.size));
    }

    void ElevatedButton::onChangeThemeHelper(WstrParam themeName)
    {
        FilledButton::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }
}
