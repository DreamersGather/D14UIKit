#include "Common/Precompile.h"

#include "Cursor.h"

#include "Panel.h"

#include "UIKit/Cursor.h"

using namespace d14engine;

namespace d14uikit
{
    Cursor::Cursor() : Cursor(Passkey{})
    {
        Panel::pimpl->uiobj =
        Cursor::pimpl->uiobj =
        uikit::makeRootUIObject<uikit::Cursor>();

        Panel::initialize();
        Cursor::initialize();
    }

    Cursor::Cursor(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void Cursor::initialize() { }

    bool Cursor::useSystemIcons() const
    {
        return pimpl->uiobj->useSystemIcons;
    }

    void Cursor::setUseSystemIcons(bool value)
    {
        pimpl->uiobj->useSystemIcons = value;
    }

    void Cursor::setIcon(StaticIconIndex index)
    {
        pimpl->uiobj->setIcon(
            uikit::Cursor::StaticIconIndex(index));
    }

    void Cursor::setIcon(DynamicIconIndex index)
    {
        pimpl->uiobj->setIcon(
            uikit::Cursor::DynamicIconIndex(index));
    }
}
