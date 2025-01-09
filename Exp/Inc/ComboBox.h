#pragma once

#include "FlatButton.h"

_D14_UIKIT_FWDEF(ComboBox)

namespace d14uikit
{
    class MenuItem;
    class PopupMenu;

    class DllExport ComboBox : public FlatButton
    {
        _D14_UIKIT_PIMPL(ComboBox)

        ComboBox();

        // The return value may be null
        MenuItem* currSelected() const;
        void setCurrSelected(int index);

        // Use wstring_view to avoid the overhead of copying strings
        std::optional<std::wstring_view> currSelectedText() const;

        PopupMenu* dropDownMenu() const;

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
