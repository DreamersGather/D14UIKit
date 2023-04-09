#pragma once

#include "Common/Precompile.h"

#include "Inc/TextEditor.h"

namespace d14engine::uikit { struct TextEditor; }

namespace d14uikit
{
    struct TextEditor::Impl
    {
        std::shared_ptr<d14engine::uikit::TextEditor> uiobj = {};

        std::shared_ptr<Label> placeholder = nullptr;
    };
}
