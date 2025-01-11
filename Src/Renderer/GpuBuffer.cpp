#include "Common/Precompile.h"

#include "Renderer/GpuBuffer.h"

#include "Common/DirectXError.h"

#include "Renderer/GraphUtils/Barrier.h"
#include "Renderer/GraphUtils/ParamHelper.h"

namespace d14engine::renderer
{
    DefaultBuffer::DefaultBuffer(ID3D12Device* device, UINT64 byteSize)
    {
        auto resourceProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

        THROW_IF_FAILED(device->CreateCommittedResource
        (
            /* pHeapProperties      */ &resourceProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &resourceDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&m_resource)
        ));
        auto intermediateProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto intermediateDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

        THROW_IF_FAILED(device->CreateCommittedResource
        (
            /* pHeapProperties      */ &intermediateProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &intermediateDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&m_intermediate)
        ));
    }

    void DefaultBuffer::uploadData(ID3D12GraphicsCommandList* cmdList, void* pSrc, UINT64 byteSize)
    {
        // Copy from source data to intermediate buffer.
        BYTE* mapped = nullptr;
        THROW_IF_FAILED(m_intermediate->Map(0, nullptr, (void**)&mapped));

        memcpy(mapped, pSrc, (size_t)byteSize);

        m_intermediate->Unmap(0, nullptr);

        // Copy from intermediate buffer to resource buffer.
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition
        (
            m_resource.Get(),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            D3D12_RESOURCE_STATE_COPY_DEST
        );
        cmdList->ResourceBarrier(1, &barrier);

        cmdList->CopyBufferRegion(m_resource.Get(), 0, m_intermediate.Get(), 0, byteSize);

        graph_utils::revertBarrier(1, &barrier);
        cmdList->ResourceBarrier(1, &barrier);
    }

    UploadBuffer::UploadBuffer(ID3D12Device* device, UINT elemCount, UINT64 elemByteSize)
        : m_elemCount(elemCount), m_elemByteSize(elemByteSize)
    {
        auto resourceProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(elemCount * elemByteSize);

        THROW_IF_FAILED(device->CreateCommittedResource
        (
            /* pHeapProperties      */ &resourceProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &resourceDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&m_resource)
        ));
        THROW_IF_FAILED(m_resource->Map(0, nullptr, (void**)&m_mapped));
    }

    void UploadBuffer::copyData(UINT dstIndexOffset, void* pSrc, UINT64 byteSize)
    {
        memcpy(&m_mapped[dstIndexOffset * m_elemByteSize], pSrc, (size_t)byteSize);
    }        
}
