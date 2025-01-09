#pragma once

#include "Font.h"
#include "Panel.h"

_D14_UIKIT_FWDEF(Label)

namespace d14uikit
{
    class DllExport Label : public Panel
    {
        friend class HorzSlider;
        friend class IconLabel;
        friend class MenuItem;
        friend class RawTextInput;
        friend class VertSlider;

        _D14_UIKIT_PIMPL(Label)

        explicit Label(const std::wstring& text = {});

        Color frgnColor() const;
        void setFrgnColor(const Color& value);

        float frgnOpacity() const;
        void setFrgnOpacity(float value);

        Color bkgnColor() const;
        void setBkgnColor(const Color& value);

        float bkgnOpacity() const;
        void setBkgnOpacity(float value);

        const std::wstring& text() const;
        void setText(const std::wstring& text);

        DrawTextOption drawTextOption() const;
        void setDrawTextOption(DrawTextOption option);

        // null for default font
        Font* font() const;
        void setFont(Font* font);

        enum class HorzAlign
        {
            Left = 0,
            Right = 1,
            Center = 2,
            Justified = 3
        };
        enum class VertAlign
        {
            Top = 0,
            Bottom = 1,
            Center = 2
        };

#define CONST_ENUM constexpr static auto

        CONST_ENUM Left = HorzAlign::Left;
        CONST_ENUM Right = HorzAlign::Right;
        CONST_ENUM HCenter = HorzAlign::Center;
        CONST_ENUM Justified = HorzAlign::Justified;
        CONST_ENUM Top = VertAlign::Top;
        CONST_ENUM Bottom = VertAlign::Bottom;
        CONST_ENUM VCenter = VertAlign::Center;

#undef CONST_ENUM

        HorzAlign horzAlign() const;
        void setHorzAlign(HorzAlign value);

        bool horzHardAlign() const;
        void setHorzHardAlign(bool value);

        VertAlign vertAlign() const;
        void setVertAlign(VertAlign value);

        bool vertHardAlign() const;
        void setVertHardAlign(bool value);

        enum class WordWrapping
        {
            Wrap = 0,
            NoWrap = 1,
            EmergencyBreak = 2,
            WholeWord = 3,
            Character = 4
        };
        WordWrapping wordWrapping() const;
        void setWordWrapping(WordWrapping value);

#define SET_WORD_WRAPPING(Name) constexpr static auto Name = WordWrapping::Name;

        SET_WORD_WRAPPING(Wrap)
        SET_WORD_WRAPPING(NoWrap)
        SET_WORD_WRAPPING(EmergencyBreak)
        SET_WORD_WRAPPING(WholeWord)
        SET_WORD_WRAPPING(Character)

#undef SET_WORD_WRAPPING

        // It is worth noting that the following methods only apply to the current text layout.
        // Since `setText` recreates the text layout, these properties will be reset
        // once the text content changes. To permanently set certain properties, use `setFont`,
        // because it changes the default properties used to create the text layout.

        std::wstring fontFamilyName() const;
        void setFontFamilyName(const std::wstring& name, const std::optional<Range>& range = std::nullopt);

        int fontSize() const;
        void setFontSize(int value, const std::optional<Range>& range = std::nullopt);

        std::wstring fontLocaleName() const;
        void setFontLocaleName(const std::wstring& name, const std::optional<Range>& range = std::nullopt);

        Font::Weight fontWeight() const;
        void setFontWeight(Font::Weight value, const std::optional<Range>& range = std::nullopt);

        Font::Style fontStyle() const;
        void setFontStyle(Font::Style value, const std::optional<Range>& range = std::nullopt);

        Font::Stretch fontStretch() const;
        void setFontStretch(Font::Stretch value, const std::optional<Range>& range = std::nullopt);

        bool underline(int offset) const;
        void setUnderline(bool value, const std::optional<Range>& range = std::nullopt);

        bool strikethrough(int offset) const;
        void setStrikethrough(bool value, const std::optional<Range>& range = std::nullopt);
    };
}
