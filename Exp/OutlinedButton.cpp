#include "Common/Precompile.h"

#include "OutlinedButton.h"

#include "Common.h"

#include "UIKit/OutlinedButton.h"

using namespace d14engine;

namespace d14uikit
{
    OutlinedButton::OutlinedButton(const std::wstring& text)
        :
        OutlinedButton(uikit::makeUIObject<uikit::OutlinedButton>(text)) { }

    _D14_UIKIT_CTOR(OutlinedButton)
        :
        Panel(uiobj),
        FlatButton(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
