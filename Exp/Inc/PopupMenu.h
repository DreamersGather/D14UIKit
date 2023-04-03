#pragma once

#include "ListView.h"

namespace d14uikit
{
    class MenuItem;

    class DllExport PopupMenu : public ScrollView
    {
        friend class ComboBox;

        _D14_UIKIT_PIMPL_DEF

        PopupMenu();

        int roundExtension() const;
        void setRoundExtension(int value);

        void insertItem(const std::list<MenuItem*>& items, int index = 0);
        void appendItem(const std::list<MenuItem*>& items);

        void removeItem(int index, int count = 1);
        void clearAllItems();

        struct Callback
        {
            std::function<void(PopupMenu*, const std::wstring&)> onTriggerMenuItem = {};
        };
        Callback& callback() const;

    protected:
        void initialize();
        explicit PopupMenu(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        virtual void onTriggerMenuItem(const std::wstring& text);
    };
}
