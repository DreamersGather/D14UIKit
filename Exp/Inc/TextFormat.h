#pragma once

#include "Common.h"
#include "Font.h"

// Version 1: Content must be valid, and returns concrete value.

#define _D14_UIKIT_TEXT_FORMAT_DECL_CONCRETE(Suffix)                \
                                                                    \
const Font& font##Suffix() const;                                   \
void setFont##Suffix(const Font& font);                             \
                                                                    \
std::wstring fontFamilyName##Suffix() const;                        \
void setFontFamilyName##Suffix(const std::wstring& name);           \
                                                                    \
int fontSize##Suffix() const;                                       \
void setFontSize##Suffix(int value);                                \
                                                                    \
std::wstring fontLocaleName##Suffix() const;                        \
void setFontLocaleName##Suffix(const std::wstring& name);           \
                                                                    \
Font::Weight fontWeight##Suffix() const;                            \
void setFontWeight##Suffix(Font::Weight value);                     \
                                                                    \
Font::Style fontStyle##Suffix() const;                              \
void setFontStyle##Suffix(Font::Style value);                       \
                                                                    \
Font::Stretch fontStretch##Suffix() const;                          \
void setFontStretch##Suffix(Font::Stretch value);                   \
                                                                    \
bool underline##Suffix() const;                                     \
void setUnderline##Suffix(bool value);                              \
                                                                    \
bool strikethrough##Suffix() const;                                 \
void setStrikethrough##Suffix(bool value);                          \

// Version 2: Content can be nullptr, and returns optional value.

#define _D14_UIKIT_TEXT_FORMAT_DECL_OPTIONAL(Suffix)                \
                                                                    \
const Font& font##Suffix() const;                                   \
void setFont##Suffix(const Font& font);                             \
                                                                    \
std::optional<std::wstring> fontFamilyName##Suffix() const;         \
void setFontFamilyName##Suffix(const std::wstring& name);           \
                                                                    \
std::optional<int> fontSize##Suffix() const;                        \
void setFontSize##Suffix(int value);                                \
                                                                    \
std::optional<std::wstring> fontLocaleName##Suffix() const;         \
void setFontLocaleName##Suffix(const std::wstring& name);           \
                                                                    \
std::optional<Font::Weight> fontWeight##Suffix() const;             \
void setFontWeight##Suffix(Font::Weight value);                     \
                                                                    \
std::optional<Font::Style> fontStyle##Suffix() const;               \
void setFontStyle##Suffix(Font::Style value);                       \
                                                                    \
std::optional<Font::Stretch> fontStretch##Suffix() const;           \
void setFontStretch##Suffix(Font::Stretch value);                   \
                                                                    \
std::optional<bool> underline##Suffix() const;                      \
void setUnderline##Suffix(bool value);                              \
                                                                    \
std::optional<bool> strikethrough##Suffix() const;                  \
void setStrikethrough##Suffix(bool value);                          \
