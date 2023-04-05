#pragma once

#include "Common/Precompile.h"

#include "Font.h"

#include "Inc/Common.h"
#include "Inc/Font.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/Basic.h"

// Version 1: Content must be valid, and returns concrete value.

#define _D14_UIKIT_TEXT_FORMAT_IMPL_CONCRETE(Type_Name, Suffix, Label, Content)                                       \
                                                                                                                      \
const Font& Type_Name::font##Suffix() const { return pimpl->font; }                                                   \
                                                                                                                      \
void Type_Name::setFont##Suffix(const Font& font)                                                                     \
{                                                                                                                     \
    pimpl->font = font;                                                                                               \
                                                                                                                      \
    const auto& content = Content;                                                                                    \
    content->Label()->setTextFormat(font.getImpl()->textFormat.Get());                                                \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::wstring Type_Name::fontFamilyName##Suffix() const                                                                \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    auto length = content->Label()->textLayout()->GetFontFamilyNameLength();                                          \
    std::wstring name(++length, L'\0');                                                                               \
    THROW_IF_FAILED(content->Label()->textLayout()->GetFontFamilyName(name.data(), length));                          \
    return name;                                                                                                      \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontFamilyName##Suffix(const std::wstring& name)                                                   \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontFamilyName(name.c_str(), textRange));                      \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
int Type_Name::fontSize##Suffix() const                                                                               \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return math_utils::round(content->Label()->textLayout()->GetFontSize() * 72.0f / 96.0f);                          \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontSize##Suffix(int value)                                                                        \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    content->Label()->textLayout()->SetFontSize(value * 96.0f / 72.0f, textRange);                                    \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::wstring Type_Name::fontLocaleName##Suffix() const                                                                \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    auto length = content->Label()->textLayout()->GetLocaleNameLength();                                              \
    std::wstring name(++length, L'\0');                                                                               \
    THROW_IF_FAILED(content->Label()->textLayout()->GetLocaleName(name.data(), length));                              \
    return name;                                                                                                      \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontLocaleName##Suffix(const std::wstring& name)                                                   \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetLocaleName(name.c_str(), textRange));                          \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
Font::Weight Type_Name::fontWeight##Suffix() const                                                                    \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return (Font::Weight)content->Label()->textLayout()->GetFontWeight();                                             \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontWeight##Suffix(Font::Weight value)                                                             \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontWeight((DWRITE_FONT_WEIGHT)value, textRange));             \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
Font::Style Type_Name::fontStyle##Suffix() const                                                                      \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return (Font::Style)content->Label()->textLayout()->GetFontStyle();                                               \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontStyle##Suffix(Font::Style value)                                                               \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontStyle((DWRITE_FONT_STYLE)value, textRange));               \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
Font::Stretch Type_Name::fontStretch##Suffix() const                                                                  \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return (Font::Stretch)content->Label()->textLayout()->GetFontStretch();                                           \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontStretch##Suffix(Font::Stretch value)                                                           \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontStretch((DWRITE_FONT_STRETCH)value, textRange));           \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
bool Type_Name::underline##Suffix() const                                                                             \
{                                                                                                                     \
    BOOL hasUnderline;                                                                                                \
    const auto& content = Content;                                                                                    \
    THROW_IF_FAILED(content->Label()->textLayout()->GetUnderline(0, &hasUnderline));                                  \
    return hasUnderline;                                                                                              \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setUnderline##Suffix(bool value)                                                                      \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetUnderline(value, textRange));                                  \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
bool Type_Name::strikethrough##Suffix() const                                                                         \
{                                                                                                                     \
    BOOL hasStrikethrough;                                                                                            \
    const auto& content = Content;                                                                                    \
    THROW_IF_FAILED(content->Label()->textLayout()->GetStrikethrough(0, &hasStrikethrough));                          \
    return hasStrikethrough;                                                                                          \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setStrikethrough##Suffix(bool value)                                                                  \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetStrikethrough(value, textRange));                              \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \

// Version 2: Content can be nullptr, and returns optional value.

#define _D14_UIKIT_TEXT_FORMAT_IMPL_OPTIONAL(Type_Name, Suffix, Label, Content)                                       \
                                                                                                                      \
const Font& Type_Name::font##Suffix() const { return pimpl->font; }                                                   \
                                                                                                                      \
void Type_Name::setFont##Suffix(const Font& font)                                                                     \
{                                                                                                                     \
    pimpl->font = font;                                                                                               \
                                                                                                                      \
    const auto& content = Content; if (!content) return;                                                              \
    content->Label()->setTextFormat(font.getImpl()->textFormat.Get());                                                \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<std::wstring> Type_Name::fontFamilyName##Suffix() const                                                 \
{                                                                                                                     \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    auto length = content->Label()->textLayout()->GetFontFamilyNameLength();                                          \
    std::wstring name(++length, L'\0');                                                                               \
    THROW_IF_FAILED(content->Label()->textLayout()->GetFontFamilyName(name.data(), length));                          \
    return name;                                                                                                      \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontFamilyName##Suffix(const std::wstring& name)                                                   \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontFamilyName(name.c_str(), textRange));                      \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<int> Type_Name::fontSize##Suffix() const                                                                \
{                                                                                                                     \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    return math_utils::round(content->Label()->textLayout()->GetFontSize() * 72.0f / 96.0f);                          \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontSize##Suffix(int value)                                                                        \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    content->Label()->textLayout()->SetFontSize(value * 96.0f / 72.0f, textRange);                                    \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<std::wstring> Type_Name::fontLocaleName##Suffix() const                                                 \
{                                                                                                                     \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    auto length = content->Label()->textLayout()->GetLocaleNameLength();                                              \
    std::wstring name(++length, L'\0');                                                                               \
    THROW_IF_FAILED(content->Label()->textLayout()->GetLocaleName(name.data(), length));                              \
    return name;                                                                                                      \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontLocaleName##Suffix(const std::wstring& name)                                                   \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetLocaleName(name.c_str(), textRange));                          \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<Font::Weight> Type_Name::fontWeight##Suffix() const                                                     \
{                                                                                                                     \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    return (Font::Weight)content->Label()->textLayout()->GetFontWeight();                                             \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontWeight##Suffix(Font::Weight value)                                                             \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontWeight((DWRITE_FONT_WEIGHT)value, textRange));             \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<Font::Style> Type_Name::fontStyle##Suffix() const                                                       \
{                                                                                                                     \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    return (Font::Style)content->Label()->textLayout()->GetFontStyle();                                               \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontStyle##Suffix(Font::Style value)                                                               \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontStyle((DWRITE_FONT_STYLE)value, textRange));               \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<Font::Stretch> Type_Name::fontStretch##Suffix() const                                                   \
{                                                                                                                     \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    return (Font::Stretch)content->Label()->textLayout()->GetFontStretch();                                           \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontStretch##Suffix(Font::Stretch value)                                                           \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetFontStretch((DWRITE_FONT_STRETCH)value, textRange));           \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<bool> Type_Name::underline##Suffix() const                                                              \
{                                                                                                                     \
    BOOL hasUnderline;                                                                                                \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    THROW_IF_FAILED(content->Label()->textLayout()->GetUnderline(0, &hasUnderline));                                  \
    return hasUnderline;                                                                                              \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setUnderline##Suffix(bool value)                                                                      \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetUnderline(value, textRange));                                  \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::optional<bool> Type_Name::strikethrough##Suffix() const                                                          \
{                                                                                                                     \
    BOOL hasStrikethrough;                                                                                            \
    const auto& content = Content; if (!content) return std::nullopt;                                                 \
    THROW_IF_FAILED(content->Label()->textLayout()->GetStrikethrough(0, &hasStrikethrough));                          \
    return hasStrikethrough;                                                                                          \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setStrikethrough##Suffix(bool value)                                                                  \
{                                                                                                                     \
    const auto& content = Content; if (!content) return;                                                              \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->Label()->text().size() };                                     \
    THROW_IF_FAILED(content->Label()->textLayout()->SetStrikethrough(value, textRange));                              \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
