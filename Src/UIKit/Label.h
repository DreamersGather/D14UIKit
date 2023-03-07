#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Label.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct Label : appearance::Label, Panel
    {
        Label(WstrParam text = {}, const D2D_RECT_F& rect = {});

        _D14_SET_APPEARANCE_GETTER(Label)

    protected:
        Wstring m_text = {};

        // input-str ---> preprocess ---> output-str ---> set-text
        virtual Optional<Wstring> preprocessInputStr(WstrParam in);

    public:
        const Wstring& text() const;
        void setText(WstrParam text);

        void setTextFormat(IDWriteTextFormat* textFormat);

        void insertTextFragment(WstrParam fragment, size_t offset);
        void appendTextFragment(WstrParam fragment);

        struct CharacterRange { size_t offset, count; };

        void eraseTextFragment(const CharacterRange& range);

        void copyTextStyle(Label* source, OptParam<WstringView> text = std::nullopt);

    protected:
        ComPtr<IDWriteTextLayout> m_textLayout = {};

        DWRITE_OVERHANG_METRICS m_textOverhangs = {};

    public:
        IDWriteTextLayout* textLayout() const;

        DWRITE_TEXT_METRICS textMetrics() const;

        const DWRITE_OVERHANG_METRICS& textOverhangs() const;
        void updateTextOverhangMetrics();

        D2D1_SIZE_F textAreaSize() const;
        
        enum class HorzAlignment { None, Left, Center, Right };
        enum class VertAlignment { None, Top, Center, Bottom };

        // Uses the cached overhang metrics to force align the text.
        struct HardAlignment
        {
            HorzAlignment horz = HorzAlignment::None;
            VertAlignment vert = VertAlignment::None;
        }
        hardAlignment = {}; // compare this with TextLayout.Alignment

        struct TextLayoutParams
        {
            Optional<WstringView> text = std::nullopt;

            IDWriteTextFormat* textFormat = nullptr;

            Optional<float> maxWidth = std::nullopt;
            Optional<float> maxHeight = std::nullopt;

            Optional<DWRITE_TEXT_ALIGNMENT> textAlignment = std::nullopt;
            Optional<DWRITE_PARAGRAPH_ALIGNMENT> paragraphAlignment = std::nullopt;

            Optional<DWRITE_WORD_WRAPPING> wordWrapping = std::nullopt;
        };
        ComPtr<IDWriteTextLayout>
        getTextLayout(const TextLayoutParams& params = {}) const;

        using TextMetricsParams = TextLayoutParams;

        DWRITE_TEXT_METRICS // generate new temporary layout
        getTextMetrics(const TextMetricsParams& params = {}) const;

    public:
        D2D1_DRAW_TEXT_OPTIONS drawTextOptions = D2D1_DRAW_TEXT_OPTIONS_NONE;

        struct PointHitTestResult
        {
            BOOL isTrailingHit = {};
            BOOL isInside = {};
            DWRITE_HIT_TEST_METRICS metrics = {};
        };
        PointHitTestResult hitTestPoint(FLOAT pointX, FLOAT pointY);

        struct TextPosHitTestResult
        {
            FLOAT pointX = {};
            FLOAT pointY = {};
            DWRITE_HIT_TEST_METRICS metrics = {};
        };
        TextPosHitTestResult hitTestTextPos(UINT32 textPosition, BOOL isTrailingHit);

        struct TextRangeHitTestResult
        {
            TextRangeHitTestResult(UINT32 count = 0)
                :
                metrics(count) { }

            std::vector<DWRITE_HIT_TEST_METRICS> metrics = {};
        };
        TextRangeHitTestResult // perform hit test for multiline
        hitTestTextRange(UINT32 textPosition, UINT32 textLength, FLOAT originX, FLOAT originY);

    protected:
        // IDrawObject2D
        void drawBackground(renderer::Renderer* rndr);
        void drawText(renderer::Renderer* rndr);
        void drawOutline(renderer::Renderer* rndr);
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;
    };
}
