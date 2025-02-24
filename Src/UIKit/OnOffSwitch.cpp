#include "Common/Precompile.h"

#include "UIKit/OnOffSwitch.h"

#include "Common/MathUtils/2D.h"

#include "Renderer/TickTimer.h"

#include "UIKit/AnimationUtils/MotionFunctions.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    OnOffSwitch::OnOffSwitch(float roundRadius, const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        ClickablePanel(rect, resource_utils::g_solidColorBrush)
    {
        roundRadiusX = roundRadiusY = roundRadius;

        m_state = { OFF, State::ButtonFlag::Idle };
        m_currState.flag = OFF;
    }

    void OnOffSwitch::setEnabled(bool value)
    {
        ClickablePanel::setEnabled(value);

        if (m_animationTargetState != State::ActiveFlag::Finished)
        {
            setOnOffState(m_animationTargetState);
        }
        m_state.buttonFlag = value ?
            State::ButtonFlag::Idle :
            State::ButtonFlag::Disabled;
    }

    void OnOffSwitch::setOnOff(State::ActiveFlag flag)
    {
        m_state.activeFlag = flag;

        StatefulObject::Event soe = {};
        soe.flag = m_state.activeFlag;

        if (soe != m_currState)
        {
            m_currState = soe;
            onStateChange(m_currState);
        }
        if (m_animationTargetState != State::ActiveFlag::Finished)
        {
            m_animationTargetState = State::ActiveFlag::Finished;
            decreaseAnimationCount();
        }
    }

    void OnOffSwitch::setOnOffState(State::ActiveFlag flag)
    {
        m_state.activeFlag = flag;
        m_currState.flag = m_state.activeFlag;

        if (m_animationTargetState != State::ActiveFlag::Finished)
        {
            m_animationTargetState = State::ActiveFlag::Finished;
            decreaseAnimationCount();
        }
    }

    void OnOffSwitch::setOnOffWithAnim(State::ActiveFlag flag)
    {
        if (m_animationTargetState == State::ActiveFlag::Finished)
        {
            m_state.activeFlag = flag;
            m_currState.flag = m_state.activeFlag;

            onStateChange(m_currState);

            m_currHandleLeftOffset = 0.0f;
            m_currHandleDisplacement = 0.0f;

            m_animationTargetState = flag;
            increaseAnimationCount();
        }
    }

    void OnOffSwitch::onRendererUpdateObject2DHelper(Renderer* rndr)
    {
        auto deltaSecs = (float)rndr->timer()->deltaSecs();
        auto& animSetting = getAppearance().handle.animation;

        if (animSetting.enabled && m_animationTargetState != State::ActiveFlag::Finished)
        {
            auto& onDownGeoSetting = getAppearance().handle.geometry[(size_t)State::Flag::OnDown];
            auto& offDownGeoSetting = getAppearance().handle.geometry[(size_t)State::Flag::OffDown];

            float onDownLeftOffset = onDownGeoSetting.getLeftOffset(width());
            float offDownLeftOffset = offDownGeoSetting.getLeftOffset(width());

            float totalDistance = std::abs(onDownLeftOffset - offDownLeftOffset);

            m_currHandleDisplacement = animation_utils::accelUniformDecelMotion
            (
                /* currDisplacement   */ m_currHandleDisplacement,
                /* lastFrameDeltaSecs */ deltaSecs,
                /* totalDistance      */ totalDistance,
                /* uniformMotionSecs  */ animSetting.durationInSecs.uniform,
                /* variableMotionSecs */ animSetting.durationInSecs.variable
            );

            if (m_animationTargetState == ON)
            {
                if (onDownLeftOffset >= offDownLeftOffset)
                {
                    m_currHandleLeftOffset = offDownLeftOffset + m_currHandleDisplacement;
                }
                else m_currHandleLeftOffset = offDownLeftOffset - m_currHandleDisplacement;
            }
            else if (m_animationTargetState == OFF)
            {
                if (offDownLeftOffset >= onDownLeftOffset)
                {
                    m_currHandleLeftOffset = onDownLeftOffset + m_currHandleDisplacement;
                }
                else m_currHandleLeftOffset = onDownLeftOffset - m_currHandleDisplacement;
            }
            if (m_currHandleDisplacement >= totalDistance)
            {
                m_animationTargetState = State::ActiveFlag::Finished;

                decreaseAnimationCount();
            }
        }
    }

    void OnOffSwitch::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        auto& setting = getAppearance().main[m_state.index()];
        
        // Background
        resource_utils::g_solidColorBrush->SetColor(setting.background.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

        Panel::drawBackground(rndr);

        // Handle
        auto& geoSetting = getAppearance().handle.geometry[m_state.index()];
        auto& bkgnSetting = getAppearance().handle.background[m_state.index()];

        resource_utils::g_solidColorBrush->SetColor(bkgnSetting.color);
        resource_utils::g_solidColorBrush->SetOpacity(bkgnSetting.opacity);

        float handleLeftOffset =
            m_animationTargetState != State::ActiveFlag::Finished ?
            m_currHandleLeftOffset : geoSetting.getLeftOffset(width());

        D2D1_ROUNDED_RECT handleRoundedRect =
        {
            math_utils::offset(math_utils::centered
            (
                math_utils::leftBorderRect(m_absoluteRect), geoSetting.size),
                {  geoSetting.size.width * 0.5f + handleLeftOffset,  0.0f  }
            ),
            geoSetting.roundRadius, geoSetting.roundRadius
        };
        rndr->d2d1DeviceContext()->FillRoundedRectangle(
            handleRoundedRect, resource_utils::g_solidColorBrush.Get());

        // Outline
        resource_utils::g_solidColorBrush->SetColor(setting.stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.stroke.opacity);

        auto frame = math_utils::inner(m_absoluteRect, setting.stroke.width);
        D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

        rndr->d2d1DeviceContext()->DrawRoundedRectangle(
            outlineRect, resource_utils::g_solidColorBrush.Get(), setting.stroke.width);
    }

    void OnOffSwitch::onChangeThemeStyleHelper(const ThemeStyle& style)
    {
        ClickablePanel::onChangeThemeStyleHelper(style);

        getAppearance().changeTheme(style.name);
    }

    void OnOffSwitch::onMouseEnterHelper(MouseMoveEvent& e)
    {
        ClickablePanel::onMouseEnterHelper(e);

        m_state.buttonFlag = State::ButtonFlag::Hover;
    }

    void OnOffSwitch::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        ClickablePanel::onMouseLeaveHelper(e);

        m_state.buttonFlag = State::ButtonFlag::Idle;
    }

    void OnOffSwitch::onMouseButtonPressHelper(ClickablePanel::Event& e)
    {
        ClickablePanel::onMouseButtonPressHelper(e);

        m_state.buttonFlag = State::ButtonFlag::Down;
    }

    void OnOffSwitch::onMouseButtonReleaseHelper(ClickablePanel::Event& e)
    {
        ClickablePanel::onMouseButtonReleaseHelper(e);

        if (e.left())
        {
            m_state.buttonFlag = State::ButtonFlag::Hover;

            auto targetState = m_currState.on() ? OFF : ON;
            getAppearance().handle.animation.enabled ?
                setOnOffWithAnim(targetState) : setOnOff(targetState);
        }
    }
}
