#pragma once

// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#header-include-order
// It is recommended to include Agility SDK headers before the Windows 10 SDK to avoid conflicts.
#include <d3d12.h>

#include <DirectXColors.h>
#include <DirectXMath.h>

#include <d2d1.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dwrite_3.h>
#include <dxgi1_6.h>

#include "Common.h"

_D14_UIKIT_FWDEF(Renderer)

namespace d14uikit
{
    using namespace DirectX;

    class DllExport Renderer : public NonCopyable
    {
        friend class Application;

        _D14_UIKIT_PIMPL(Renderer)

        IDXGIFactory6* dxgiFactory() const;

        ID3D12Device* d3d12Device() const;

        ID3D12CommandQueue* cmdQueue() const;

        ID3D12CommandAllocator* cmdAlloc() const;

        ID3D12GraphicsCommandList* cmdList() const;

        //UINT backBufferCount() const;

        //UINT currFrameIndex() const;

        //ID3D11On12Device* d3d11On12Device() const;

        //ID3D11DeviceContext* d3d11DeviceContext() const;

        //ID2D1Factory1* d2d1Factory() const;

        //ID2D1Device* d2d1Device() const;

        //ID2D1DeviceContext* d2d1DeviceContext() const;

        //IDWriteFactory3* dwriteFactory() const;

        //DXGI_FORMAT renderTargetFormat() const;
        //DXGI_FORMAT depthStencilFormat() const;

        //IDXGISwapChain3* swapChain() const;

        //ID3D12DescriptorHeap* rtvHeap() const;
        //ID3D12DescriptorHeap* srvHeap() const;

        //ID3D12Resource* currBackBuffer() const;

        //ID3D12Resource* sceneBuffer() const;

        //ID3D11Resource* wrappedBuffer() const;

        //ID2D1Bitmap1* d2d1RenderTarget() const;

        void resetCmdList();
        void submitCmdList();
        void flushCmdQueue();

        void beginGpuCommand();
        void endGpuCommand();

        void waitGpuCommand();

        const XMVECTORF32& sceneColor() const;
        void setSceneColor(const XMVECTORF32& color);
    };
}
