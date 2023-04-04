#pragma once

#include "Common/Precompile.h"

#include "Font.h"

#include "Inc/Common.h"
#include "Inc/Font.h"

#include "Common/DirectXError.h"

#define _D14_UIKIT_TEXT_FORMAT_IMPL(Type_Name, Content)                                                               \
                                                                                                                      \
const Font& Type_Name::font() const { return pimpl->font; }                                                           \
                                                                                                                      \
void Type_Name::setFont(const Font& font)                                                                             \
{                                                                                                                     \
    pimpl->font = font;                                                                                               \
                                                                                                                      \
    const auto& content = Content;                                                                                    \
    content->label()->setTextFormat(font.getImpl()->textFormat.Get());                                                \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::wstring Type_Name::fontFamilyName() const                                                                        \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    auto length = content->label()->textLayout()->GetFontFamilyNameLength();                                          \
    std::wstring name(++length, L'\0');                                                                               \
    THROW_IF_FAILED(content->label()->textLayout()->GetFontFamilyName(name.data(), length));                          \
    return name;                                                                                                      \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontFamilyName(const std::wstring& name)                                                           \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetFontFamilyName(name.c_str(), textRange));                      \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
int Type_Name::fontSize() const                                                                                       \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return math_utils::round(content->label()->textLayout()->GetFontSize() * 72.0f / 96.0f);                          \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontSize(int value)                                                                                \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    content->label()->textLayout()->SetFontSize(value * 96.0f / 72.0f, textRange);                                    \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
std::wstring Type_Name::fontLocaleName() const                                                                        \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    auto length = content->label()->textLayout()->GetLocaleNameLength();                                              \
    std::wstring name(++length, L'\0');                                                                               \
    THROW_IF_FAILED(content->label()->textLayout()->GetLocaleName(name.data(), length));                              \
    return name;                                                                                                      \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontLocaleName(const std::wstring& name)                                                           \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetLocaleName(name.c_str(), textRange));                          \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
Font::Weight Type_Name::fontWeight() const                                                                            \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return (Font::Weight)content->label()->textLayout()->GetFontWeight();                                             \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontWeight(Font::Weight value)                                                                     \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetFontWeight((DWRITE_FONT_WEIGHT)value, textRange));             \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
Font::Style Type_Name::fontStyle() const                                                                              \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return (Font::Style)content->label()->textLayout()->GetFontStyle();                                               \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontStyle(Font::Style value)                                                                       \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetFontStyle((DWRITE_FONT_STYLE)value, textRange));               \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
Font::Stretch Type_Name::fontStretch() const                                                                          \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    return (Font::Stretch)content->label()->textLayout()->GetFontStretch();                                           \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setFontStretch(Font::Stretch value)                                                                   \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetFontStretch((DWRITE_FONT_STRETCH)value, textRange));           \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
bool Type_Name::underline(int offset) const                                                                           \
{                                                                                                                     \
    BOOL hasUnderline;                                                                                                \
    const auto& content = Content;                                                                                    \
    THROW_IF_FAILED(content->label()->textLayout()->GetUnderline(offset, &hasUnderline));                             \
    return hasUnderline;                                                                                              \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setUnderline(bool value)                                                                              \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetUnderline(value, textRange));                                  \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
                                                                                                                      \
bool Type_Name::strikethrough(int offset) const                                                                       \
{                                                                                                                     \
    BOOL hasStrikethrough;                                                                                            \
    const auto& content = Content;                                                                                    \
    THROW_IF_FAILED(content->label()->textLayout()->GetStrikethrough(offset, &hasStrikethrough));                     \
    return hasStrikethrough;                                                                                          \
}                                                                                                                     \
                                                                                                                      \
void Type_Name::setStrikethrough(bool value)                                                                          \
{                                                                                                                     \
    const auto& content = Content;                                                                                    \
    DWRITE_TEXT_RANGE textRange = { 0, (UINT32)content->label()->text().size() };                                     \
    THROW_IF_FAILED(content->label()->textLayout()->SetStrikethrough(value, textRange));                              \
                                                                                                                      \
    content->updateLayout();                                                                                          \
}                                                                                                                     \
