#pragma once

#include "Common/Precompile.h"

#include "RawTextInput.h"

#include "Common.h"

#include "UIKit/RawTextInput.h"

using namespace d14engine;

namespace d14uikit
{
    RawTextInput::RawTextInput(bool multiline)
        :
        RawTextInput(uikit::makeUIObject<uikit::RawTextInput>(multiline)) { }

    _D14_UIKIT_CTOR(RawTextInput)
        :
        LabelArea(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        pimpl->uiobj->f_onTextChange = [this]
        (uikit::RawTextInput::TextInputObject* obj, WstrParam text)
        {
            onTextChange(text);
            if (pcallback->onTextChange)
            {
                pcallback->onTextChange(this, text);
            }
        };
        _D14_UIKIT_BIND(Label, placeholder);
    }

    bool RawTextInput::editable() const
    {
        return pimpl->uiobj->editable;
    }

    void RawTextInput::setEditable(bool value)
    {
        pimpl->uiobj->editable = value;
    }

    Rect RawTextInput::textRect() const
    {
        return convert(pimpl->uiobj->visibleTextRect());
    }

    void RawTextInput::setTextRect(const Rect& value)
    {
        pimpl->uiobj->setVisibleTextRect(convert(value));
    }

    Label* RawTextInput::placeholder() const
    {
        return pimpl->placeholder.get();
    }

    void RawTextInput::performCommandCtrlX()
    {
        pimpl->uiobj->performCommandCtrlX();
    }

    void RawTextInput::performCommandCtrlV()
    {
        pimpl->uiobj->performCommandCtrlV();
    }

    RawTextInput::Callback& RawTextInput::callback() const { return *pcallback; }

    void RawTextInput::onTextChange(const std::wstring& text) {}
}
