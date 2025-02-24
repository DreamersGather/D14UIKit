#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::info_utils
{
    Wstring text(const DXGI_ADAPTER_DESC& desc);
    Wstring text(const DXGI_MODE_DESC& desc);
    Wstring text(const DXGI_OUTPUT_DESC& desc);
}
