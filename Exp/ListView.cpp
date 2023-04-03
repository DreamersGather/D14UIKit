#include "Common/Precompile.h"

#include "ListView.h"

#include "ListViewItem.h"
#include "Panel.h"
#include "ResizablePanel.h"
#include "ScrollView.h"

#include "UIKit/ListView.h"

using namespace d14engine;

namespace d14uikit
{
    ListView::ListView() : ListView(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        ScrollView::pimpl->uiobj =
        ListView::pimpl->uiobj =
        uikit::makeUIObject<uikit::ListView>();

        Panel::initialize();
        ResizablePanel::initialize();
        ScrollView::initialize();
        ListView::initialize();
    }

    ListView::ListView(Passkey)
        :
        Panel(Panel::Passkey{}),
        ScrollView(ScrollView::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void ListView::initialize() { }

    void ListView::insertItem(const std::list<ListViewItem*>& items, int index)
    {
        uikit::ListView::ItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](ListViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->insertItem(entities, index);
    }

    void ListView::appendItem(const std::list<ListViewItem*>& items)
    {
        uikit::ListView::ItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](ListViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->appendItem(entities);
    }

    void ListView::removeItem(int index, int count)
    {
        pimpl->uiobj->removeItem(index, count);
    }

    void ListView::clearAllItems()
    {
        pimpl->uiobj->clearAllItems();
    }
}
