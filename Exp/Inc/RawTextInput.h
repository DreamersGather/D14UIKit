#pragma once

#include "BasicType.h"
#include "Common.h"

namespace d14uikit { class Label; }

#define _D14_UIKIT_RAW_TEXT_INPUT_DECL(Type_Name)                                       \
                                                                                        \
Type_Name();                                                                            \
                                                                                        \
bool editable() const;                                                                  \
void setEditable(bool value);                                                           \
                                                                                        \
Rect textRect() const;                                                                  \
void setTextRect(const Rect& value);                                                    \
                                                                                        \
Label* placeholder() const;                                                             \
                                                                                        \
void performCommandCtrlX();                                                             \
void performCommandCtrlV();                                                             \
                                                                                        \
struct Callback                                                                         \
{                                                                                       \
    std::function<void(Type_Name*, const std::wstring&)> onTextChange = {};             \
};                                                                                      \
Callback& callback() const;                                                             \
                                                                                        \
protected:                                                                              \
                                                                                        \
std::unique_ptr<Callback> pcallback = {};                                               \
                                                                                        \
virtual void onTextChange(const std::wstring& text);                                    \
