#include "Common/Precompile.h"

#include "TreeView.h"

#include "Panel.h"
#include "ResizablePanel.h"
#include "ScrollView.h"
#include "TreeViewItem.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/TreeView.h"

using namespace d14engine;

namespace d14uikit
{
    TreeView::TreeView() : TreeView(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        ScrollView::pimpl->uiobj =
        TreeView::pimpl->uiobj =
        uikit::makeUIObject<uikit::TreeView>();

        Panel::initialize();
        ResizablePanel::initialize();
        ScrollView::initialize();
        TreeView::initialize();
    }

    TreeView::TreeView(Passkey)
        :
        Panel(Panel::Passkey{}),
        ScrollView(ScrollView::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void TreeView::initialize() { }

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

    int TreeView::allItemCount() const
    {
        return (int)pimpl->uiobj->childrenItems().size();
    }

    int TreeView::rootItemCount() const
    {
        return (int)pimpl->uiobj->rootItems().size();
    }
}
