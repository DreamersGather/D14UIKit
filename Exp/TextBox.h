#pragma once

#include "Common/Precompile.h"

#include "Inc/TextBox.h"

namespace d14engine::uikit { struct TextBox; }

namespace d14uikit
{
    struct TextBox::Impl
    {
        std::shared_ptr<d14engine::uikit::TextBox> uiobj = {};

        std::shared_ptr<Label> placeholder = nullptr;
    };
}
