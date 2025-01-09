#include "Common/Precompile.h"

#include "RawTextBox.h"

#include "Common.h"

#include "UIKit/RawTextBox.h"

using namespace d14engine;

namespace d14uikit
{
    RawTextBox::RawTextBox()
        :
        RawTextBox(uikit::makeUIObject<uikit::RawTextBox>()) { }

    _D14_UIKIT_CTOR(RawTextBox)
        :
        RawTextInput(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
