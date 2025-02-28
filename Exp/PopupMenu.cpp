#include "Common/Precompile.h"

#include "PopupMenu.h"

#include "Common.h"
#include "MenuItem.h"
#include "WaterfallView.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/PopupMenu.h"

using namespace d14engine;

namespace d14uikit
{
    _D14_UIKIT_WATERFALL_VIEW_RAWI(PopupMenu, MenuItem)

    PopupMenu::PopupMenu()
        :
        PopupMenu(uikit::makeRootUIObject<uikit::PopupMenu>()) { }

    _D14_UIKIT_CTOR(PopupMenu)
        :
        Panel(uiobj),
        ScrollView(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        using ViewType = uikit::PopupMenu::WaterfallView;
        using ItemType = ViewType::ItemIndexSet::value_type;

        pimpl->uiobj->f_onSelectChange = [this]
        (ViewType* v, const ViewType::ItemIndexSet& selected)
        {
            std::set<int> indices = {};
            std::transform(
                selected.begin(), selected.end(),
                std::inserter(indices, indices.begin()),
                [](const ItemType& s) { return (int)s.index; });

            onSelectChange(indices);
            if (pcallback->onSelectChange)
            {
                pcallback->onSelectChange(this, indices);
            }
        };
        // Some UI objects that use PopupMenu as a component
        // will use the callback to implement corresponding functionality.
        // Therefore, it is necessary to check here to avoid redefinition.
        if (!pimpl->uiobj->f_onTriggerMenuItem)
        {
        pimpl->uiobj->f_onTriggerMenuItem = [this]
        (uikit::PopupMenu* menu,
         uikit::PopupMenu::ItemIndexParam itemIndex)
        {
            auto content = (*itemIndex)->getContent<uikit::IconLabel>();
            auto& text = content.lock()->label()->text();
            onTriggerMenuItem(text);
            if (pcallback->onTriggerMenuItem)
            {
                pcallback->onTriggerMenuItem(this, text);
            }
        };}
    }

    void PopupMenu::insertItem(const std::list<MenuItem*>& items, int index)
    {
        uikit::PopupMenu::ItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](MenuItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->insertItem(entities, index);
    }

    void PopupMenu::appendItem(const std::list<MenuItem*>& items)
    {
        uikit::PopupMenu::ItemList entities = {};
        std::transform(items.begin(), items.end(), std::back_inserter(entities),
            [](MenuItem* elem) { return elem->getImpl()->uiobj; });

        pimpl->uiobj->appendItem(entities);
    }

    void PopupMenu::removeItem(int index, int count)
    {
        pimpl->uiobj->removeItem(index, count);
    }

    void PopupMenu::clearAllItems()
    {
        pimpl->uiobj->clearAllItems();
    }

    int PopupMenu::roundExtension() const
    {
        auto& geoSetting = pimpl->uiobj->getAppearance().geometry;
        return math_utils::round(geoSetting.extension); // and radius
    }

    void PopupMenu::setRoundExtension(int value)
    {
        auto& geoSetting = pimpl->uiobj->getAppearance().geometry;
        geoSetting.extension = geoSetting.roundRadius = (float)value;

        auto sz = pimpl->uiobj->extendedSize(pimpl->uiobj->size());
        pimpl->uiobj->shadow.loadBitmap(math_utils::roundu(sz));
    }

    void PopupMenu::setActivated(bool value)
    {
        pimpl->uiobj->setActivated(value);
    }

    void PopupMenu::setBkgnTriggerPanel(bool value)
    {
        pimpl->uiobj->setBackgroundTriggerPanel(value);
    }

    PopupMenu::Callback& PopupMenu::callback() const { return *pcallback; }

    void PopupMenu::onSelectChange(const std::set<int>& selected) {}

    void PopupMenu::onTriggerMenuItem(const std::wstring& text) { }
}
