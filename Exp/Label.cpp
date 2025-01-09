#include "Common/Precompile.h"

#include "Label.h"

#include "Common.h"
#include "Font.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/Basic.h"

#include "UIKit/Label.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    Label::Label(const std::wstring& text)
        :
        Label(uikit::makeUIObject<uikit::Label>(text)) { }

    _D14_UIKIT_CTOR(Label)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    Color Label::frgnColor() const
    {
        auto& appear = pimpl->uiobj->getAppearance();
        return convert(appear.foreground.color);
    }

    void Label::setFrgnColor(const Color& value)
    {
        auto& appear = pimpl->uiobj->getAppearance();
        appear.foreground.color = convert(value);
    }

    float Label::frgnOpacity() const
    {
        auto& appear = pimpl->uiobj->getAppearance();
        return appear.foreground.opacity;
    }

    void Label::setFrgnOpacity(float value)
    {
        auto& appear = pimpl->uiobj->getAppearance();
        appear.foreground.opacity = value;
    }

    Color Label::bkgnColor() const
    {
        auto& appear = pimpl->uiobj->getAppearance();
        return convert(appear.background.color);
    }

    void Label::setBkgnColor(const Color& value)
    {
        auto& appear = pimpl->uiobj->getAppearance();
        appear.background.color = convert(value);
    }

    float Label::bkgnOpacity() const
    {
        auto& appear = pimpl->uiobj->getAppearance();
        return appear.background.opacity;
    }

    void Label::setBkgnOpacity(float value)
    {
        auto& appear = pimpl->uiobj->getAppearance();
        appear.background.opacity = value;
    }

    const std::wstring& Label::text() const
    {
        return pimpl->uiobj->text();
    }

    void Label::setText(const std::wstring& text)
    {
        pimpl->uiobj->setText(text);
    }

    DrawTextOption Label::drawTextOption() const
    {
        return (DrawTextOption)pimpl->uiobj->drawTextOptions;
    }

    void Label::setDrawTextOption(DrawTextOption option)
    {
        pimpl->uiobj->drawTextOptions = (D2D1_DRAW_TEXT_OPTIONS)option;
    }

    Font* Label::font() const { return pimpl->font; }

    void Label::setFont(Font* font)
    {
        pimpl->font = font;
        if (font != nullptr)
        {
            pimpl->uiobj->setTextFormat(font->getImpl()->textFormat.Get());
        }
        else pimpl->uiobj->setTextFormat(D14_FONT(L"Default/Normal/16"));
    }

    Label::HorzAlign Label::horzAlign() const
    {
        return (HorzAlign)pimpl->uiobj->textLayout()->GetTextAlignment();
    }

    void Label::setHorzAlign(HorzAlign value)
    {
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)value));

        // Updates horz hard alignment state.
        setHorzHardAlign(horzHardAlign());
    }

    bool Label::horzHardAlign() const
    {
        return pimpl->uiobj->hardAlignment.horz != uikit::Label::HNone;
    }

    void Label::setHorzHardAlign(bool value)
    {
        auto& dst = pimpl->uiobj->hardAlignment.horz;
        if (value)
        {
            dst = (uikit::Label::HorzAlignment)pimpl->uiobj->textLayout()->GetTextAlignment();
        }
        else dst = uikit::Label::HNone;
    }

    Label::VertAlign Label::vertAlign() const
    {
        return (VertAlign)pimpl->uiobj->textLayout()->GetParagraphAlignment();
    }

    void Label::setVertAlign(VertAlign value)
    {
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)value));

        // Updates vert hard alignment state.
        setVertHardAlign(vertHardAlign());
    }

    bool Label::vertHardAlign() const
    {
        return pimpl->uiobj->hardAlignment.vert != uikit::Label::VNone;
    }

    void Label::setVertHardAlign(bool value)
    {
        auto& dst = pimpl->uiobj->hardAlignment.vert;
        if (value)
        {
            dst = (uikit::Label::VertAlignment)pimpl->uiobj->textLayout()->GetParagraphAlignment();
        }
        else dst = uikit::Label::VNone;
    }

    Label::WordWrapping Label::wordWrapping() const
    {
        return (WordWrapping)pimpl->uiobj->textLayout()->GetWordWrapping();
    }

    void Label::setWordWrapping(WordWrapping value)
    {
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetWordWrapping((DWRITE_WORD_WRAPPING)value));
    }

    std::wstring Label::fontFamilyName() const
    {
        auto length = pimpl->uiobj->textLayout()->GetFontFamilyNameLength();
        std::wstring name(++length, L'\0');
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->GetFontFamilyName(name.data(), length));
        return name;
    }

    void Label::setFontFamilyName(const std::wstring& name, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontFamilyName(name.c_str(), textRange));
    }

    int Label::fontSize() const
    {
        return math_utils::round(pimpl->uiobj->textLayout()->GetFontSize() * 72.0f / 96.0f);
    }

    void Label::setFontSize(int value, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        pimpl->uiobj->textLayout()->SetFontSize(value * 96.0f / 72.0f, textRange);
    }

    std::wstring Label::fontLocaleName() const
    {
        auto length = pimpl->uiobj->textLayout()->GetLocaleNameLength();
        std::wstring name(++length, L'\0');
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->GetLocaleName(name.data(), length));
        return name;
    }

    void Label::setFontLocaleName(const std::wstring& name, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetLocaleName(name.c_str(), textRange));
    }

    Font::Weight Label::fontWeight() const
    {
        return (Font::Weight)pimpl->uiobj->textLayout()->GetFontWeight();
    }

    void Label::setFontWeight(Font::Weight value, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontWeight((DWRITE_FONT_WEIGHT)value, textRange));
    }

    Font::Style Label::fontStyle() const
    {
        return (Font::Style)pimpl->uiobj->textLayout()->GetFontStyle();
    }

    void Label::setFontStyle(Font::Style value, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontStyle((DWRITE_FONT_STYLE)value, textRange));
    }

    Font::Stretch Label::fontStretch() const
    {
        return (Font::Stretch)pimpl->uiobj->textLayout()->GetFontStretch();
    }

    void Label::setFontStretch(Font::Stretch value, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontStretch((DWRITE_FONT_STRETCH)value, textRange));
    }

    bool Label::underline(int offset) const
    {
        BOOL hasUnderline;
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->GetUnderline(offset, &hasUnderline));
        return hasUnderline;
    }

    void Label::setUnderline(bool value, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetUnderline(value, textRange));
    }

    bool Label::strikethrough(int offset) const
    {
        BOOL hasStrikethrough;
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->GetStrikethrough(offset, &hasStrikethrough));
        return hasStrikethrough;
    }

    void Label::setStrikethrough(bool value, const std::optional<Range>& range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.count };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetStrikethrough(value, textRange));
    }
}
