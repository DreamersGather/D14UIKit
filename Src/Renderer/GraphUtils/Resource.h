#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{
    // GPU Commands Required
    ComPtr<ID3D12Resource> capture(
        ID3D12Resource* texture,
        ID3D12GraphicsCommandList* cmdList,
        D3D12_RESOURCE_STATES orgState = D3D12_RESOURCE_STATE_COMMON);
}
