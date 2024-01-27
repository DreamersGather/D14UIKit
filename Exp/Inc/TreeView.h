#pragma once

#include "ScrollView.h"
#include "WaterfallView.h"

namespace d14uikit
{
    class TreeViewItem;

    class DllExport TreeView : public ScrollView
    {
        _D14_UIKIT_PIMPL(TreeView)

        void insertRootItem(const std::list<TreeViewItem*>& items, int rootIndex = 0);
        void appendRootItem(const std::list<TreeViewItem*>& items);

        void removeRootItem(int rootIndex, int count = 1);
        void clearAllItems();

        int rootItemCount() const;

        _D14_UIKIT_WATERFALL_VIEW_DECL(TreeView)
    };
}
