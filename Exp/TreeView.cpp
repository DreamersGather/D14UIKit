#include "Common/Precompile.h"

#include "TreeView.h"

#include "TreeViewItem.h"
#include "WaterfallView.h"

#include "UIKit/TreeView.h"

using namespace d14engine;

namespace d14uikit
{
    _D14_UIKIT_WATERFALL_VIEW_IMPL(TreeView, TreeViewItem)

    int TreeView::rootItemCount() const
    {
        return (int)pimpl->uiobj->rootItems().size();
    }

    TreeViewItem* TreeView::getRootItem(int rootIndex) const
    {
        auto& rootItems = pimpl->uiobj->rootItems();
        if (0 <= rootIndex && rootIndex < (int)rootItems.size())
        {
            auto rootItem = std::next(rootItems.begin(), rootIndex);
            pimpl->rootItem = std::shared_ptr<TreeViewItem>(new TreeViewItem(*rootItem));
        }
        else pimpl->rootItem.reset();

        return pimpl->rootItem.get();
    }

    void TreeView::insertRootItem(const std::list<TreeViewItem*>& rootItems, int rootIndex)
    {
        uikit::TreeView::ItemList entities = {};
        std::transform(rootItems.begin(), rootItems.end(), std::back_inserter(entities),
            [](TreeViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->insertRootItem(entities, rootIndex);
    }

    void TreeView::appendRootItem(const std::list<TreeViewItem*>& rootItems)
    {
        uikit::TreeView::ItemList entities = {};
        std::transform(rootItems.begin(), rootItems.end(), std::back_inserter(entities),
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
}
