#pragma once

#include "Common/Precompile.h"

#include "UIKit/Panel.h"

namespace d14engine::renderer { struct DrawLayer; }

namespace d14engine::uikit
{
    struct ScenePanel : Panel
    {
        ScenePanel(const D2D1_RECT_F& rect = {}, int cmdLayerPriority = 100);

        void onInitializeFinish() override;

    protected:
        SharedPtr<renderer::DrawLayer> m_primaryLayer = {};
        SharedPtr<renderer::DrawLayer> m_closingLayer = {};

        SharedPtr<renderer::Renderer::CommandLayer> m_cmdLayer = {};

    public:
        const SharedPtr<renderer::DrawLayer>& primaryLayer() const;
        const SharedPtr<renderer::DrawLayer>& closingLayer() const;

        const SharedPtr<renderer::Renderer::CommandLayer>& cmdLayer() const;

        int cmdLayerPriority() const;
        void setCmdLayerPriority(int value);

    protected:
        UINT m_sampleCount = 1, m_sampleQuality = 0;

        bool m_msaaEnabled = false;

    public:
        UINT sampleCount() const; UINT sampleQuality() const;

        // Uses the max supported MSAA quality level when quality is empty.
        // Returns whether the target MSAA setting is enabled successfully.
        bool setMultiSample(UINT count, OptParam<UINT> quality = std::nullopt);

        bool msaaEnabled() const;

    protected:
        XMVECTORF32 m_clearColor = Colors::Black;

        ComPtr<ID3D12DescriptorHeap> m_rtvHeap = {};

        ComPtr<ID3D12Resource> m_msaaBuffer = {};
        ComPtr<ID3D12Resource> m_backBuffer = {};

        ComPtr<ID3D11Resource> m_wrappedBuffer = {};

        ComPtr<ID2D1Bitmap> m_sharedBitmap = {};

        void loadOffscreenTexture();

        void createBackBuffer();
        void createMsaaBuffer();

        void createWrappedBuffer();

    public:
        const XMVECTORF32& clearColor() const;
        void setClearColor(const XMVECTORF32& color);

        ID3D12DescriptorHeap* rtvHeap() const;

        ID3D12Resource* backBuffer() const;

        ID3D11Resource* wrappedBuffer() const;

        ID2D1Bitmap* sharedBitmap() const;

    public:
        struct SharedBitmapProperty
        {
            float opacity = 1.0f;
            Optional<D2D1_INTERPOLATION_MODE> interpolationMode = std::nullopt;
        }
        sharedBitmapProperty = {};

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        // Panel
        void onSizeHelper(SizeEvent& e) override;
    };
}
