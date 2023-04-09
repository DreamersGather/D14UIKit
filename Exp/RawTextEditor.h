#pragma once

#include "Common/Precompile.h"

#include "Inc/RawTextEditor.h"

namespace d14engine::uikit { struct RawTextEditor; }

namespace d14uikit
{
    struct RawTextEditor::Impl
    {
        std::shared_ptr<d14engine::uikit::RawTextEditor> uiobj = {};

        std::shared_ptr<Label> placeholder = nullptr;
    };
}
