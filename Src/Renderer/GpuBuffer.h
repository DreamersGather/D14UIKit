#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    ////////////////
    // GPU Buffer //
    ////////////////

    struct GpuBuffer
    {
    protected:
        ComPtr<ID3D12Resource> m_resource = {};

    public:
        ID3D12Resource* resource() const;
    };

    ////////////////////
    // Default Buffer //
    ////////////////////

    // Maintains a resource buffer with DEFAULT type (static).
    // Pros: optimized for GPU, working at the best performance.
    // Cons: immutable at runtime, isolated between CPU and GPU.
    struct DefaultBuffer : GpuBuffer
    {
        DefaultBuffer(ID3D12Device* device, UINT64 byteSize);

    protected:
        ComPtr<ID3D12Resource> m_intermediate = {};

    public:
        ID3D12Resource* intermediate() const;

    public:
        // To update the data in DefaultBuffer:
        // 1. Call copyDataCPU to copy the data from CPU to intermediate,
        // 2. Call copyDataGPU to copy the data from intermediate to GPU.
        //
        // When the data size is small, you can directly call uploadData
        // (which internally calls those two functions in sequence);
        // otherwise, it is recommended to first call copyDataCPU for the 1st copy,
        // then reset the command list and call copyDataGPU for the 2nd copy.
        //
        // Performance Tips: When the GPU load is high,
        // the second method allows the CPU to proceed with the 1st copy
        // without waiting for the GPU, thereby improving overall performance.

        // Copy from CPU buffer to intermediate buffer.
        void copyDataCPU(void* pSrc, UINT64 byteSize);

        // GPU Commands Required
        // Copy from intermediate buffer to GPU buffer.
        void copyDataGPU(ID3D12GraphicsCommandList* cmdList, UINT64 byteSize);

        // GPU Commands Required
        // This simply calls copyDataCPU and copyDataGPU in sequence.
        void uploadData(ID3D12GraphicsCommandList* cmdList, void* pSrc, UINT64 byteSize)
        {
            copyDataCPU(pSrc, byteSize);
            copyDataGPU(cmdList, byteSize);
        }
    };

    ///////////////////
    // Upload Buffer //
    ///////////////////

    // Maintains a resource buffer with UPLOAD type (dynamic).
    // Pros: modifiable at runtime, coherent between CPU and GPU.
    // Cons: wouldn't work at the best performance of GPU device.
    struct UploadBuffer : GpuBuffer
    {
        UploadBuffer(ID3D12Device* device, UINT elemCount, UINT64 elemByteSize);

        virtual ~UploadBuffer();

    protected:
        UINT m_elemCount = {};
        UINT64 m_elemByteSize = {};

        // Where in memory the buffer is mapped to.
        // Use BYTE* to be compatible with DirectX.
        BYTE* m_mapped = nullptr;

    public:
        UINT elemCount() const;
        UINT64 elemByteSize() const;

        BYTE* mapped() const;

    public:
        // It is worth noting that dstIndexOffset will not be applied to pSrc,
        // and pSrc needs to be offset to the correct position before calling.
        void copyData(UINT dstIndexOffset, void* pSrc, UINT64 byteSize);
    };

    /////////////////////
    // Constant Buffer //
    /////////////////////

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
