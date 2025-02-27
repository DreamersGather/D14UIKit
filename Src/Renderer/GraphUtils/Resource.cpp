#include "Common/Precompile.h"

#include "Renderer/GraphUtils/Resource.h"

#include "Common/DirectXError.h"

#include "Renderer/GraphUtils/Barrier.h"

namespace d14engine::renderer::graph_utils
{
    ComPtr<ID3D12Resource> capture(
        ID3D12Resource* texture,
        ID3D12GraphicsCommandList* cmdList,
        D3D12_RESOURCE_STATES orgState)
    {
        ComPtr<ID3D12Device> device = {};
        THROW_IF_FAILED(texture->GetDevice(IID_PPV_ARGS(&device)));

        auto texDesc = texture->GetDesc();
        UINT64 totalBytes = 0, rowSizeInBytes = 0;

        device->GetCopyableFootprints
        (
        /* pResourceDesc    */ &texDesc,
        /* FirstSubresource */ 0,
        /* NumSubresources  */ 1,
        /* BaseOffset       */ 0,
        /* pLayouts         */ nullptr,
        /* pNumRows         */ nullptr,
        /* pRowSizeInBytes  */ &rowSizeInBytes,
        /* pTotalBytes      */ &totalBytes
        );
        ComPtr<ID3D12Resource> staging = {};
        auto stagingProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
        auto stagingDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);

        THROW_IF_FAILED(device->CreateCommittedResource
        (
        /* pHeapProperties      */ &stagingProp,
        /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
        /* pDesc                */ &stagingDesc,
        /* InitialResourceState */ D3D12_RESOURCE_STATE_COPY_DEST,
        /* pOptimizedClearValue */ nullptr,
        /* riidResource         */
        /* ppvResource          */ IID_PPV_ARGS(&staging)
        ));
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition
        (
            texture, orgState, D3D12_RESOURCE_STATE_COPY_SOURCE
        );
        cmdList->ResourceBarrier(1, &barrier);

        D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
        footprint.Footprint =
        {
            .Format   = texDesc.Format,
            .Width    = (UINT)texDesc.Width,
            .Height   = (UINT)texDesc.Height,
            .Depth    = (UINT)texDesc.DepthOrArraySize,
            .RowPitch = (UINT)rowSizeInBytes
        };
        CD3DX12_TEXTURE_COPY_LOCATION dst(staging.Get(), footprint);
        CD3DX12_TEXTURE_COPY_LOCATION src(texture, 0);

        cmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

        graph_utils::revertBarrier(1, &barrier);
        cmdList->ResourceBarrier(1, &barrier);

        return staging;
    }
}
