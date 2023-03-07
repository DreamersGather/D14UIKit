#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    struct GpuBuffer
    {
    protected:
        ComPtr<ID3D12Resource> m_resource = {};

    public:
        ID3D12Resource* resource() const { return m_resource.Get(); }
    };

    // Maintains a resource buffer with DEFAULT type (static).
    // Pros: optimized for GPU, working at the best performance.
    // Cons: immutable at runtime, isolated between CPU and GPU.
    struct DefaultBuffer : GpuBuffer
    {
        DefaultBuffer(ID3D12Device* device, UINT64 byteSize);

    protected:
        ComPtr<ID3D12Resource> m_intermediate = {};

    public:
        ID3D12Resource* intermediate() const { return m_intermediate.Get(); }

    public:
        void uploadData(ID3D12GraphicsCommandList* cmdList, void* pSrc, UINT64 byteSize);
    };

    // Maintains a resource buffer with UPLOAD type (dynamic).
    // Pros: modifiable at runtime, coherent between CPU and GPU.
    // Cons: wouldn't work at the best performance of GPU device.
    struct UploadBuffer : GpuBuffer
    {
        UploadBuffer(ID3D12Device* device, UINT elemCount, UINT64 elemByteSize);

        virtual ~UploadBuffer()
        {
            if (m_resource) m_resource->Unmap(0, nullptr); m_mapped = nullptr;
        }

    protected:
        // Total element count in buffer.
        UINT m_elemCount = {};

        // Byte size of single element in buffer.
        UINT64 m_elemByteSize = {};

        // Where in memory the buffer is mapped to.
        // Use BYTE* to be compatible with DirectX.
        BYTE* m_mapped = nullptr;

    public:
        UINT elemCount() const { return m_elemCount; }

        UINT64 elemByteSize() const { return m_elemByteSize; }

        BYTE* mapped() const { return m_mapped; }

    public:
        // dstIndexOffset is not applied to pSrc.
        // Offset pSrc to the correct position before calling.
        void copyData(UINT dstIndexOffset, void* pSrc, UINT64 byteSize);
    };

    // Maps to the cbuffer of HLSL.
    // The only difference from UploadBuffer is that the data in this buffer
    // is aligned by the minimum hardware allocation size (usually 256-byte).
    struct ConstantBuffer : UploadBuffer
    {
        ConstantBuffer(ID3D12Device* device, UINT elemCount, UINT64 elemByteSize)
            : UploadBuffer(device, elemCount, calcElemSize(elemByteSize)) { }

    private:
        constexpr static UINT64 calcElemSize(UINT64 rawByteSize)
        { return (rawByteSize + 255) & ~255; } // round up to 256
    };
}
