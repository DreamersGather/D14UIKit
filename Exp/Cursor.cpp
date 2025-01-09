#include "Common/Precompile.h"

#include "Cursor.h"

#include "Common.h"

#include "UIKit/Cursor.h"

using namespace d14engine;

namespace d14uikit
{
    Cursor::Cursor()
        :
        Cursor(uikit::makeRootUIObject<uikit::Cursor>()) { }

    _D14_UIKIT_CTOR(Cursor)
        :
        Panel(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

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
        pimpl->uiobj->setIcon((uikit::Cursor::StaticIconIndex)index);
    }

    void Cursor::setIcon(DynamicIconIndex index)
    {
        pimpl->uiobj->setIcon((uikit::Cursor::DynamicIconIndex)index);
    }
}
