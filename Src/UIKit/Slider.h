#pragma once

#include "Common/Precompile.h"

#include "Common/MathUtils/2D.h"

#include "UIKit/Appearances/Slider.h"
#include "UIKit/Panel.h"
#include "UIKit/ShadowMask.h"
#include "UIKit/ValuefulObject.h"

namespace d14engine::uikit
{
    struct Label;

    struct Slider : appearance::Slider, Panel, ValuefulObject<float>
    {
        Slider(
            const D2D1_RECT_F& rect = {},
            float value = 0.0f,
            float minValue = 0.0f,
            float maxValue = 100.0f);

        void onInitializeFinish() override;

        ShadowMask handleShadow = {};
        ShadowMask valueLabelMask = {};

        void loadHandleShadowBitmap();
        virtual void loadValueLabelMaskBitmap() = 0;

        ComPtr<ID2D1PathGeometry> sideTrianglePathGeo = {};

        void loadSideTrianglePathGeo();

        virtual D2D1_RECT_F thumbAreaExtendedRect(const D2D1_RECT_F& flatRect) const = 0;

        _D14_SET_APPEARANCE_GETTER(Slider)

    public:
        void onStartSliding(float value);

        Function<void(Slider*, float)> f_onStartSliding = {};

        void onEndSliding(float value);

        Function<void(Slider*, float)> f_onEndSliding = {};

    protected:
        void onStartSlidingHelper(float value);
        void onEndSlidingHelper(float value);

    public:
        enum class StepMode
        {
            Continuous, Discrete
        }
        stepMode = StepMode::Continuous;

        float stepInterval = 0.0f;

    public:
        bool setValue(float value) override;
        bool setMinValue(float value) override;
        bool setMaxValue(float value) override;
    
    protected:
        bool m_isSliding = false;

        SharedPtr<Label> m_valueLabel = {};

    public:
        const SharedPtr<Label>& valueLabel() const;

    protected:
        virtual D2D1_POINT_2F valueToGeometryOffset(float value) const = 0;
        virtual float geometryOffsetToValue(const D2D1_POINT_2F& offset) const = 0;

        void updateValue(const D2D1_POINT_2F& offset);

        virtual D2D1_RECT_F valueLabelMainRectInShadow() const = 0;
        virtual math_utils::Triangle2D valueLabelSideTriangleInShadow() const = 0;

        virtual D2D1_RECT_F filledBarAbsoluteRect() const = 0;
        virtual D2D1_RECT_F completeBarAbsoluteRect() const = 0;
        virtual D2D1_RECT_F handleAbsoluteRect() const = 0;
        virtual D2D1_RECT_F valueLabelSelfCoordRect() const = 0;
        virtual D2D1_RECT_F valueLabelShadowSelfCoordRect() const = 0;

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        bool isHitHelper(const Event::Point& p) const override;

        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;

        // ValuefulObject
        void onValueChangeHelper(float value) override;
    };
}
