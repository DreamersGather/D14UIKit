#pragma once

#include "Common/Precompile.h"

#include "Inc/Font.h"

namespace d14uikit
{
    struct Font::Impl
    {
        d14engine::ComPtr<IDWriteTextFormat> textFormat = {};
    };
}
