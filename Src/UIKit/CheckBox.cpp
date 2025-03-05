#include "Common/Precompile.h"

#include "UIKit/CheckBox.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "Renderer/Renderer.h"

#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    CheckBox::CheckBox(
        bool isTripleState,
        float roundRadius,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        ClickablePanel(rect, resource_utils::g_solidColorBrush)
    {
        roundRadiusX = roundRadiusY = roundRadius;

        m_state = { UNCHECKED, State::ButtonFlag::Idle };
        m_currState.flag = UNCHECKED;

        enableTripleState(isTripleState);
    }

    void CheckBox::onInitializeFinish()
    {
        ClickablePanel::onInitializeFinish();

        loadCheckedIconStrokeStyle();
    }

    void CheckBox::loadCheckedIconStrokeStyle()
    {
        THROW_IF_NULL(Application::g_app);

        auto factory = Application::g_app->dx12Renderer()->d2d1Factory();

        auto properties = D2D1::StrokeStyleProperties
        (
            D2D1_CAP_STYLE_ROUND,
            D2D1_CAP_STYLE_ROUND,
            D2D1_CAP_STYLE_ROUND,
            D2D1_LINE_JOIN_MITER,
            10.0f, // miterLimit
            D2D1_DASH_STYLE_SOLID,
            0.0f   // dashOffset
        );
        THROW_IF_FAILED(factory->CreateStrokeStyle(
            properties, nullptr, 0, &checkedIcon.strokeStyle));
    }

    void CheckBox::setEnabled(bool value)
    {
        ClickablePanel::setEnabled(value);

        m_state.buttonFlag = value ?
            State::ButtonFlag::Idle :
            State::ButtonFlag::Disabled;
    }

    void CheckBox::setChecked(State::ActiveFlag flag)
    {
        m_state.activeFlag = flag;

        StatefulObject::Event soe = {};
        soe.flag = m_state.activeFlag;

        if (m_currState != soe)
        {
            m_currState = soe;
            onStateChange(m_currState);
        }
    }

    void CheckBox::setCheckedState(State::ActiveFlag flag)
    {
        m_state.activeFlag = flag;
        m_currState.flag = m_state.activeFlag;
    }

    bool CheckBox::isTripleState() const
    {
        return m_isTripleState;
    }

    void CheckBox::enableTripleState(bool value)
    {
        setChecked(UNCHECKED);

        m_isTripleState = value;
        if (m_isTripleState)
        {
            m_stateTransitionMap =
            {
                /* Unchecked    */ State::ActiveFlag::Intermediate,
                /* Intermediate */ State::ActiveFlag::Checked,
                /* Checked      */ State::ActiveFlag::Unchecked
            };
        }
        else // Use double-state mode.
        {
            m_stateTransitionMap =
            {
                /* Unchecked    */ State::ActiveFlag::Checked,
                /* Intermediate */ State::ActiveFlag::Unchecked,
                /* Checked      */ State::ActiveFlag::Unchecked
            };
        }
    }

    void CheckBox::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        auto& setting = getAppearance().button[m_state.index()];

        // Background
        resource_utils::g_solidColorBrush->SetColor(setting.background.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);

        ClickablePanel::drawBackground(rndr);

        // Icon
        if (m_state.activeFlag == INTERMEDIATE)
        {
            auto& geoSetting = getAppearance().icon.geometry.intermediate;
            auto& iconBackground = getAppearance().icon.background[m_state.index()];

            resource_utils::g_solidColorBrush->SetColor(iconBackground.color);
            resource_utils::g_solidColorBrush->SetOpacity(iconBackground.opacity);

            auto iconRect = math_utils::centered(m_absoluteRect, geoSetting.size);

            rndr->d2d1DeviceContext()->FillRectangle(iconRect, resource_utils::g_solidColorBrush.Get());
        }
        else if (m_state.activeFlag == CHECKED)
        {
            auto& geoSetting = getAppearance().icon.geometry.checked;
            auto& iconBackground = getAppearance().icon.background[m_state.index()];

            resource_utils::g_solidColorBrush->SetColor(iconBackground.color);
            resource_utils::g_solidColorBrush->SetOpacity(iconBackground.opacity);

            auto iconLeftTop = absolutePosition();

            rndr->d2d1DeviceContext()->DrawLine(
                math_utils::offset(iconLeftTop, geoSetting.tickLine0.point0),
                math_utils::offset(iconLeftTop, geoSetting.tickLine0.point1),
                resource_utils::g_solidColorBrush.Get(),
                geoSetting.strokeWidth, checkedIcon.strokeStyle.Get());

            rndr->d2d1DeviceContext()->DrawLine(
                math_utils::offset(iconLeftTop, geoSetting.tickLine1.point0),
                math_utils::offset(iconLeftTop, geoSetting.tickLine1.point1),
                resource_utils::g_solidColorBrush.Get(),
                geoSetting.strokeWidth, checkedIcon.strokeStyle.Get());
        }
        // Outline
        resource_utils::g_solidColorBrush->SetColor(setting.stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.stroke.opacity);

        auto frame = math_utils::inner(m_absoluteRect, setting.stroke.width);
        D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

        rndr->d2d1DeviceContext()->DrawRoundedRectangle(
            outlineRect, resource_utils::g_solidColorBrush.Get(), setting.stroke.width);
    }

    void CheckBox::onChangeThemeStyleHelper(const ThemeStyle& style)
    {
        ClickablePanel::onChangeThemeStyleHelper(style);

        getAppearance().changeTheme(style.name);
    }

    void CheckBox::onMouseEnterHelper(MouseMoveEvent& e)
    {
        ClickablePanel::onMouseEnterHelper(e);

        m_state.buttonFlag = State::ButtonFlag::Hover;
    }

    void CheckBox::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        ClickablePanel::onMouseLeaveHelper(e);

        m_state.buttonFlag = State::ButtonFlag::Idle;
    }

    void CheckBox::onMouseButtonPressHelper(ClickablePanel::Event& e)
    {
        ClickablePanel::onMouseButtonPressHelper(e);

        m_state.buttonFlag = State::ButtonFlag::Down;
    }

    void CheckBox::onMouseButtonReleaseHelper(ClickablePanel::Event& e)
    {
        ClickablePanel::onMouseButtonReleaseHelper(e);

        if (e.left())
        {
            m_state.buttonFlag = State::ButtonFlag::Hover;

            setChecked(m_stateTransitionMap[(size_t)m_state.activeFlag]);
        }
    }
}
