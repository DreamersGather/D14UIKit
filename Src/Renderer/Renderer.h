#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/EnableMasterPtr.h"
#include "Common/Interfaces/ISortable.h"

#include "Renderer/FrameResource.h"

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
            // Def-resources (shader etc.) are loaded from this path.
            Wstring binaryPath = L"Bin/";

            Optional<float> dpi = std::nullopt;

            bool fullscreen = false;

            // Select GPU device.  Set as 0 to use the default one.
            UINT adapterIndex = 0;

            // The scene is resized to:
            // (True) fit the resolution of current display mode.
            // (False) fill the client area of the root Win32 window.
            bool resolutionScaling = false;

            // The scene is resized to fit the resolution
            // of specific display mode if enable resolution-scaling.
            UINT displayModeIndex = 0;

            // True/False ==> Disable/Enable Vertical-Synchronization.
            bool allowTearing = true;

            XMVECTORF32 sceneColor = Colors::White;
            XMVECTORF32 letterboxColor = Colors::Black;
        };

        Renderer(HWND window, const CreateInfo& info = {});

        virtual ~Renderer();

        // This field stores the original create-info passed in the ctor.
        const CreateInfo createInfo = {};

#pragma region Win32 Components

    private:
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
            bool fullscreen() const;

            const OriginalInfo& originalInfo() const;

            // These methods return directly if already in target mode.
            void enterFullscreenMode() const;
            void restoreWindowedMode() const;
        }
        m_window{ this };

    public:
        const Window& window() const;

        void onWindowResize(); // Call this in WM_SIZE.
        void renderNextFrame(); // Call this in WM_SIZE, WM_PAINT and main.

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
                bool allowTearing = {}; // Tearing-On == VSync-Off
            }
            feature = {};

            struct Setting : cpp_lang_utils::EnableMasterPtr<DxgiFactoryInfo>
            {
                friend Renderer;

                using EnableMasterPtrType::EnableMasterPtrType;

            private:
                mutable UINT m_currSelectedAdapterIndex = {};

            public:
                UINT currSelectedAdapterIndex() const;

                IDXGIAdapter* currSelectedAdapter() const;
                void selectAdapter(UINT index) const;

                bool allowTearing = {}; // Tearing-On == VSync-Off
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

        void queryDxgiFactoryProperties();
        void queryAvailableAdapters();

        void queryDxgiFactoryFeatures();
        void queryTearingSupport();

        void checkDxgiFactoryConfigs();

        void checkAdapterConfig();
        void checkTearingConfig();

        void populateDxgiFactorySettings();

    private:
        ComPtr<ID3D12Device> m_d3d12Device = {};

        using DisplayModeArray = std::vector<DXGI_MODE_DESC>;

        struct D3D12DeviceInfo : cpp_lang_utils::EnableMasterPtr<Renderer>
        {
            using EnableMasterPtrType::EnableMasterPtrType;

            struct Property
            {
                struct DescHandleIncrementSize
                {
                    UINT64 RTV = {}, DSV = {}, CBV_SRV_UAV = {};
                }
                descHandleIncrementSize = {};

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
                mutable bool m_resolutionScaling = false;
                mutable UINT m_currDisplayModeIndex = {};

            public:
                bool resolutionScaling() const;
                UINT currDisplayModeIndex() const;

                const DXGI_MODE_DESC& currDisplayMode() const;
                void setDisplayMode(bool scaling, UINT index) const;
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

        void queryD3d12DeviceProperties();
        void queryDescHandleIncrementSizes();
        void queryAvailableDisplayModes();

        void queryD3d12DeviceFeatures();
        void queryRootSignatureFeature();

        void checkD3d12DeviceConfigs();
        void checkDisplayModeConfig();

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
        ID3D12DescriptorHeap* rtvHeap() const;
        ID3D12DescriptorHeap* srvHeap() const;

    private:
        void createRtvHeap();
        void createSrvHeap();

    public:
        D3D12_CPU_DESCRIPTOR_HANDLE getRtvHandle(UINT offsetIndex) const;

    private:
        using BackBufferArray = FrameResource::Array<ComPtr<ID3D12Resource>>;

        BackBufferArray m_backBuffers = {};

        ComPtr<ID3D12Resource> m_sceneBuffer = {};

        ComPtr<ID3D11Resource> m_wrappedBuffer = {};

        ComPtr<ID2D1Bitmap1> m_d2d1RenderTarget = {};

    public:
        const BackBufferArray& backBuffers() const;

        ID3D12Resource* currBackBuffer() const;
        D3D12_CPU_DESCRIPTOR_HANDLE backRtvHandle() const;

        UINT getSceneWidth() const;
        UINT getSceneHeight() const;

        ID3D12Resource* sceneBuffer() const;

        D3D12_CPU_DESCRIPTOR_HANDLE sceneRtvHandle() const;
        D3D12_CPU_DESCRIPTOR_HANDLE sceneSrvhandle() const;

        ID3D11Resource* wrappedBuffer() const;

        ID2D1Bitmap1* d2d1RenderTarget() const;

    private:
        void createBackBuffers();

        void createSceneBuffer();

        void createWrappedBuffer();

        void clearInterpStates();

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

    public:
        // Skipping the updating helps optimize performance
        // if there is no need to play animation or similar things.
        bool skipUpdating = false;

        const XMVECTORF32& sceneColor() const;
        void setSceneColor(const XMVECTORF32& color);

    private:
        UniquePtr<TickTimer> m_timer = {};

    public:
        TickTimer* timer() const;

    private:
        UniquePtr<Letterbox> m_letterbox = {};

    public:
        Letterbox* letterbox() const;

    public:
        struct CommandLayer : ISortable<CommandLayer>
        {
            friend Renderer;

            explicit CommandLayer(ID3D12Device* device);

        public:
            bool enabled = true;

            void setPriority(int value);

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
