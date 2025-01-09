#include "Common/Precompile.h"

#include "TextEditor.h"

#include "Common.h"

#include "UIKit/TextEditor.h"

using namespace d14engine;

namespace d14uikit
{
    TextEditor::TextEditor()
        :
        TextEditor(uikit::makeUIObject<uikit::TextEditor>()) { }

    _D14_UIKIT_CTOR(TextEditor)
        :
        TextInput(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
