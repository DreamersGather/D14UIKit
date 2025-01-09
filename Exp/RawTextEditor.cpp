#include "Common/Precompile.h"

#include "RawTextEditor.h"

#include "Common.h"

#include "UIKit/RawTextEditor.h"

using namespace d14engine;

namespace d14uikit
{
    RawTextEditor::RawTextEditor()
        :
        RawTextEditor(uikit::makeUIObject<uikit::RawTextEditor>()) { }

    _D14_UIKIT_CTOR(RawTextEditor)
        :
        RawTextInput(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }
}
