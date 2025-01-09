#pragma once

#include "Common/Precompile.h"

#include "Inc/RawTextEditor.h"

namespace d14uikit
{
    struct RawTextEditor::Impl
    {
        std::shared_ptr<d14engine::uikit::RawTextEditor> uiobj = {};
    };
}
