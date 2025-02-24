#include "Common/Precompile.h"

#include "Renderer.h"

#include "Common.h"

#include "Renderer/Renderer.h"

using namespace d14engine;

namespace d14uikit
{
    _D14_UIKIT_CTOR(Renderer)
        :
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    IDXGIFactory6* Renderer::dxgiFactory() const
    {
        return pimpl->uiobj->rndr->dxgiFactory();
    }

    ID3D12Device* Renderer::d3d12Device() const
    {
        return pimpl->uiobj->rndr->d3d12Device();
    }

    ID3D12CommandQueue* Renderer::cmdQueue() const
    {
        return pimpl->uiobj->rndr->cmdQueue();
    }

    ID3D12CommandAllocator* Renderer::cmdAlloc() const
    {
        return pimpl->uiobj->rndr->cmdAlloc();
    }

    ID3D12GraphicsCommandList* Renderer::cmdList() const
    {
        return pimpl->uiobj->rndr->cmdList();
    }

    //UINT Renderer::backBufferCount() const
    //{
    //    return renderer::FrameResource::g_bufferCount;
    //}

    //UINT Renderer::currFrameIndex() const
    //{
    //    return pimpl->uiobj->rndr->currFrameIndex();
    //}

    //ID3D11On12Device* Renderer::d3d11On12Device() const
    //{
    //    return pimpl->uiobj->rndr->d3d11On12Device();
    //}

    //ID3D11DeviceContext* Renderer::d3d11DeviceContext() const
    //{
    //    return pimpl->uiobj->rndr->d3d11DeviceContext();
    //}

    //ID2D1Factory1* Renderer::d2d1Factory() const
    //{
    //    return pimpl->uiobj->rndr->d2d1Factory();
    //}

    //ID2D1Device* Renderer::d2d1Device() const
    //{
    //    return pimpl->uiobj->rndr->d2d1Device();
    //}

    //ID2D1DeviceContext* Renderer::d2d1DeviceContext() const
    //{
    //    return pimpl->uiobj->rndr->d2d1DeviceContext();
    //}

    //IDWriteFactory3* Renderer::dwriteFactory() const
    //{
    //    return pimpl->uiobj->rndr->dwriteFactory();
    //}

    //DXGI_FORMAT Renderer::renderTargetFormat() const
    //{
    //    return pimpl->uiobj->rndr->g_renderTargetFormat;
    //}

    //DXGI_FORMAT Renderer::depthStencilFormat() const
    //{
    //    return pimpl->uiobj->rndr->g_depthStencilFormat;
    //}

    //IDXGISwapChain3* Renderer::swapChain() const
    //{
    //    return pimpl->uiobj->rndr->swapChain();
    //}

    //ID3D12DescriptorHeap* Renderer::rtvHeap() const
    //{
    //    return pimpl->uiobj->rndr->rtvHeap();
    //}

    //ID3D12DescriptorHeap* Renderer::srvHeap() const
    //{
    //    return pimpl->uiobj->rndr->srvHeap();
    //}

    //ID3D12Resource* Renderer::currBackBuffer() const
    //{
    //    return pimpl->uiobj->rndr->currBackBuffer();
    //}

    //ID3D12Resource* Renderer::sceneBuffer() const
    //{
    //    return pimpl->uiobj->rndr->sceneBuffer();
    //}

    //ID3D11Resource* Renderer::wrappedBuffer() const
    //{
    //    return pimpl->uiobj->rndr->wrappedBuffer();
    //}

    //ID2D1Bitmap1* Renderer::d2d1RenderTarget() const
    //{
    //    return pimpl->uiobj->rndr->d2d1RenderTarget();
    //}

    void Renderer::resetCmdList()
    {
        pimpl->uiobj->rndr->resetCmdList();
    }
    void Renderer::submitCmdList()
    {
        pimpl->uiobj->rndr->submitCmdList();
    }

    void Renderer::flushCmdQueue()
    {
        pimpl->uiobj->rndr->flushCmdQueue();
    }

    void Renderer::beginGpuCommand()
    {
        pimpl->uiobj->rndr->beginGpuCommand();
    }

    void Renderer::endGpuCommand()
    {
        pimpl->uiobj->rndr->endGpuCommand();
    }

    void Renderer::waitGpuCommand()
    {
        pimpl->uiobj->rndr->waitGpuCommand();
    }

    const XMVECTORF32& Renderer::sceneColor() const
    {
        return pimpl->uiobj->rndr->sceneColor();
    }

    void Renderer::setSceneColor(const XMVECTORF32& color)
    {
        pimpl->uiobj->rndr->setSceneColor(color);
    }
}
