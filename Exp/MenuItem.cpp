#include "Common/Precompile.h"

#include "MenuItem.h"

#include "Panel.h"
#include "PopupMenu.h"
#include "TextFormat.h"
#include "ViewItem.h"

#include "UIKit/IconLabel2.h"
#include "UIKit/Label.h"
#include "UIKit/MenuItem.h"
#include "UIKit/ResourceUtils.h"

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

    // Some UIObject types inherited from MenuItem
    // may not create the content as IconLabel2.

    const std::wstring& MenuItem::hotkeyText() const
    {
        auto content = pimpl->uiobj->getContent<uikit::IconLabel2>();
        if (!content.expired())
        {
            return content.lock()->label2()->text();
        }
        else return uikit::resource_utils::emptyWstrRef();
    }

    void MenuItem::setHotkeyText(const std::wstring& text)
    {
        auto content = pimpl->uiobj->getContent<uikit::IconLabel2>();
        if (!content.expired())
        {
            content.lock()->label2()->setText(text);
        }
    }

    void MenuItem::syncLabelHotkeyTextFormat()
    {
        auto content = pimpl->uiobj->getContent<uikit::IconLabel2>();
        if (!content.expired())
        {
            auto pcontent = content.lock();
            // The hotkey label should always keep horizontal right alignment.
            auto orgHorzAlign = pcontent->label2()->textLayout()->GetTextAlignment();

            pcontent->label2()->copyTextStyle(pcontent->label().get());
            pcontent->label2()->textLayout()->SetTextAlignment(orgHorzAlign);
        }
    }
}
