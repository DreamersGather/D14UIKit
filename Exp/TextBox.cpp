#include "Common/Precompile.h"

#include "TextBox.h"

#include "Common.h"

#include "UIKit/TextBox.h"

using namespace d14engine;

namespace d14uikit
{
    TextBox::TextBox()
        :
        TextBox(uikit::makeUIObject<uikit::TextBox>()) { }

    _D14_UIKIT_CTOR(TextBox)
        :
        TextInput(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
