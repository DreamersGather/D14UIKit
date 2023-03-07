#pragma once

#include "Common/Precompile.h"

#ifdef _DEBUG

namespace d14engine::renderer::debug_utils
{
    void enableD3d12DebugLayer();

    void suppressWarnings(ID3D12Device* device);

    ComPtr<IDXGIDebug> queryDxgiDebugInterface();
}

#endif
