#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnableMasterPtr.h"
#include "Common/Interfaces/ISortable.h"

#include "Renderer/FrameData/FrameResource.h"

namespace d14engine::renderer
{
    struct IDrawLayer;
    struct IDrawObject;
    struct IDrawObject2D;
    struct Letterbox;
    struct TickTimer;

    struct Renderer : cpp_lang_utils::NonCopyable
    {
        struct CreateInfo
        {
            // Resources (such as shaders) are loaded from this path.
            Wstring binaryPath = L"Bin/";

            Optional<float> dpi = std::nullopt;

            bool fullscreen = false;

            // Select GPU device. Set to 0 to use the default one.
            UINT adapterIndex = 0;

            // For [flip] model, which is used by default, values are:
            // 0 - The presentation occurs immediately, there is no synchronization.
            // 1 through 4 - Synchronize presentation after the nth vertical blank.
            UINT syncInterval = 0;

            // Allowing tearing is required for VRR displays.
            bool allowTearing = false;

            // The scene will be resized to:
            // (True) fit the resolution of current display mode.
            // (False) fill the client area of the root Win32 window.
            bool scaling = false;

            // Select output monitor. Set to 0 to use the default one.
            UINT outputIndex = 0;

            // The scene will be resized to fit the resolution
            // of specific display mode if enable resolution-scaling.
            UINT displayModeIndex = 0;

            // The back buffers will be:
            // (True) alpha bitmaps provided by DirectComposition.
            // (False) opaque Direct3D textures with best performance.
            bool composition = false;

            // Clear value of the scene buffer.
            XMVECTORF32 sceneColor = Colors::Black;

            // Clear value of the composition layer.
            D2D1_COLOR_F layerColor = { .a = 0.0f };
        };

        Renderer(HWND window, const CreateInfo& info = {});

        virtual ~Renderer();

        // This field stores the original create-info passed in the ctor.
        const CreateInfo createInfo = {};

#pragma region Win32 Components

    private:
        static RECT queryDesktopRectGDI();

        struct Window : cpp_lang_utils::EnableMasterPtr<Renderer>
        {
            using EnableMasterPtrType::EnableMasterPtrType;

            Window(Renderer* master, HWND ptr);

            HWND ptr = nullptr;

            void onResize();

        private:
            mutable RECT m_clientRect = {};

        public:
            const RECT& clientRect() const;

            UINT clientWidth() const;
            UINT clientHeight() const;

        private:
            mutable RECT m_windowRect = {};

        public:
            const RECT& windowRect() const;

            UINT windowWidth() const;
            UINT windowHeight() const;

        private:
            mutable struct OriginalInfo
            {
                UINT style = 0;
                RECT windowRect = {};
            }
            m_originalInfo = {};

            mutable bool m_fullscreen = false;

        public:
            const OriginalInfo& originalInfo() const;

            bool fullscreen() const;
            void setFullscreen(bool value) const;
        }
        m_window{ this };

    public:
        const Window& window() const;

        void onWindowResize(); // Call this in WM_SIZE.
        void renderNextFrame(); // Call this in WM_SIZE, WM_PAINT.

#pragma endregion

#pragma region D3D12 Components

    private:
        ComPtr<IDXGIFactory6> m_dxgiFactory = {};

        using AdapterArray = std::vector<ComPtr<IDXGIAdapter>>;

        struct DxgiFactoryInfo : cpp_lang_utils::EnableMasterPtr<Renderer>
        {
            using EnableMasterPtrType::EnableMasterPtrType;

            struct Property
            {
                AdapterArray availableAdapters = {};
            }
            property = {};

            struct Feature
            {
                bool allowTearing = {}; // Tearing-On --> VSync-Off
            }
            feature = {};

            struct Setting : cpp_lang_utils::EnableMasterPtr<DxgiFactoryInfo>
            {
                friend Renderer;

                using EnableMasterPtrType::EnableMasterPtrType;

            private:
                mutable UINT m_adapterIndex = {};

                mutable UINT m_syncInterval = {};

                mutable bool m_allowTearing = {};

            public:
                UINT adapterIndex() const;

                IDXGIAdapter* adapter() const;
                void setAdapter(UINT index) const;

                UINT syncInterval() const;
                void setSyncInterval(UINT count) const;

                bool allowTearing() const;
                void setAllowTearing(bool value) const;
            }
            setting{ this };
        }
        m_dxgiFactoryInfo{ this };

    public:
        IDXGIFactory6* dxgiFactory() const;

        const DxgiFactoryInfo& dxgiFactoryInfo() const;

    private:
        void createDxgiFactory();

        void queryDxgiFactoryInfo();

        // DXGI Factory Properties
        void queryDxgiFactoryProperties();
        void queryAvailableAdapters();

        // DXGI Factory Features
        void queryDxgiFactoryFeatures();
        void queryTearingSupport();

        void checkDxgiFactoryConfigs();
        void checkAdapterConfig();
        void checkSyncIntervalConfig();
        void checkTearingConfig();

        // DXGI Factory Settings
        void populateDxgiFactorySettings();

    private:
        ComPtr<ID3D12Device> m_d3d12Device = {};

        using OutputArray = std::vector<ComPtr<IDXGIOutput>>;

        using DisplayModeArray = std::vector<DXGI_MODE_DESC>;

        struct D3D12DeviceInfo : cpp_lang_utils::EnableMasterPtr<Renderer>
        {
            using EnableMasterPtrType::EnableMasterPtrType;

            struct Property
            {
                struct DescHandleSize
                {
                    UINT RTV = {}, DSV = {}, CBV_SRV_UAV = {};
                }
                descHandleSize = {};

                OutputArray availableOutputs = {};

                DisplayModeArray availableDisplayModes = {};
            }
            property = {};

            struct Feature : cpp_lang_utils::EnableMasterPtr<D3D12DeviceInfo>
            {
                using EnableMasterPtrType::EnableMasterPtrType;

                // Returns empty value if the GPU does not support such MSAA.
                Optional<UINT> queryMsaaQualityLevel(UINT sampleCount) const;

                D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSignature = {};
            }
            feature{ this };

            struct Setting : cpp_lang_utils::EnableMasterPtr<D3D12DeviceInfo>
            {
                friend Renderer;

                using EnableMasterPtrType::EnableMasterPtrType;

            private:
                mutable UINT m_outputIndex = {};

                mutable bool m_scaling = {};
                mutable UINT m_displayModeIndex = {};

                void updateDisplayMode() const;

            public:
                UINT outputIndex() const;

                IDXGIOutput* output() const;
                void setOutput(UINT index) const;

                bool scaling() const;
                void setScaling(bool value) const;

                UINT displayModeIndex() const;

                const DXGI_MODE_DESC& displayMode() const;
                void setDisplayMode(UINT index) const;
            }
            setting{ this };
        }
        m_d3d12DeviceInfo{ this };

    public:
        ID3D12Device* d3d12Device() const;

        const D3D12DeviceInfo& d3d12DeviceInfo() const;

    private:
        void createD3d12Device();

        void queryD3d12DeviceInfo();

        // D3D12 Device Properties
        void queryD3d12DeviceProperties();
        void queryDescHandleSizes();
        void queryAvailableOutputs();
        void queryAvailableDisplayModes();

        // D3D12 Device Features
        void queryD3d12DeviceFeatures();
        void queryRootSignatureFeature();

        void checkD3d12DeviceConfigs();
        void checkOutputConfig();
        void checkDisplayModeConfig();

        // D3D12 Device Settings
        void populateD3d12DeviceSettings();

    private:
        ComPtr<ID3D12Fence> m_fence = {};

        UINT64 m_fenceValue = 0;

    public:
        ID3D12Fence* fence() const;

        UINT64 fenceValue() const;

    private:
        void createFence();

    private:
        ComPtr<ID3D12CommandQueue> m_cmdQueue = {};

        ComPtr<ID3D12CommandAllocator> m_cmdAlloc = {};

        ComPtr<ID3D12GraphicsCommandList> m_cmdList = {};

    public:
        ID3D12CommandQueue* cmdQueue() const;

        ID3D12CommandAllocator* cmdAlloc() const;

        ID3D12GraphicsCommandList* cmdList() const;

    private:
        void createCommandObjects();

    private:
        using FrameResourceArray = FrameResource::Array<UniquePtr<FrameResource>>;

        FrameResourceArray m_frameResources = {};

        UINT m_currFrameIndex = 0;

    public:
        const FrameResourceArray& frameResources() const;

        UINT currFrameIndex() const;
        FrameResource* currFrameResource() const;

    private:
        void createFrameResources();

#pragma endregion

#pragma region D3D11On12 & D2D1 Components

    private:
        ComPtr<ID3D11On12Device> m_d3d11On12Device = {};

        ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext = {};

    public:
        ID3D11On12Device* d3d11On12Device() const;

        ID3D11DeviceContext* d3d11DeviceContext() const;

    private:
        void createD3d11On12Objects();

    private:
        ComPtr<ID2D1Factory1> m_d2d1Factory = {};

        ComPtr<ID2D1Device> m_d2d1Device = {};

        ComPtr<ID2D1DeviceContext> m_d2d1DeviceContext = {};

        ComPtr<IDWriteFactory3> m_dwriteFactory = {};

    public:
        ID2D1Factory1* d2d1Factory() const;

        ID2D1Device* d2d1Device() const;

        ID2D1DeviceContext* d2d1DeviceContext() const;

        IDWriteFactory3* dwriteFactory() const;

    private:
        void createD2d1Objects();

#pragma endregion

#pragma region DComposition Components

    private:
        bool m_composition = {};

        ComPtr<IDCompositionDevice> m_dcompDevice = {};

        ComPtr<IDCompositionTarget> m_dcompTarget = {};

        ComPtr<IDCompositionVisual> m_dcompVisual = {};

    public:
        bool composition() const;
        void setComposition(bool value);

        Optional<IDCompositionDevice*> dcompDevice() const;

        Optional<IDCompositionTarget*> dcompTarget() const;

        Optional<IDCompositionVisual*> dcompVisual() const;

    private:
        void createDcompObjects();

#pragma endregion

#pragma region Graphics Resources

    public:
        constexpr static DXGI_FORMAT g_renderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        constexpr static DXGI_FORMAT g_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    private:
        ComPtr<IDXGISwapChain3> m_swapChain = {};

    public:
        IDXGISwapChain3* swapChain() const;

    private:
        void createSwapChain();
        void resizeSwapChain();

    private:
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap = {};
        ComPtr<ID3D12DescriptorHeap> m_srvHeap = {};

    public:
        Optional<ID3D12DescriptorHeap*> rtvHeap() const;
        Optional<ID3D12DescriptorHeap*> srvHeap() const;

    private:
        void createRtvHeap();
        void createSrvHeap();

    public:
        Optional<D3D12_CPU_DESCRIPTOR_HANDLE> getRtvHandle(UINT offsetIndex) const;

    private:
        using BackBufferArray = FrameResource::Array<ComPtr<ID3D12Resource>>;

        BackBufferArray m_backBuffers = {};

        ComPtr<ID3D12Resource> m_sceneBuffer = {};

        ComPtr<ID3D11Resource> m_wrappedBuffer = {};

        ComPtr<ID2D1Bitmap1> m_renderTarget = {};

    public:
        Optional<ID3D12Resource*> getBackBuffer(UINT index) const;

        Optional<ID3D12Resource*> currBackBuffer() const;
        Optional<D3D12_CPU_DESCRIPTOR_HANDLE> backRtvHandle() const;

        UINT getSceneWidth() const;
        UINT getSceneHeight() const;

        Optional<ID3D12Resource*> sceneBuffer() const;

        Optional<D3D12_CPU_DESCRIPTOR_HANDLE> sceneRtvHandle() const;
        Optional<D3D12_CPU_DESCRIPTOR_HANDLE> sceneSrvhandle() const;

        Optional<ID3D11Resource*> wrappedBuffer() const;

        // The render target will be:
        // (composition=true) the first back buffer of the composition swap chain.
        // (composition=false) the current back buffer of the d3dCmdQueue swap chain.
        ID2D1Bitmap1* renderTarget() const;

    private:
        void createBackBuffers();

        void createSceneBuffer();

        void createWrappedBuffer();

        void createRenderTarget();

        // Releases all outstanding references to the back buffers of the swap chain.
        void clearSwapChainRefs();

#pragma endregion

#pragma region Render Pass Implementation

    public:
        void resetCmdList();
        void submitCmdList();
        void flushCmdQueue();

        // Wrap the GPU command code with begin/end,
        // otherwise it might cause the render pass to crash.
        void beginGpuCommand();
        void endGpuCommand();

        void waitGpuCommand();

    private:
        void waitCurrFrameResource();
        void update();
        void present();

        XMVECTORF32 m_sceneColor = {};

        void clearSceneBuffer();

        D2D1_COLOR_F m_layerColor = {};

        void clearLayerBuffer();

        void clearRenderTarget();

    public:
        // Skipping the updating helps optimize performance
        // if there is no need to play animation or similar things.
        bool skipUpdating = false;

        const XMVECTORF32& sceneColor() const;
        void setSceneColor(const XMVECTORF32& color);

        const D2D1_COLOR_F& layerColor() const;
        void setLayerColor(const D2D1_COLOR_F& color);

    private:
        UniquePtr<TickTimer> m_timer = {};

    public:
        TickTimer* timer() const;

    private:
        UniquePtr<Letterbox> m_letterbox = {};

    public:
        Optional<Letterbox*> letterbox() const;

    public:
        struct CommandLayer : ISortable<CommandLayer>
        {
            friend Renderer;

            explicit CommandLayer(ID3D12Device* device);

        public:
            bool enabled = true;

        public:
            using DrawObjectSet = ISortable<IDrawObject>::ShrdPrioritySet;

            using DrawObjectLayerMap = ISortable<IDrawLayer>::ShrdPriorityMap<DrawObjectSet>;

            using D3D12Target = DrawObjectLayerMap;

            using DrawObject2DSet = ISortable<IDrawObject2D>::ShrdPrioritySet;

            using D2D1Target = DrawObject2DSet;

            using DrawTarget = Variant<std::monostate, D3D12Target, D2D1Target>;

            DrawTarget drawTarget = {};

        private:
            FrameResource::CmdAllocArray m_cmdAllocs = {};

            void resetCmdList(ID3D12GraphicsCommandList* cmdList, size_t index);
        };

        using CommandLayerSet = ISortable<CommandLayer>::ShrdPrioritySet;

        CommandLayerSet cmdLayers = {};

    private:
        void drawD3d12Target(CommandLayer::D3D12Target& target);

        void drawD2d1Target(CommandLayer::D2D1Target& target);

#pragma endregion

#pragma region Miscellaneous

    public:
        // These Direct2D and DWrite rendering settings are initialized by
        // m_d2d1DeviceContext and m_dwriteFactory, so there is no related
        // config field provided in create-info (just set them at runtime).

        D2D1_ANTIALIAS_MODE getAntialiasMode2D() const;
        void setAntialiasMode2D(D2D1_ANTIALIAS_MODE mode);

        D2D1_TEXT_ANTIALIAS_MODE getTextAntialiasMode() const;
        void setTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE mode);

        struct TextRenderingSettings
        {
            FLOAT gamma = {};
            FLOAT enhancedContrast = {};
            FLOAT clearTypeLevel = {};
            DWRITE_PIXEL_GEOMETRY pixelGeometry = {};
            DWRITE_RENDERING_MODE renderingMode = {};
        };

        TextRenderingSettings getDefaultTextRenderingMode() const;
        void setTextRenderingMode(const TextRenderingSettings& mode);

#pragma endregion

    };
}
