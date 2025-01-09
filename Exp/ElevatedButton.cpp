#include "Common/Precompile.h"

#include "ElevatedButton.h"

#include "Common.h"

#include "UIKit/ElevatedButton.h"

using namespace d14engine;

namespace d14uikit
{
    ElevatedButton::ElevatedButton(const std::wstring& text)
        :
        ElevatedButton(uikit::makeUIObject<uikit::ElevatedButton>(text)) { }

    _D14_UIKIT_CTOR(ElevatedButton)
        :
        Panel(uiobj),
        FilledButton(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
