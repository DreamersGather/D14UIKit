#include "Common/Precompile.h"

#include "TreeViewItem.h"

#include "Panel.h"
#include "ViewItem.h"

#include "UIKit/TreeViewItem.h"

using namespace d14engine;

namespace d14uikit
{
    TreeViewItem::TreeViewItem(const std::wstring& text)
        :
        TreeViewItem(Passkey{})
    {
        Panel::pimpl->uiobj =
        ViewItem::pimpl->uiobj =
        TreeViewItem::pimpl->uiobj =
        uikit::makeUIObject<uikit::TreeViewItem>(text);

        Panel::initialize();
        ViewItem::initialize();
        TreeViewItem::initialize();
    }

    TreeViewItem::TreeViewItem(Passkey)
        :
        ViewItem(ViewItem::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void TreeViewItem::initialize()
    {
        pimpl->uiobj->f_onStateChange = [this]
        (uikit::TreeViewItem::StatefulObject* obj,
         uikit::TreeViewItem::StatefulObject::Event& e)
        {
            State state = {};
            switch (e.flag)
            {
            case uikit::TreeViewItem::FOLDED: state = Folded; break;
            case uikit::TreeViewItem::UNFOLDED: state = Unfolded; break;
            default: break;
            }
            onStateChange(state);
            if (pcallback->onStateChange)
            {
                pcallback->onStateChange(this, state);
            }
        };
    }

    int TreeViewItem::nodeLevel() const
    {
        return (int)pimpl->uiobj->nodeLevel();
    }

    void TreeViewItem::insertItem(const std::list<TreeViewItem*>& items, int index)
    {
        uikit::TreeViewItem::ChildItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](TreeViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->insertItem(entities, index);
    }

    void TreeViewItem::appendItem(const std::list<TreeViewItem*>& items)
    {
        uikit::TreeViewItem::ChildItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](TreeViewItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->appendItem(entities);
    }

    void TreeViewItem::removeItem(int index, int count)
    {
        pimpl->uiobj->removeItem(index, count);
    }

    void TreeViewItem::clearAllItems()
    {
        pimpl->uiobj->clearAllItems();
    }

    int TreeViewItem::itemCount() const
    {
        return (int)pimpl->uiobj->childrenItems().size();
    }

    TreeViewItem::State TreeViewItem::state() const
    {
        return pimpl->state;
    }

    void TreeViewItem::setState(State state)
    {
        pimpl->state = state;
        uikit::TreeViewItemState::Flag flag = {};
        switch (state)
        {
        case Unfolded: flag = uikit::TreeViewItem::UNFOLDED; break;
        case Folded: flag = uikit::TreeViewItem::FOLDED; break;
        default: break;
        }
        pimpl->uiobj->setFolded(flag);
    }

    TreeViewItem::Callback& TreeViewItem::callback() const { return *pcallback; }

    void TreeViewItem::onStateChange(State state) { }
}
