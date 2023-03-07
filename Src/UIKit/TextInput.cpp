#include "Common/Precompile.h"

#include "UIKit/TextInput.h"

#include "Renderer/TickTimer.h"
#include "UIKit/AnimationUtils/Motion.h"
#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    void TextInput::onRendererUpdateObject2DHelper(Renderer* rndr)
    {
        RawTextInput::onRendererUpdateObject2DHelper(rndr);

        auto& animSetting = getAppearance().bottomLine.animation;

        auto deltaSecs = (float)rndr->timer()->deltaSecs();
        float totalDistance = width() - 2.0f * roundRadiusX;

        if (isFocused() && animSetting.enabled && totalDistance > 0.0f)
        {
            m_currDynamicBottomLineLength = animation_utils::accelUniformDecelMotion
            (
                /* currDisplacement   */ m_currDynamicBottomLineLength,
                /* lastFrameDeltaSecs */ deltaSecs,
                /* totalDistance      */ totalDistance,
                /* uniformMotionSecs  */ animSetting.durationInSecs.uniform,
                /* variableMotionSecs */ animSetting.durationInSecs.variable
            );
        }
    }

    void TextInput::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        auto& srcSetting = getAppearance().main[(size_t)m_currState];
        auto& dstSetting = Label::getAppearance();

        dstSetting.background = srcSetting.background;
        dstSetting.stroke = srcSetting.stroke;

        RawTextInput::onRendererDrawD2d1LayerHelper(rndr);
    }

    void TextInput::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Static Bottom Line
        auto& srcBtlnSetting = getAppearance().bottomLine;
        auto& dstSetting = RawTextInput::getAppearance();

        dstSetting.bottomLine.bottomOffset = srcBtlnSetting.bottomOffset;
        dstSetting.bottomLine.background = srcBtlnSetting.background.Static[(size_t)m_currState];
        dstSetting.bottomLine.strokeWidth = srcBtlnSetting.strokeWidth;

        RawTextInput::onRendererDrawD2d1ObjectHelper(rndr);

        // Dynamic Bottom Line
        if (m_currDynamicBottomLineLength > 0.0f)
        {
            auto& dynamicBackground = srcBtlnSetting.background.Dynamic;

            resource_utils::g_solidColorBrush->SetColor(dynamicBackground.color);
            resource_utils::g_solidColorBrush->SetOpacity(dynamicBackground.opacity);

            auto point0 = math_utils::offset(math_utils::leftBottom(m_absoluteRect),
            {
                roundRadiusX, srcBtlnSetting.bottomOffset
            });
            auto point1 = math_utils::offset(point0, { m_currDynamicBottomLineLength, 0.0f });

            auto solidColorBrush = resource_utils::g_solidColorBrush.Get();
            float strokeWidth = srcBtlnSetting.strokeWidth;

            rndr->d2d1DeviceContext()->DrawLine(point0, point1, solidColorBrush, strokeWidth);
        }
    }

    void TextInput::onChangeThemeHelper(WstrParam themeName)
    {
        RawTextInput::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void TextInput::onGetFocusHelper()
    {
        RawTextInput::onGetFocusHelper();

        increaseAnimationCount();

        m_currState = State::Active;

        if (!getAppearance().bottomLine.animation.enabled)
        {
            m_currDynamicBottomLineLength = width() - 2.0f * roundRadiusX;
        }
    }

    void TextInput::onLoseFocusHelper()
    {
        RawTextInput::onLoseFocusHelper();

        decreaseAnimationCount();

        m_currState = State::Idle;

        m_currDynamicBottomLineLength = 0.0f;
    }

    void TextInput::onMouseEnterHelper(MouseMoveEvent& e)
    {
        RawTextInput::onMouseEnterHelper(e);

        if (m_currState != State::Active)
        {
            m_currState = State::Hover;
        }
    }

    void TextInput::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        RawTextInput::onMouseLeaveHelper(e);

        if (m_currState != State::Active)
        {
            m_currState = State::Idle;
        }
    }

    void TextInput::setEnabled(bool value)
    {
        RawTextInput::setEnabled(value);

        if (value)
        {
            m_currState = State::Idle;
        }
        else // lose focus when disabled
        {
            if (isFocused())
            {
                Application::g_app->focusUIObject(nullptr);
            }
            m_currState = State::Disabled;
        }
    }
}
