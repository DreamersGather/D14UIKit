#pragma once

#include "Common/Precompile.h"

#include "UIKit/Slider.h"

namespace d14engine::uikit
{
    struct VertSlider : Slider
    {
        using Slider::Slider;

        void loadValueLabelShadowBitmap() override;

        D2D1_RECT_F thumbAreaExtendedRect(const D2D1_RECT_F& flatRect) const override;

    protected:
        D2D1_POINT_2F valueToGeometryOffset(float value) const override;
        float geometryOffsetToValue(const D2D1_POINT_2F& offset) const override;

        D2D1_RECT_F valueLabelMainRectInShadow() const override;
        math_utils::Triangle2D valueLabelSideTriangleInShadow() const override;

        D2D1_RECT_F filledBarAbsoluteRect() const override;
        D2D1_RECT_F completeBarAbsoluteRect() const override;
        D2D1_RECT_F handleAbsoluteRect() const override;
        D2D1_RECT_F valueLabelSelfCoordRect() const override;
        D2D1_RECT_F valueLabelShadowSelfCoordRect() const override;
    };
}
