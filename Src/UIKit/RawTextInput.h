#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/RawTextInput.h"
#include "UIKit/LabelArea.h"
#include "UIKit/MaskObject.h"
#include "UIKit/TextInputObject.h"

namespace d14engine::uikit
{
    struct RawTextInput : appearance::RawTextInput, LabelArea, TextInputObject
    {
        RawTextInput(bool multiline, float roundRadius = 0.0f, const D2D1_RECT_F& rect = {});

        void onInitializeFinish() override;

        _D14_SET_APPEARANCE_GETTER(RawTextInput)

        const bool multiline = {};

    public:
        void onTextContentOffsetChange(const D2D1_POINT_2F& offset);

        Function<void(RawTextInput*, const D2D1_POINT_2F&)> f_onTextContentOffsetChange = {};

    protected:
        virtual void onTextContentOffsetChangeHelper(const D2D1_POINT_2F& offset);

    protected:
        Optional<Wstring> preprocessInputStr(WstrParam in) override;

    public:
        bool editable = true;

        void setText(WstrParam text) override;

    protected:
        MaskObject m_visibleTextMask = {};

        D2D1_RECT_F m_visibleTextRect = {};

    public:
        const D2D1_RECT_F& visibleTextRect() const;
        void setVisibleTextRect(const D2D1_RECT_F& rect);

    protected:
        SharedPtr<Label> m_placeholder = {};

    public:
        const SharedPtr<Label>& placeholder() const;

    protected:
        D2D1_POINT_2F m_textContentOffset = {};

        // Override to take m_textContentOffset into consideration.
        size_t hitTestCharacterOffset(const D2D1_POINT_2F& sfpt) override;

    protected:
        virtual D2D1_POINT_2F validateTextContentOffset(const D2D1_POINT_2F& in);

    public:
        const D2D1_POINT_2F& textContentOffset() const;
        void setTextContentOffset(const D2D1_POINT_2F& offset);

        void setTextContentOffsetDirect(const D2D1_POINT_2F& offset);

        // Override to take m_textContentOffset into consideration.
        void setIndicatorPosition(size_t characterOffset) override;

    public:
        virtual void performCommandCtrlX();
        virtual void performCommandCtrlV();

    public:
        void changeCandidateText(WstrParam str);

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeStyleHelper(const ThemeStyle& style) override;

        void onKeyboardHelper(KeyboardEvent& e) override;

    public:
        // TextInputObject
        Optional<LOGFONT> getCompositionFont() const override;
        Optional<COMPOSITIONFORM> getCompositionForm() const override;

    protected:
        void onInputStringHelper(WstrParam str) override;
    };
}
