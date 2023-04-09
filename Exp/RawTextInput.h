#pragma once

#include "Common/Precompile.h"

#include "Common.h"
#include "Label.h"
#include "LabelArea.h"
#include "Panel.h"

#include "UIKit/Label.h"

#define _D14_UIKIT_RAW_TEXT_INPUT_IMPL(Type_Name)                                       \
                                                                                        \
Type_Name::Type_Name()                                                                  \
    :                                                                                   \
    Type_Name(Passkey{})                                                                \
{                                                                                       \
    Panel::pimpl->uiobj =                                                               \
    Label::pimpl->uiobj =                                                               \
    LabelArea::pimpl->uiobj =                                                           \
    Type_Name::pimpl->uiobj =                                                           \
    uikit::makeUIObject<uikit::Type_Name>();                                            \
                                                                                        \
    Panel::initialize();                                                                \
    Label::initialize();                                                                \
    LabelArea::initialize();                                                            \
    Type_Name::initialize();                                                            \
}                                                                                       \
                                                                                        \
Type_Name::Type_Name(Passkey)                                                           \
    :                                                                                   \
    LabelArea(LabelArea::Passkey{}),                                                    \
    pimpl(std::make_shared<Impl>()),                                                    \
    pcallback(std::make_unique<Callback>()) { }                                         \
                                                                                        \
void Type_Name::initialize()                                                            \
{                                                                                       \
    pimpl->uiobj->f_onTextChange = [this]                                               \
    (uikit::Type_Name::TextInputObject* obj, WstrParam text)                            \
    {                                                                                   \
        onTextChange(text);                                                             \
        if (pcallback->onTextChange)                                                    \
        {                                                                               \
            pcallback->onTextChange(this, text);                                        \
        }                                                                               \
    };                                                                                  \
    /* Bind the existing label implementation to the interface. */                      \
    {                                                                                   \
        pimpl->placeholder = std::shared_ptr<Label>(new Label(Label::Passkey{}));       \
                                                                                        \
        pimpl->placeholder->Panel::pimpl->uiobj =                                       \
        pimpl->placeholder->Label::pimpl->uiobj =                                       \
        pimpl->uiobj->placeholder();                                                    \
                                                                                        \
        pimpl->placeholder->Panel::initialize();                                        \
        pimpl->placeholder->Label::initialize();                                        \
    }                                                                                   \
}                                                                                       \
                                                                                        \
Rect Type_Name::textRect() const                                                        \
{                                                                                       \
    return convert(pimpl->uiobj->visibleTextRect());                                    \
}                                                                                       \
                                                                                        \
void Type_Name::setTextRect(const Rect& value)                                          \
{                                                                                       \
    pimpl->uiobj->setVisibleTextRect(convert(value));                                   \
}                                                                                       \
                                                                                        \
Label* Type_Name::placeholder() const                                                   \
{                                                                                       \
    return pimpl->placeholder.get();                                                    \
}                                                                                       \
                                                                                        \
void Type_Name::performCommandCtrlX()                                                   \
{                                                                                       \
    pimpl->uiobj->performCommandCtrlX();                                                \
}                                                                                       \
                                                                                        \
void Type_Name::performCommandCtrlV()                                                   \
{                                                                                       \
    pimpl->uiobj->performCommandCtrlV();                                                \
}                                                                                       \
                                                                                        \
Type_Name::Callback& Type_Name::callback() const { return *pcallback; }                 \
                                                                                        \
void Type_Name::onTextChange(const std::wstring& text) { }                              \
