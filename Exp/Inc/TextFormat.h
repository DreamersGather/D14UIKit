#pragma once

#include "Common.h"
#include "Font.h"

#define _D14_UIKIT_TEXT_FORMAT_DECL                       \
                                                          \
const Font& font() const;                                 \
void setFont(const Font& font);                           \
                                                          \
std::wstring fontFamilyName() const;                      \
void setFontFamilyName(const std::wstring& name);         \
                                                          \
int fontSize() const;                                     \
void setFontSize(int value);                              \
                                                          \
std::wstring fontLocaleName() const;                      \
void setFontLocaleName(const std::wstring& name);         \
                                                          \
Font::Weight fontWeight() const;                          \
void setFontWeight(Font::Weight value);                   \
                                                          \
Font::Style fontStyle() const;                            \
void setFontStyle(Font::Style value);                     \
                                                          \
Font::Stretch fontStretch() const;                        \
void setFontStretch(Font::Stretch value);                 \
                                                          \
bool underline(int offset) const;                         \
void setUnderline(bool value);                            \
                                                          \
bool strikethrough(int offset) const;                     \
void setStrikethrough(bool value);                        \
