#include "Common/Precompile.h"

#include "Renderer/GraphUtils/Barrier.h"

namespace d14engine::renderer::graph_utils
{
    void revertBarrier(size_t NumBarriers, D3D12_RESOURCE_BARRIER* pBarriers)
    {
        for (size_t i = 0; i < NumBarriers; ++i)
        {
            auto& barrier = *(pBarriers + i);
            std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
        }
    }
}
