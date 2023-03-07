#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{
    void revertBarrier(size_t NumBarriers, D3D12_RESOURCE_BARRIER* pBarriers);
}
