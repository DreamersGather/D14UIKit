#include "Common/Precompile.h"

#include "TreeView.h"

#include "TreeViewItem.h"
#include "WaterfallView.h"

#include "UIKit/TreeView.h"

using namespace d14engine;

namespace d14uikit
{
    _D14_UIKIT_WATERFALL_VIEW_IMPL(TreeView)

    void TreeView::insertRootItem(const std::list<TreeViewItem*>& items, int rootIndex)
    {
        uikit::TreeView::ItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](TreeViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->insertRootItem(entities, rootIndex);
    }

    void TreeView::appendRootItem(const std::list<TreeViewItem*>& items)
    {
        uikit::TreeView::ItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](TreeViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->appendRootItem(entities);
    }

    void TreeView::removeRootItem(int rootIndex, int count)
    {
        pimpl->uiobj->removeRootItem(rootIndex, count);
    }

    void TreeView::clearAllItems()
    {
        pimpl->uiobj->clearAllItems();
    }

    int TreeView::rootItemCount() const
    {
        return (int)pimpl->uiobj->rootItems().size();
    }
}
