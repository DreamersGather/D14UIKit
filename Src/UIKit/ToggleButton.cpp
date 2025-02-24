#include "Common/Precompile.h"

#include "UIKit/ToggleButton.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"

namespace d14engine::uikit
{
    ToggleButton::ToggleButton(
        ShrdPtrParam<IconLabel> content,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        FilledButton(content, roundRadius, rect)
    {
        m_state = { DEACTIVATED, StatefulObject::State::ButtonFlag::Idle };
        StatefulObject::m_currState.flag = DEACTIVATED;
    }

    ToggleButton::ToggleButton(
        WstrParam text,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        ToggleButton(
            IconLabel::uniformLayout(text),
            roundRadius,
            rect) { }

    void ToggleButton::setActivated(StatefulObject::State::ActiveFlag flag)
    {
        m_state.activeFlag = flag;

        StatefulObject::Event soe = {};
        soe.flag = m_state.activeFlag;

        if (StatefulObject::m_currState != soe)
        {
            StatefulObject::m_currState = soe;
            onStateChange(StatefulObject::m_currState);
        }
    }

    void ToggleButton::setActivatedState(StatefulObject::State::ActiveFlag flag)
    {
        m_state.activeFlag = flag;
        StatefulObject::m_currState.flag = m_state.activeFlag;
    }

    void ToggleButton::onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr)
    {
        if (m_state.activeFlag == ACTIVATED)
        {
            auto& dstSetting = Button::getAppearance();
            auto& srcSetting = getAppearance().main[(size_t)Button::m_currState];

            dstSetting.foreground = srcSetting.foreground;
            dstSetting.background = srcSetting.background;
            dstSetting.stroke = srcSetting.stroke;

            Button::onRendererDrawD2d1ObjectHelper(rndr);
        }
        else FilledButton::onRendererDrawD2d1ObjectHelper(rndr);
    }

    void ToggleButton::onChangeThemeStyleHelper(const ThemeStyle& style)
    {
        FilledButton::onChangeThemeStyleHelper(style);

        getAppearance().changeTheme(style.name);
    }

    void ToggleButton::onMouseButtonReleaseHelper(Button::Event& e)
    {
        FilledButton::onMouseButtonReleaseHelper(e);

        if (e.left()) setActivated(StatefulObject::m_currState.activated() ? DEACTIVATED : ACTIVATED);
    }
}
