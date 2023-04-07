#include "Common/Precompile.h"

#include "PopupMenu.h"

#include "MenuItem.h"
#include "Panel.h"
#include "ResizablePanel.h"
#include "ScrollView.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/PopupMenu.h"

using namespace d14engine;

namespace d14uikit
{
    PopupMenu::PopupMenu() : PopupMenu(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        ScrollView::pimpl->uiobj =
        PopupMenu::pimpl->uiobj =
        uikit::makeRootUIObject<uikit::PopupMenu>();

        Panel::initialize();
        ResizablePanel::initialize();
        ScrollView::initialize();
        PopupMenu::initialize();
    }

    PopupMenu::PopupMenu(Passkey)
        :
        ScrollView(ScrollView::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void PopupMenu::initialize()
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
        };
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
        pimpl->uiobj->shadow.loadShadowBitmap(math_utils::roundu(sz));
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

    void PopupMenu::setActivated(bool value)
    {
        pimpl->uiobj->setActivated(value);
    }

    void PopupMenu::setBkgnTriggerPanel(bool value)
    {
        pimpl->uiobj->setBackgroundTriggerPanel(value);
    }

    PopupMenu::Callback& PopupMenu::callback() const { return *pcallback; }

    void PopupMenu::onTriggerMenuItem(const std::wstring& text) { }
}
