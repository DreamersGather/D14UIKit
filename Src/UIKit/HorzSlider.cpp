#include "Common/Precompile.h"

#include "UIKit/HorzSlider.h"

#include "Common/MathUtils/Basic.h"

namespace d14engine::uikit
{
    void HorzSlider::loadValueLabelShadowBitmap()
    {
        auto& rectSize = getAppearance().valueLabel.mainRect.geometry.size;
        auto& trngSize = getAppearance().valueLabel.sideTriangle.size;

        auto bitmapWidth = math_utils::round<UINT>(rectSize.width);
        auto bitmapHeight = math_utils::round<UINT>(rectSize.height + trngSize.height);

        valueLabelShadow.loadBitmap(bitmapWidth, bitmapHeight);
    }

    D2D1_RECT_F HorzSlider::thumbAreaExtendedRect(const D2D1_RECT_F& flatRect) const
    {
        return math_utils::stretch(flatRect, { getAppearance().thumbAreaExtension, 0.0f });
    }

    D2D1_POINT_2F HorzSlider::valueToGeometryOffset(float value) const
    {
        float valueSpan = m_maxValue - m_minValue;

        if (width() > 0.0f && valueSpan > 0.0f)
        {
            if (value <= m_minValue) return { 0.0f, 0.0f };
            else if (value >= m_maxValue) return { width(), 0.0f };
            else return { (value - m_minValue) / valueSpan * width(), 0.0f };
        }
        else return { 0.0f, 0.0f };
    }

    float HorzSlider::geometryOffsetToValue(const D2D1_POINT_2F& offset) const
    {
        float valueSpan = m_maxValue - m_minValue;

        if (width() > 0.0f && valueSpan > 0.0f)
        {
            if (offset.x <= 0.0f) return m_minValue;
            else if (offset.x >= width()) return m_maxValue;
            else return m_minValue + (offset.x / width()) * valueSpan;
        }
        else return 0.0f;
    }

    D2D1_RECT_F HorzSlider::valueLabelMainRectInShadow() const
    {
        auto& geoSetting = getAppearance().valueLabel.mainRect.geometry;

        return math_utils::rect({ 0.0f, 0.0f }, geoSetting.size);
    }

    math_utils::Triangle2D HorzSlider::valueLabelSideTriangleInShadow() const
    {
        auto& rectSize = getAppearance().valueLabel.mainRect.geometry.size;
        auto& trngSize = getAppearance().valueLabel.sideTriangle.size;

        float halfRectWidth = rectSize.width * 0.5f;
        float halfTrngWidth = trngSize.width * 0.5f;

        D2D1_POINT_2F vertex0 =
        {
            halfRectWidth,
            rectSize.height + trngSize.height
        };
        D2D1_POINT_2F vertex1 =
        {
            halfRectWidth - halfTrngWidth,
            rectSize.height
        };
        D2D1_POINT_2F vertex2 =
        {
            halfRectWidth + halfTrngWidth,
            rectSize.height
        };
        return { vertex0, vertex1, vertex2 };
    }

    D2D1_RECT_F HorzSlider::filledBarAbsoluteRect() const
    {
        float barHeight = getAppearance().bar.filled.geometry.height;
        float barRectTop = m_absoluteRect.top + (height() - barHeight) * 0.5f;
        return
        {
            m_absoluteRect.left,
            barRectTop,
            m_absoluteRect.left + valueToGeometryOffset(m_value).x,
            barRectTop + barHeight
        };
    }

    D2D1_RECT_F HorzSlider::completeBarAbsoluteRect() const
    {
        float barHeight = getAppearance().bar.complete.geometry.height;
        float barRectTop = m_absoluteRect.top + (height() - barHeight) * 0.5f;
        return
        {
            m_absoluteRect.left,
            barRectTop,
            m_absoluteRect.right,
            barRectTop + barHeight
        };
    }

    D2D1_RECT_F HorzSlider::handleAbsoluteRect() const
    {
        auto& handleSize = getAppearance().handle.geometry.size;
        return math_utils::rect
        (
            m_absoluteRect.left + valueToGeometryOffset(m_value).x - handleSize.width * 0.5f,
            m_absoluteRect.top + (height() - handleSize.height) * 0.5f,
            handleSize.width,
            handleSize.height
        );
    }

    D2D1_RECT_F HorzSlider::valueLabelSelfCoordRect() const
    {
        auto& setting = getAppearance().valueLabel;

        auto& rectSize = setting.mainRect.geometry.size;
        auto& trngSize = setting.sideTriangle.size;

        D2D1_RECT_F rect = {};

        rect.left = valueToGeometryOffset(m_value).x - rectSize.width * 0.5f;
        rect.right = rect.left + rectSize.width;
        rect.bottom = height() * 0.5f - setting.offset - trngSize.height;
        rect.top = rect.bottom - rectSize.height;

        return rect;
    }

    D2D1_RECT_F HorzSlider::valueLabelShadowSelfCoordRect() const
    {
        auto& trngSize = getAppearance().valueLabel.sideTriangle.size;

        return math_utils::increaseBottom(valueLabelSelfCoordRect(), trngSize.height);
    }
}
