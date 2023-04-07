#include "Common/Precompile.h"

#include "MenuSeparator.h"

#include "MenuItem.h"
#include "Panel.h"
#include "ViewItem.h"

#include "UIKit/MenuSeparator.h"

using namespace d14engine;

namespace d14uikit
{
    MenuSeparator::MenuSeparator()
        :
        MenuSeparator(Passkey{})
    {
        Panel::pimpl->uiobj =
        ViewItem::pimpl->uiobj =
        MenuItem::pimpl->uiobj =
        MenuSeparator::pimpl->uiobj =
        uikit::makeUIObject<uikit::MenuSeparator>();

        Panel::initialize();
        ViewItem::initialize();
        MenuItem::initialize();
        MenuSeparator::initialize();
    }

    MenuSeparator::MenuSeparator(Passkey)
        :
        MenuItem(MenuItem::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }


    void MenuSeparator::initialize()
        { setHeight(11); } // better odd height
}
