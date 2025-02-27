#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer
{
    struct FrameResource
    {
        friend struct Renderer;

        explicit FrameResource(ID3D12Device* device);

        // Maintain g_bufferCount copies for all frame-dynamic resources
        // to make sure they will be synchronized with the command fence.
        constexpr static UINT g_bufferCount = 3;

#define SET_REQUIRE(Name, Expression) \
static_assert(Expression, #Name " requires that " #Expression);

        SET_REQUIRE(flip model, g_bufferCount >= 2)
        SET_REQUIRE(swap chain, g_bufferCount <= DXGI_MAX_SWAP_CHAIN_BUFFERS)

#undef SET_REQUIRE

        template<typename T>
        using Array = std::array<T, g_bufferCount>;

        using CmdAllocArray = Array<ComPtr<ID3D12CommandAllocator>>;

    private:
        UINT64 m_fenceValue = 0;

        // Used to allocate commands for the updating in each render pass.
        ComPtr<ID3D12CommandAllocator> m_cmdAlloc = {};

    public:
        UINT64 fenceValue() const;

        void resetCmdList(ID3D12GraphicsCommandList* cmdList);
    };
}
