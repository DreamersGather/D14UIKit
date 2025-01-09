#pragma once

#include "LabelArea.h"

_D14_UIKIT_FWDEF(RawTextInput)

namespace d14uikit
{
    class DllExport RawTextInput : public LabelArea
    {
        _D14_UIKIT_PIMPL(RawTextInput)

        explicit RawTextInput(bool multiline);

        bool editable() const;
        void setEditable(bool value);

        Rect textRect() const;
        void setTextRect(const Rect& value);

        Label* placeholder() const;

        void performCommandCtrlX();
        void performCommandCtrlV();

        struct Callback
        {
            std::function<void(RawTextInput*, const std::wstring&)> onTextChange = {};
        };
        Callback& callback() const;

    protected:

        std::unique_ptr<Callback> pcallback = {};

        virtual void onTextChange(const std::wstring& text);
    };
}
