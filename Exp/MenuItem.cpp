#include "Common/Precompile.h"

#include "MenuItem.h"

#include "Common.h"
#include "Label.h"
#include "PopupMenu.h"

#include "UIKit/IconLabel2.h"
#include "UIKit/MenuItem.h"

using namespace d14engine;

namespace d14uikit
{
    MenuItem::MenuItem(
        const std::wstring& labelText,
        const std::wstring& hotkeyText)
        :
        MenuItem(uikit::makeUIObject<uikit::MenuItem>(
            uikit::IconLabel2::menuItemLayout(labelText, hotkeyText))) { }

    _D14_UIKIT_CTOR(MenuItem)
        :
        ViewItem(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;

        auto content = pimpl->uiobj->getContent<uikit::IconLabel2>();
        if (!content.expired())
        {
            auto& hotkey = content.lock()->label2();
            pimpl->hotkey = std::shared_ptr<Label>(new Label(hotkey));
        }
    }

    Label* MenuItem::hotkey() const
    {
        return pimpl->hotkey.get();
    }

    PopupMenu* MenuItem::associatedMenu() const
    {
        return pimpl->associatedMenu;
    }

    void MenuItem::setAssociatedMenu(PopupMenu* menu)
    {
        pimpl->associatedMenu = menu;
        if (menu != nullptr)
        {
            pimpl->uiobj->setAssociatedMenu(menu->getImpl()->uiobj);
        }
        else pimpl->uiobj->setAssociatedMenu(nullptr);
    }

    bool MenuItem::instant() const
    {
        return pimpl->uiobj->isInstant;
    }

    void MenuItem::setInstant(bool value)
    {
        pimpl->uiobj->isInstant = value;
    }

    bool MenuItem::trigger() const
    {
        return pimpl->uiobj->isTriggerItem;
    }

    void MenuItem::setTrigger(bool value)
    {
        pimpl->uiobj->isTriggerItem = value;
    }
}
