#pragma once

#include "ScrollView.h"

namespace d14uikit
{
    class ListViewItem;

    class DllExport ListView : public ScrollView
    {
        _D14_UIKIT_PIMPL_DEF

        ListView();

        void insertItem(const std::list<ListViewItem*>& items, int index = 0);
        void appendItem(const std::list<ListViewItem*>& items);

        void removeItem(int index, int count = 1);
        void clearAllItems();

    protected:
        void initialize();
        explicit ListView(Passkey);
    };
}
