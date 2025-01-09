#include "Common/Precompile.h"

#include "FlatButton.h"

#include "Common.h"

#include "UIKit/FlatButton.h"

using namespace d14engine;

namespace d14uikit
{
    FlatButton::FlatButton(const std::wstring& text)
        :
        FlatButton(uikit::makeUIObject<uikit::FlatButton>(text)) { }

    _D14_UIKIT_CTOR(FlatButton)
        :
        Panel(uiobj),
        Button(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
