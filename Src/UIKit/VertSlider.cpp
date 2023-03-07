#include "Common/Precompile.h"

#include "UIKit/VertSlider.h"

#include "Common/MathUtils/Basic.h"

namespace d14engine::uikit
{
    void VertSlider::loadValueLabelShadowBitmap()
    {
        auto& rectSize = getAppearance().valueLabel.mainRect.geometry.size;
        auto& trngSize = getAppearance().valueLabel.sideTriangle.size;

        auto bitmapWidth = math_utils::round<UINT>(rectSize.width + trngSize.height);
        auto bitmapHeight = math_utils::round<UINT>(rectSize.height);

        valueLabelShadow.loadShadowBitmap(bitmapWidth, bitmapHeight);
    }

    D2D1_RECT_F VertSlider::thumbAreaExtendedRect(const D2D1_RECT_F& flatRect) const
    {
        return math_utils::stretch(flatRect, { 0.0f, getAppearance().thumbAreaExtension });
    }

    D2D1_POINT_2F VertSlider::valueToGeometryOffset(float value) const
    {
        float valueSpan = m_maxValue - m_minValue;

        if (height() > 0.0f && valueSpan > 0.0f)
        {
            if (value >= m_maxValue) return { 0.0f, 0.0f };
            else if (value <= m_minValue) return { 0.0f, height() };
            else return { 0.0f, (m_maxValue - value) / valueSpan * height() };
        }
        else return { 0.0f, 0.0f };
    }

    float VertSlider::geometryOffsetToValue(const D2D1_POINT_2F& offset) const
    {
        float valueSpan = m_maxValue - m_minValue;

        if (height() > 0.0f && valueSpan > 0.0f)
        {
            if (offset.y <= 0.0f) return m_maxValue;
            else if (offset.y >= height()) return m_minValue;
            else return m_maxValue - (offset.y / height()) * valueSpan;
        }
        else return 0.0f;
    }

    D2D1_RECT_F VertSlider::valueLabelMainRectInShadow() const
    {
        auto& geoSetting = getAppearance().valueLabel.mainRect.geometry;

        return math_utils::rect({ 0.0f, 0.0f }, geoSetting.size);
    }

    math_utils::Triangle2D VertSlider::valueLabelSideTriangleInShadow() const
    {
        auto& rectSize = getAppearance().valueLabel.mainRect.geometry.size;
        auto& trngSize = getAppearance().valueLabel.sideTriangle.size;

        float halfRectHeight = rectSize.height * 0.5f;
        float halfTrngHeight = trngSize.height * 0.5f;

        D2D1_POINT_2F vertex0 =
        {
            rectSize.width + trngSize.height,
            halfRectHeight
        };
        D2D1_POINT_2F vertex1 =
        {
            rectSize.width,
            halfRectHeight - halfTrngHeight
        };
        D2D1_POINT_2F vertex2 =
        {
            rectSize.width,
            halfRectHeight + halfTrngHeight
        };
        return { vertex0, vertex1, vertex2 };
    }

    D2D1_RECT_F VertSlider::filledBarAbsoluteRect() const
    {
        float barHeight = getAppearance().bar.filled.geometry.height;
        float barRectLeft = m_absoluteRect.left + (width() - barHeight) * 0.5f;
        return
        {
            barRectLeft,
            m_absoluteRect.top + valueToGeometryOffset(m_value).y,
            barRectLeft + barHeight,
            m_absoluteRect.bottom
        };
    }

    D2D1_RECT_F VertSlider::completeBarAbsoluteRect() const
    {
        float barHeight = getAppearance().bar.complete.geometry.height;
        float barRectLeft = m_absoluteRect.left + (width() - barHeight) * 0.5f;
        return
        {
            barRectLeft,
            m_absoluteRect.top,
            barRectLeft + barHeight,
            m_absoluteRect.bottom
        };
    }

    D2D1_RECT_F VertSlider::handleAbsoluteRect() const
    {
        auto& handleSize = getAppearance().handle.geometry.size;
        return math_utils::rect
        (
            m_absoluteRect.left + (width() - handleSize.width) * 0.5f,
            m_absoluteRect.top + valueToGeometryOffset(m_value).y - handleSize.height * 0.5f,
            handleSize.width,
            handleSize.height
        );
    }

    D2D1_RECT_F VertSlider::valueLabelSelfCoordRect() const
    {
        auto& setting = getAppearance().valueLabel;

        auto& rectSize = setting.mainRect.geometry.size;
        auto& trngSize = setting.sideTriangle.size;

        D2D1_RECT_F rect = {};

        rect.right = width() * 0.5f - setting.offset - trngSize.height;
        rect.left = rect.right - rectSize.width;
        rect.top = valueToGeometryOffset(m_value).y - rectSize.height * 0.5f;
        rect.bottom = rect.top + rectSize.height;

        return rect;
    }

    D2D1_RECT_F VertSlider::valueLabelShadowSelfCoordRect() const
    {
        auto& trngSize = getAppearance().valueLabel.sideTriangle.size;

        return math_utils::increaseRight(valueLabelSelfCoordRect(), trngSize.height);
    }
}
