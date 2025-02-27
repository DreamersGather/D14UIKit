#include "Common/Precompile.h"

#include "Renderer/FrameData/FrameResource.h"

#include "Common/DirectXError.h"

namespace d14engine::renderer
{
    FrameResource::FrameResource(ID3D12Device* device)
    {
        THROW_IF_FAILED(device->CreateCommandAllocator
        (
        /* type               */ D3D12_COMMAND_LIST_TYPE_DIRECT,
        /* riid               */
        /* ppCommandAllocator */ IID_PPV_ARGS(&m_cmdAlloc)
        ));
    }

    UINT64 FrameResource::fenceValue() const
    {
        return m_fenceValue;
    }

    void FrameResource::resetCmdList(ID3D12GraphicsCommandList* cmdList)
    {
        m_cmdAlloc->Reset();
        cmdList->Reset(m_cmdAlloc.Get(), nullptr);
    }
}
