#include "Common/Precompile.h"

#include "FilledButton.h"

#include "Common.h"

#include "UIKit/FilledButton.h"

using namespace d14engine;

namespace d14uikit
{
    FilledButton::FilledButton(const std::wstring& text)
        :
        FilledButton(uikit::makeUIObject<uikit::FilledButton>(text)) { }

    _D14_UIKIT_CTOR(FilledButton)
        :
        Panel(uiobj),
        FlatButton(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
