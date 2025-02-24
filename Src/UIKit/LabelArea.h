#pragma once

#include "Common/Precompile.h"

#include "Common/MathUtils/2D.h"

#include "UIKit/Appearances/LabelArea.h"
#include "UIKit/Label.h"

namespace d14engine::uikit
{
    struct LabelArea : appearance::LabelArea, Label
    {
        LabelArea(WstrParam text = {}, const D2D_RECT_F& rect = {});

        _D14_SET_APPEARANCE_GETTER(LabelArea)

    protected:
        size_t m_hiliteRangeOrigin = 0;

        Label::CharacterRange m_hiliteRange = { 0, 0 };

        TextRangeHitTestResult m_hiliteRangeData = {};

    public:
        bool keepHiliteRange = false;

        const Label::CharacterRange& hiliteRange() const;
        void setHiliteRange(const Label::CharacterRange& range);

    protected:
        // Controls the blinking of the indicator.
        bool m_showIndicator = false;

        std::pair<D2D1_POINT_2F, D2D1_POINT_2F> m_indicatorGeometry = {};
        size_t m_indicatorCharacterOffset = 0;

        // Returns the index result (uses isTrailingHit) of hitTestPoint.
        virtual size_t hitTestCharacterOffset(const D2D1_POINT_2F& sfpt);

        float m_indicatorBlinkElapsedSecs = 0.0f;

    public:
        bool keepIndicatorPosition = false;

        D2D1_RECT_F indicatorConstrainedRect = math_utils::infiniteRectF();

        size_t indicatorPosition() const;
        virtual void setIndicatorPosition(size_t characterOffset);

    public:
        virtual void performCommandCtrlA();
        virtual void performCommandCtrlC();

    protected:
        // IDrawObject2D
        void onRendererUpdateObject2DHelper(renderer::Renderer* rndr) override;

        void drawHiliteRange(renderer::Renderer* rndr);
        void drawIndicator(renderer::Renderer* rndr);
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onChangeThemeStyleHelper(const ThemeStyle& style) override;

        void onGetFocusHelper() override;

        void onLoseFocusHelper() override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;

        void onKeyboardHelper(KeyboardEvent& e) override;
    };
}
