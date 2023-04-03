#include "Common/Precompile.h"

#include "MenuItem.h"

#include "Panel.h"
#include "PopupMenu.h"
#include "ViewItem.h"

#include "UIKit/IconLabel2.h"
#include "UIKit/Label.h"
#include "UIKit/MenuItem.h"

using namespace d14engine;

namespace d14uikit
{
    MenuItem::MenuItem(
        const std::wstring& labelText,
        const std::wstring& hotkeyText)
        :
        MenuItem(Passkey{})
    {
        Panel::pimpl->uiobj =
        ViewItem::pimpl->uiobj =
        MenuItem::pimpl->uiobj =
        uikit::makeUIObject<uikit::MenuItem>(
        uikit::IconLabel2::menuItemLayout(labelText, hotkeyText));

        Panel::initialize();
        ViewItem::initialize();
        MenuItem::initialize();
    }

    MenuItem::MenuItem(Passkey)
        :
        ViewItem(ViewItem::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MenuItem::initialize()
    {
        setHeight(40);
    }

    const std::wstring& MenuItem::hotkeyText() const
    {
        return pimpl->uiobj->getContent<uikit::IconLabel2>().lock()->label2()->text();
    }

    void MenuItem::setHotkeyText(const std::wstring& text)
    {
        pimpl->uiobj->getContent<uikit::IconLabel2>().lock()->label2()->setText(text);
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
}
