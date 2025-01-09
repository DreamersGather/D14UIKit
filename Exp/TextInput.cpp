#include "Common/Precompile.h"

#include "TextInput.h"

#include "Common.h"

#include "UIKit/TextInput.h"

using namespace d14engine;

namespace d14uikit
{
    TextInput::TextInput(bool multiline)
        :
        TextInput(uikit::makeUIObject<uikit::TextInput>(multiline)) { }

    _D14_UIKIT_CTOR(TextInput)
        :
        RawTextInput(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
