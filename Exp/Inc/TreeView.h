#pragma once

#include "ScrollView.h"
#include "WaterfallView.h"

_D14_UIKIT_FWDEF(TreeView)

namespace d14uikit
{
    class TreeViewItem;

    class DllExport TreeView : public ScrollView
    {
        _D14_UIKIT_PIMPL(TreeView)

        int rootItemCount() const;
        // The return value may be null
        TreeViewItem* getRootItem(int rootIndex) const;

        void insertRootItem(const std::list<TreeViewItem*>& rootItems, int rootIndex = 0);
        void appendRootItem(const std::list<TreeViewItem*>& rootItems);

        void removeRootItem(int rootIndex, int count = 1);
        void clearAllItems();

        _D14_UIKIT_WATERFALL_VIEW_DECL(TreeView, TreeViewItem)
    };
}
