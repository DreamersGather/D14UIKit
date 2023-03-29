#include "Common/Precompile.h"

#include "Font.h"
#include "Label.h"
#include "Panel.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/Basic.h"
#include "UIKit/Label.h"

using namespace d14engine;

namespace d14uikit
{
    Label::Label(const std::wstring& text)
        :
        Label(Passkey{})
    {
        Panel::pimpl->uiobj =
        Label::pimpl->uiobj =
        uikit::makeUIObject<uikit::Label>(text);

        Panel::initialize();
        Label::initialize();
    }

    Label::Label(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void Label::initialize() { }

    const std::wstring& Label::text() const
    {
        return pimpl->uiobj->text();
    }

    void Label::setText(const std::wstring& text)
    {
        pimpl->uiobj->setText(text);
    }

    const Font& Label::font() const
    {
        return pimpl->font; // ref
    }

    void Label::setFont(const Font& font)
    {
        pimpl->font = font;
        pimpl->uiobj->setTextFormat(font.getImpl()->textFormat.Get());
    }

    Label::HorzAlign Label::horzAlign() const
    {
        switch (pimpl->uiobj->textLayout()->GetTextAlignment())
        {
        case DWRITE_TEXT_ALIGNMENT_LEADING: return Left;
        case DWRITE_TEXT_ALIGNMENT_CENTER: return HCenter;
        case DWRITE_TEXT_ALIGNMENT_TRAILING: return Right;
        case DWRITE_TEXT_ALIGNMENT_JUSTIFIED: return Justified;
        default: return Left;
        }
    }

    void Label::setHorzAlign(HorzAlign value)
    {
        DWRITE_TEXT_ALIGNMENT dst = {};
        switch (value)
        {
        case Left: dst = DWRITE_TEXT_ALIGNMENT_LEADING; break;
        case HCenter: dst = DWRITE_TEXT_ALIGNMENT_CENTER; break;
        case Right: dst = DWRITE_TEXT_ALIGNMENT_TRAILING; break;
        case Justified: dst = DWRITE_TEXT_ALIGNMENT_JUSTIFIED; break;
        default: dst = DWRITE_TEXT_ALIGNMENT_LEADING; break;
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetTextAlignment(dst));

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
            auto src = pimpl->uiobj->textLayout()->GetTextAlignment();
            switch (src)
            {
            case DWRITE_TEXT_ALIGNMENT_LEADING: dst = uikit::Label::Left; break;
            case DWRITE_TEXT_ALIGNMENT_CENTER: dst = uikit::Label::HCenter; break;
            case DWRITE_TEXT_ALIGNMENT_TRAILING: dst = uikit::Label::Right; break;
            case DWRITE_TEXT_ALIGNMENT_JUSTIFIED: dst = uikit::Label::HNone;
            default: dst = uikit::Label::HNone; break;
            }
        }
        else dst = uikit::Label::HNone;
    }

    Label::VertAlign Label::vertAlign() const
    {
        switch (pimpl->uiobj->textLayout()->GetParagraphAlignment())
        {
        case DWRITE_PARAGRAPH_ALIGNMENT_NEAR: return Top;
        case DWRITE_PARAGRAPH_ALIGNMENT_CENTER: return VCenter;
        case DWRITE_PARAGRAPH_ALIGNMENT_FAR: return Bottom;
        default: return VCenter;
        }
    }

    void Label::setVertAlign(VertAlign value)
    {
        DWRITE_PARAGRAPH_ALIGNMENT dst = {};
        switch (value)
        {
        case Top: dst = DWRITE_PARAGRAPH_ALIGNMENT_NEAR; break;
        case VCenter: dst = DWRITE_PARAGRAPH_ALIGNMENT_CENTER; break;
        case Bottom: dst = DWRITE_PARAGRAPH_ALIGNMENT_FAR; break;
        default: dst = DWRITE_PARAGRAPH_ALIGNMENT_CENTER; break;
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetParagraphAlignment(dst));

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
            auto src = pimpl->uiobj->textLayout()->GetParagraphAlignment();
            switch (src)
            {
            case DWRITE_PARAGRAPH_ALIGNMENT_NEAR: dst = uikit::Label::Top; break;
            case DWRITE_PARAGRAPH_ALIGNMENT_FAR: dst = uikit::Label::Bottom; break;
            case DWRITE_PARAGRAPH_ALIGNMENT_CENTER: dst = uikit::Label::VCenter; break;
            default: dst = uikit::Label::VNone; break;
            }
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

    void Label::setFontFamilyName(const std::wstring& name, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontFamilyName(name.c_str(), textRange));
    }

    int Label::fontSize() const
    {
        return math_utils::round(pimpl->uiobj->textLayout()->GetFontSize() * 72.0f / 96.0f);
    }

    void Label::setFontSize(int value, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
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

    void Label::setFontLocaleName(const std::wstring& name, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetLocaleName(name.c_str(), textRange));
    }

    Font::Weight Label::fontWeight() const
    {
        return (Font::Weight)pimpl->uiobj->textLayout()->GetFontWeight();
    }

    void Label::setFontWeight(Font::Weight value, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontWeight((DWRITE_FONT_WEIGHT)value, textRange));
    }

    Font::Style Label::fontStyle() const
    {
        return (Font::Style)pimpl->uiobj->textLayout()->GetFontStyle();
    }

    void Label::setFontStyle(Font::Style value, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontStyle((DWRITE_FONT_STYLE)value, textRange));
    }

    Font::Stretch Label::fontStretch() const
    {
        return (Font::Stretch)pimpl->uiobj->textLayout()->GetFontStretch();
    }

    void Label::setFontStretch(Font::Stretch value, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetFontStretch((DWRITE_FONT_STRETCH)value, textRange));
    }

    bool Label::underline(int offset) const
    {
        BOOL hasUnderline;
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->GetUnderline(offset, &hasUnderline));
        return hasUnderline;
    }

    void Label::setUnderline(bool value, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetUnderline(value, textRange));
    }

    bool Label::strikethrough(int offset) const
    {
        BOOL hasStrikethrough;
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->GetStrikethrough(offset, &hasStrikethrough));
        return hasStrikethrough;
    }

    void Label::setStrikethrough(bool value, std::optional<Range> range)
    {
        DWRITE_TEXT_RANGE textRange = { 0, (UINT32)pimpl->uiobj->text().size() };
        if (range.has_value())
        {
            auto& rangeValue = range.value();
            textRange = { (UINT32)rangeValue.offset, (UINT32)rangeValue.length };
        }
        THROW_IF_FAILED(pimpl->uiobj->textLayout()->SetStrikethrough(value, textRange));
    }

    bool Label::drawTextClip() const
    {
        return pimpl->uiobj->drawTextOptions == D2D1_DRAW_TEXT_OPTIONS_CLIP;
    }

    void Label::setDrawTextClip(bool value)
    {
        pimpl->uiobj->drawTextOptions =
            value ? D2D1_DRAW_TEXT_OPTIONS_CLIP : D2D1_DRAW_TEXT_OPTIONS_NONE;
    }
}
