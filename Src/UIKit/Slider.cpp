#include "Common/Precompile.h"

#include "UIKit/Slider.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/Basic.h"
#include "UIKit/Application.h"
#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Slider::Slider(
        const D2D1_RECT_F& rect,
        float value,
        float minValue,
        float maxValue)
        :
        Panel(rect, resource_utils::g_solidColorBrush)
    {
        appEventReactability.focus.get = true;

        m_takeOverChildrenDrawing = true;

        m_value = value;
        m_minValue = minValue;
        m_maxValue = maxValue;

        m_valueLabel = makeUIObject<Label>();

        m_valueLabel->setVisible(false);
        m_valueLabel->setEnabled(false);

        m_valueLabel->setTextFormat(resource_utils::g_textFormats.at(L"Default/Normal/9").Get());
        THROW_IF_FAILED(m_valueLabel->textLayout()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
    }

    void Slider::onInitializeFinish()
    {
        Panel::onInitializeFinish();

        addUIObject(m_valueLabel);

        m_valueLabel->transform(valueLabelSelfCoordRect());

        int precision = getAppearance().valueLabel.precision;

        std::wstringstream ss;
        ss << std::fixed << std::setprecision(precision) << m_value;

        m_valueLabel->setText(ss.str());

        loadHandleShadowBitmap();
        loadValueLabelShadowBitmap();
    }

    void Slider::onStartSliding(float value)
    {
        onStartSlidingHelper(value);

        if (f_onStartSliding) f_onStartSliding(this, value);
    }

    void Slider::onEndSliding(float value)
    {
        onEndSlidingHelper(value);

        if (f_onEndSliding) f_onEndSliding(this, value);
    }

    void Slider::onStartSlidingHelper(float value)
    {
        forceGlobalExclusiveFocusing = true;
    }

    void Slider::onEndSlidingHelper(float value)
    {
        forceGlobalExclusiveFocusing = false;
    }

    void Slider::loadHandleShadowBitmap()
    {
        auto& geoSetting = getAppearance().handle.geometry;

        handleShadow.loadShadowBitmap(math_utils::roundu(geoSetting.size));
    }

    bool Slider::setValue(float value)
    {
        // ValuefulObject::setValue rejects the value out of range.
        value = std::clamp(value, m_minValue, m_maxValue);

        bool isValueChanged = ValuefulObject::setValue(value);
        if (isValueChanged) onValueChange(m_value);
        return isValueChanged;
    }

    bool Slider::setMinimalValue(float value)
    {
        bool isValueChanged = ValuefulObject::setMinimalValue(value);
        if (isValueChanged) onValueChange(m_value);
        return isValueChanged;
    }

    bool Slider::setMaximalValue(float value)
    {
        bool isValueChanged = ValuefulObject::setMaximalValue(value);
        if (isValueChanged) onValueChange(m_value);
        return isValueChanged;
    }

    const SharedPtr<Label>& Slider::valueLabel() const
    {
        return m_valueLabel;
    }

    void Slider::updateValue(const D2D1_POINT_2F& offset)
    {
        float tmpValue = geometryOffsetToValue(offset);

        if (stepMode == StepMode::Discrete && stepInterval > 0.0f)
        {
            float diffValue = tmpValue - m_value;
            float absDiffValue = std::abs(diffValue);
            
            if (absDiffValue + FLT_EPSILON >= stepInterval)
            {
                setValue(m_value + math_utils::round(diffValue / stepInterval) * stepInterval);
            }
        }
        else setValue(tmpValue);
    }

    void Slider::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        // Handle Shadow
        handleShadow.beginShadowDraw(rndr->d2d1DeviceContext());
        {
            auto& geoSetting = getAppearance().handle.geometry;

            resource_utils::g_solidColorBrush->SetOpacity(1.0f);

            auto handleRect = math_utils::rect({ 0.0f, 0.0f }, geoSetting.size);

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
            {
                math_utils::moveVertex(handleRect, getAppearance().handle.shadow.offset),
                geoSetting.roundRadius, geoSetting.roundRadius
            },
            resource_utils::g_solidColorBrush.Get());
        }
        handleShadow.endShadowDraw(rndr->d2d1DeviceContext());

        // Value Label Shadow
        if (m_valueLabel->isD2d1ObjectVisible())
        {
            valueLabelShadow.beginShadowDraw(rndr->d2d1DeviceContext());
            {
                auto& setting = getAppearance().valueLabel;

                // Main rect
                auto& rectBkgn = setting.mainRect.background;

                resource_utils::g_solidColorBrush->SetColor(rectBkgn.color);
                resource_utils::g_solidColorBrush->SetOpacity(rectBkgn.opacity);

                rndr->d2d1DeviceContext()->FillRoundedRectangle(
                {
                    valueLabelMainRectInShadow(),
                    setting.mainRect.geometry.roundRadius,
                    setting.mainRect.geometry.roundRadius
                },
                resource_utils::g_solidColorBrush.Get());

                // Side Triangle
                ComPtr<ID2D1PathGeometry> pathGeo;
                THROW_IF_FAILED(rndr->d2d1Factory()->CreatePathGeometry(&pathGeo));

                ComPtr<ID2D1GeometrySink> geoSink;
                THROW_IF_FAILED(pathGeo->Open(&geoSink));
                {
                    auto triangleVertices = valueLabelSideTriangleInShadow();

                    geoSink->BeginFigure(triangleVertices[2], D2D1_FIGURE_BEGIN_FILLED);

                    geoSink->AddLines(triangleVertices.data(), (UINT32)triangleVertices.size());

                    geoSink->EndFigure(D2D1_FIGURE_END_CLOSED);
                }
                THROW_IF_FAILED(geoSink->Close());

                auto& trngBkgn = setting.sideTriangle.background;

                resource_utils::g_solidColorBrush->SetColor(trngBkgn.color);
                resource_utils::g_solidColorBrush->SetOpacity(trngBkgn.opacity);

                rndr->d2d1DeviceContext()->FillGeometry(
                    pathGeo.Get(), resource_utils::g_solidColorBrush.Get());
            }
            valueLabelShadow.endShadowDraw(rndr->d2d1DeviceContext());
        }
    }

    void Slider::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Complete Bar
        auto& complete = getAppearance().bar.complete;
        auto& completeBkgn = m_enabled ? complete.background : complete.secondaryBackground;

        resource_utils::g_solidColorBrush->SetColor(completeBkgn.color);
        resource_utils::g_solidColorBrush->SetOpacity(completeBkgn.opacity);

        float radius = complete.geometry.roundRadius;

        rndr->d2d1DeviceContext()->FillRoundedRectangle(
            { completeBarAbsoluteRect(), radius, radius }, resource_utils::g_solidColorBrush.Get());

        // Filled Bar
        auto& filled = getAppearance().bar.filled;
        auto& filledBkgn = m_enabled ? filled.background : filled.secondaryBackground;

        resource_utils::g_solidColorBrush->SetColor(filledBkgn.color);
        resource_utils::g_solidColorBrush->SetOpacity(filledBkgn.opacity);

        float fradius = filled.geometry.roundRadius;

        rndr->d2d1DeviceContext()->FillRoundedRectangle(
            { filledBarAbsoluteRect(), fradius, fradius }, resource_utils::g_solidColorBrush.Get());

        // Handle
        {
            // Shadow
            auto& shadow = getAppearance().handle.shadow;
            handleShadow.color = m_enabled ? shadow.color : shadow.secondaryColor;

            handleShadow.configShadowEffectInput(resource_utils::g_shadowEffect.Get());

            rndr->d2d1DeviceContext()->DrawImage(
                resource_utils::g_shadowEffect.Get(), math_utils::leftTop(handleAbsoluteRect()));

            // Entity
            auto& handle = getAppearance().handle;
            auto& handleBkgn = m_enabled ? handle.background : handle.secondaryBackground;

            resource_utils::g_solidColorBrush->SetColor(handleBkgn.color);
            resource_utils::g_solidColorBrush->SetOpacity(handleBkgn.opacity);

            float hradius = getAppearance().handle.geometry.roundRadius;

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
                { handleAbsoluteRect(), hradius, hradius }, resource_utils::g_solidColorBrush.Get());
        }
        // Value Label
        if (getAppearance().valueLabel.isResident)
        {
            m_valueLabel->setVisible(true);
            m_valueLabel->setEnabled(true);
        }
        if (m_valueLabel->isD2d1ObjectVisible())
        {
            auto vlblRect = selfCoordToAbsolute(valueLabelShadowSelfCoordRect());

            // Shadow
            auto& shadowSetting = getAppearance().valueLabel.shadow;

            valueLabelShadow.color = shadowSetting.color;
            valueLabelShadow.standardDeviation = shadowSetting.standardDeviation;

            valueLabelShadow.configShadowEffectInput(resource_utils::g_shadowEffect.Get());

            auto targetOffset = math_utils::roundf(math_utils::leftTop(vlblRect));

            rndr->d2d1DeviceContext()->DrawImage(resource_utils::g_shadowEffect.Get(), targetOffset);

            // Entity
            auto destinationRect = math_utils::roundf(vlblRect);

            rndr->d2d1DeviceContext()->DrawBitmap(valueLabelShadow.bitmap.Get(), destinationRect, 1.0f);

            // Text
            m_valueLabel->onRendererDrawD2d1Object(rndr);
        }
    }

    bool Slider::isHitHelper(const Event::Point& p) const
    {
        return math_utils::isOverlapped(p, thumbAreaExtendedRect(m_absoluteRect));
    }

    void Slider::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);

        m_valueLabel->transform(valueLabelSelfCoordRect());

        loadHandleShadowBitmap();
        loadValueLabelShadowBitmap();
    }

    void Slider::onChangeThemeHelper(WstrParam themeName)
    {
        Panel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void Slider::onMouseMoveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseMoveHelper(e);

        if (m_isSliding) updateValue(absoluteToSelfCoord(e.cursorPoint));
    }

    void Slider::onMouseButtonHelper(MouseButtonEvent& e)
    {
        Panel::onMouseButtonHelper(e);

        auto& p = e.cursorPoint;

        if (e.state.leftDown() || e.state.leftDblclk())
        {
            m_isSliding = true;

            updateValue(absoluteToSelfCoord(p));

            if (getAppearance().valueLabel.enabled)
            {
                m_valueLabel->setVisible(true);
                m_valueLabel->setEnabled(true);
            }
            onStartSliding(m_value);
        }
        else if (e.state.leftUp())
        {
            if (m_isSliding)
            {
                m_isSliding = false;

                if (getAppearance().valueLabel.enabled)
                {
                    m_valueLabel->setVisible(false);
                    m_valueLabel->setEnabled(false);
                }
                onEndSliding(m_value);
            }
        }
    }

    void Slider::onValueChangeHelper(float value)
    {
        ValuefulObject::onValueChangeHelper(value);

        m_valueLabel->transform(valueLabelSelfCoordRect());

        int precision = getAppearance().valueLabel.precision;

        std::wstringstream ss;
        ss << std::fixed << std::setprecision(precision) << value;

        m_valueLabel->setText(ss.str());
    }
}
