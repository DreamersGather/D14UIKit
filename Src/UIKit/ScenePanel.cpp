#include "Common/Precompile.h"

#include "UIKit/ScenePanel.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"
#include "Common/MathUtils/Basic.h"

#include "Renderer/GraphUtils/Barrier.h"
#include "Renderer/GraphUtils/ParamHelper.h"
#include "Renderer/Interfaces/DrawLayer.h"
#include "Renderer/Interfaces/IDrawObject.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapObject.h"
#include "UIKit/PlatformUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    ScenePanel::ScenePanel(const D2D1_RECT_F& rect, int cmdLayerPriority)
        :
        Panel(rect)
    {
        THROW_IF_NULL(Application::g_app);

        auto rndr = Application::g_app->dx12Renderer();

        auto device = rndr->d3d12Device();
        m_cmdLayer = std::make_shared<Renderer::CommandLayer>(device);

        m_cmdLayer->setPriority(cmdLayerPriority);
        rndr->cmdLayers.insert(m_cmdLayer);

        using D3D12Target = Renderer::CommandLayer::D3D12Target;
        auto& target = m_cmdLayer->drawTarget.emplace<D3D12Target>();

        m_primaryLayer = std::make_shared<DrawLayer>();
        m_primaryLayer->setPriority(INT_MIN);
        target[m_primaryLayer] = {};

        m_closingLayer = std::make_shared<DrawLayer>();
        m_closingLayer->setPriority(INT_MAX);
        target[m_closingLayer] = {};
    }

    void ScenePanel::onInitializeFinish()
    {
        THROW_IF_NULL(Application::g_app);

        Panel::onInitializeFinish();

        auto rndr = Application::g_app->dx12Renderer();

        // The back/MSAA buffers share the same RTV heap. Just 1 RTV seat
        // is enough since only 1 of them is used as render target at the same time:
        // MSAA Enabled: back buffer == staging, MSAA buffer == render target
        // MSAA Disabled: back buffer == render target, MSAA buffer == not used

        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 1;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 0; // single-adapter

        auto device = rndr->d3d12Device();
        THROW_IF_FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap)));

        loadOffscreenTexture();

        // It is worth noting that the back buffer is always transitioned to
        // PIXEL_SHADER_RESOURCE state after finishing the Direct3D rendering.
        // This is because Direct2D::DrawBitmap requires the underlying resource
        // be of such state to resample the shared bitmap to the render target.

        m_primaryLayer->f_onRendererDrawD3d12LayerAfter = [this]
        (DrawLayer* layer, Renderer* rndr)
        {
            if (!m_msaaEnabled) // back buffer == render target
            {
                auto barrier = CD3DX12_RESOURCE_BARRIER::Transition
                (
                    m_backBuffer.Get(),
                    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                    D3D12_RESOURCE_STATE_RENDER_TARGET
                );
                rndr->cmdList()->ResourceBarrier(1, &barrier);
            }
            auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

            rndr->cmdList()->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);
            rndr->cmdList()->ClearRenderTargetView(rtvHandle, m_clearColor, 0, nullptr);
        };
        m_closingLayer->f_onRendererDrawD3d12LayerAfter = [this]
        (DrawLayer* layer, Renderer* rndr)
        {
            if (m_msaaEnabled) // MSAA buffer == render target
            {
                D3D12_RESOURCE_BARRIER barriers[] =
                {
                    CD3DX12_RESOURCE_BARRIER::Transition
                    (
                        m_backBuffer.Get(),
                        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                        D3D12_RESOURCE_STATE_RESOLVE_DEST
                    ),
                    CD3DX12_RESOURCE_BARRIER::Transition
                    (
                        m_msaaBuffer.Get(),
                        D3D12_RESOURCE_STATE_RENDER_TARGET,
                        D3D12_RESOURCE_STATE_RESOLVE_SOURCE
                    ),
                };
                rndr->cmdList()->ResourceBarrier(NUM_ARR_ARGS(barriers));

                rndr->cmdList()->ResolveSubresource(
                    /* dstResource */ m_backBuffer.Get(), 0,
                    /* srcResource */ m_msaaBuffer.Get(), 0,
                    Renderer::g_renderTargetFormat);

                graph_utils::revertBarrier(NUM_ARR_ARGS(barriers));
                rndr->cmdList()->ResourceBarrier(NUM_ARR_ARGS(barriers));
            }
            else // back buffer == render target
            {
                auto barrier = CD3DX12_RESOURCE_BARRIER::Transition
                (
                    m_backBuffer.Get(),
                    D3D12_RESOURCE_STATE_RENDER_TARGET,
                    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
                );
                rndr->cmdList()->ResourceBarrier(1, &barrier);
            }
        };
    }

    const SharedPtr<DrawLayer>& ScenePanel::primaryLayer() const
    {
        return m_primaryLayer;
    }

    const SharedPtr<DrawLayer>& ScenePanel::closingLayer() const
    {
        return m_closingLayer;
    }

    const SharedPtr<Renderer::CommandLayer>& ScenePanel::cmdLayer() const
    {
        return m_cmdLayer;
    }

    int ScenePanel::cmdLayerPriority() const
    {
        return m_cmdLayer->priority();
    }

    void ScenePanel::setCmdLayerPriority(int value)
    {
        THROW_IF_NULL(Application::g_app);

        auto rndr = Application::g_app->dx12Renderer();
        rndr->cmdLayers.erase(m_cmdLayer);
        m_cmdLayer->setPriority(value);
        rndr->cmdLayers.insert(m_cmdLayer);
    }

    UINT ScenePanel::sampleCount() const
    {
        return m_sampleCount;
    }

    UINT ScenePanel::sampleQuality() const
    {
        return m_sampleQuality;
    }

    bool ScenePanel::setMultiSample(UINT count, OptParam<UINT> quality)
    {
        THROW_IF_NULL(Application::g_app);

        auto rndr = Application::g_app->dx12Renderer();

        if (count <= 1)
        {
            m_msaaEnabled = false;

            m_sampleCount = 1;
            m_sampleQuality = 0;

            loadOffscreenTexture();
            return true;
        }
        else // Enable MSAA
        {
            auto& feature = rndr->d3d12DeviceInfo().feature;
            auto level = feature.queryMsaaQualityLevel(count);
            if (level.has_value())
            {
                m_sampleCount = count;
                m_sampleQuality = level.value();

                m_msaaEnabled = true;

                loadOffscreenTexture();
                return true;
            }
            else return false;
        }
    }

    bool ScenePanel::msaaEnabled() const
    {
        return m_msaaEnabled;
    }

    void ScenePanel::loadOffscreenTexture()
    {
        THROW_IF_NULL(Application::g_app);

        auto rndr = Application::g_app->dx12Renderer();
        rndr->beginGpuCommand();

        createBackBuffer();
        if (m_msaaEnabled)
        {
            createMsaaBuffer();
        }
        else m_msaaBuffer.Reset();
        createWrappedBuffer();

        rndr->endGpuCommand();
    }

    void ScenePanel::createBackBuffer()
    {
        THROW_IF_NULL(Application::g_app);

        auto texSize = math_utils::roundu(size());
        texSize.width = std::max(texSize.width, 1u);
        texSize.height = std::max(texSize.height, 1u);

        auto rndr = Application::g_app->dx12Renderer();

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto desc = CD3DX12_RESOURCE_DESC::Tex2D
        (
            /* format    */ Renderer::g_renderTargetFormat,
            /* width     */ math_utils::round<UINT64>(texSize.width),
            /* height    */ math_utils::round<UINT>(texSize.height),
            /* arraySize */ 1,
            /* mipLevels */ 1
        );
        if (m_msaaEnabled)
        {
            THROW_IF_FAILED(rndr->d3d12Device()->CreateCommittedResource
            (
                /* pHeapProperties      */ &prop,
                /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
                /* pDesc                */ &desc,
                /* InitialResourceState */ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                /* pOptimizedClearValue */ nullptr,
                /* riidResource         */
                /* ppvResource          */ IID_PPV_ARGS(&m_backBuffer)
            ));
        }
        else // back buffer == render target
        {
            desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

            D3D12_CLEAR_VALUE clearValue = {};
            clearValue.Format = Renderer::g_renderTargetFormat;
            memcpy(clearValue.Color, m_clearColor, 4 * sizeof(FLOAT));

            auto device = rndr->d3d12Device();

            THROW_IF_FAILED(device->CreateCommittedResource
            (
                /* pHeapProperties      */ &prop,
                /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
                /* pDesc                */ &desc,
                /* InitialResourceState */ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                /* pOptimizedClearValue */ &clearValue,
                /* riidResource         */
                /* ppvResource          */ IID_PPV_ARGS(&m_backBuffer)
            ));

            auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
            device->CreateRenderTargetView(m_backBuffer.Get(), nullptr, rtvHandle);
        }
    }

    void ScenePanel::createMsaaBuffer()
    {
        THROW_IF_NULL(Application::g_app);

        auto texSize = math_utils::roundu(size());
        texSize.width = std::max(texSize.width, 1u);
        texSize.height = std::max(texSize.height, 1u);

        auto rndr = Application::g_app->dx12Renderer();

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto desc = CD3DX12_RESOURCE_DESC::Tex2D
        (
            /* format    */ Renderer::g_renderTargetFormat,
            /* width     */ math_utils::round<UINT64>(texSize.width),
            /* height    */ math_utils::round<UINT>(texSize.height),
            /* arraySize */ 1,
            /* mipLevels */ 1
        );
        desc.SampleDesc.Count = m_sampleCount;
        desc.SampleDesc.Quality = m_sampleQuality;
        desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = Renderer::g_renderTargetFormat;
        memcpy(clearValue.Color, m_clearColor, 4 * sizeof(FLOAT));

        auto device = rndr->d3d12Device();

        THROW_IF_FAILED(device->CreateCommittedResource
        (
            /* pHeapProperties      */ &prop,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &desc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_RENDER_TARGET,
            /* pOptimizedClearValue */ &clearValue,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&m_msaaBuffer)
        ));

        auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
        device->CreateRenderTargetView(m_msaaBuffer.Get(), nullptr, rtvHandle);
    }

    void ScenePanel::createWrappedBuffer()
    {
        THROW_IF_NULL(Application::g_app);

        auto rndr = Application::g_app->dx12Renderer();
        auto dpi = platform_utils::dpi();

        auto props = D2D1::BitmapProperties
        (
            /* pixelFormat */ D2D1::PixelFormat(Renderer::g_renderTargetFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
            /* dpiX        */ dpi,
            /* dpiY        */ dpi
        );
        D3D11_RESOURCE_FLAGS flags = {};
        flags.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        auto device = rndr->d3d11On12Device();
        THROW_IF_FAILED(device->CreateWrappedResource
        (
            /* pResource12  */ m_backBuffer.Get(),
            /* pFlags11     */ &flags,
            /* InState      */ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            /* OutState     */ D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            /* ppResource11 */ IID_PPV_ARGS(&m_wrappedBuffer)
        ));
        ComPtr<IDXGISurface> surface;
        THROW_IF_FAILED(m_wrappedBuffer.As(&surface));

        auto context = rndr->d2d1DeviceContext();
        THROW_IF_FAILED(context->CreateSharedBitmap(__uuidof(surface), surface.Get(), &props, &m_sharedBitmap));
    }

    const XMVECTORF32& ScenePanel::clearColor() const
    {
        return m_clearColor;
    }

    void ScenePanel::setClearColor(const XMVECTORF32& color)
    {
        m_clearColor = color;

        loadOffscreenTexture();
    }

    ID3D12DescriptorHeap* ScenePanel::rtvHeap() const
    {
        return m_rtvHeap.Get();
    }

    ID3D12Resource* ScenePanel::backBuffer() const
    {
        return m_backBuffer.Get();
    }

    ID3D11Resource* ScenePanel::wrappedBuffer() const
    {
        return m_wrappedBuffer.Get();
    }

    ID2D1Bitmap* ScenePanel::sharedBitmap() const
    {
        return m_sharedBitmap.Get();
    }

    void ScenePanel::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        Panel::onRendererDrawD2d1ObjectHelper(rndr);

        // There is no need to acquire or release the wrapped buffer since
        // the state transition has already been handled in Direct3D layer.

        // round to fit pixel size
        auto rect = math_utils::roundf(m_absoluteRect);

        D2D1_INTERPOLATION_MODE mode = {};
        if (sharedBitmapProperty.interpolationMode.has_value())
        {
            mode = sharedBitmapProperty.interpolationMode.value();
        }
        else mode = BitmapObject::g_interpolationMode;

        rndr->d2d1DeviceContext()->DrawBitmap(
            m_sharedBitmap.Get(), rect, sharedBitmapProperty.opacity, mode);
    }

    void ScenePanel::onSizeHelper(SizeEvent& e)
    {
        Panel::onSizeHelper(e);
        
        loadOffscreenTexture();
    }
}
