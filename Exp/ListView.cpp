#include "Common/Precompile.h"

#include "ListView.h"

#include "ListViewItem.h"
#include "WaterfallView.h"

#include "UIKit/ListView.h"

using namespace d14engine;

namespace d14uikit
{
    _D14_UIKIT_WATERFALL_VIEW_IMPL(ListView)

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
