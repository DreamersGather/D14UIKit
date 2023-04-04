#pragma once

#include "FlatButton.h"

namespace d14uikit
{
    class PopupMenu;

    class DllExport ComboBox : public FlatButton
    {
        _D14_UIKIT_PIMPL(ComboBox)

        ComboBox();

        PopupMenu* dropDownMenu() const;

        void setCurrSelected(int index);
        const std::wstring& currSelectedText() const;

        struct Callback
        {
            std::function<void(ComboBox*, const std::wstring&)> onSelectedChange = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onSelectedChange(const std::wstring& text);
    };
}
