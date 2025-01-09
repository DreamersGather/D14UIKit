#include "Common/Precompile.h"

#include "MenuSeparator.h"

#include "Common.h"

#include "UIKit/MenuSeparator.h"

using namespace d14engine;

namespace d14uikit
{
    MenuSeparator::MenuSeparator()
        :
        MenuSeparator(uikit::makeUIObject<uikit::MenuSeparator>()) { }

    _D14_UIKIT_CTOR(MenuSeparator)
        :
        MenuItem(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;

        setHeight(11);
    }
}
