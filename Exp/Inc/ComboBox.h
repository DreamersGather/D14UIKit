#pragma once

#include "FlatButton.h"

namespace d14uikit
{
    class PopupMenu;

    class DllExport ComboBox : public FlatButton
    {
        _D14_UIKIT_PIMPL_DEF

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
        void initialize();
        explicit ComboBox(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        void onSelectedChange(const std::wstring& text);
    };
}
