#include "Common/Precompile.h"

#include "Renderer/Renderer.h"

#include "Common/CppLangUtils/FinalAction.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/GDI.h"

#include "Renderer/GraphUtils/Barrier.h"
#include "Renderer/GraphUtils/Bitmap.h"
#include "Renderer/GraphUtils/ParamHelper.h"
#include "Renderer/Interfaces/IDrawLayer.h"
#include "Renderer/Interfaces/IDrawObject.h"
#include "Renderer/Interfaces/IDrawObject2D.h"
#include "Renderer/Letterbox.h"
#include "Renderer/TickTimer.h"

#ifdef _DEBUG
#include "Renderer/DebugUtils.h"
#endif

namespace d14engine::renderer
{
    Renderer::Renderer(HWND window, const CreateInfo& info)
        : createInfo(info), m_window(this, window)
    {
        graph_utils::bitmap::initialize();

        populateDxgiFactorySettings();
        populateD3d12DeviceSettings();

        m_sceneColor = createInfo.sceneColor;

        m_timer = std::make_unique<TickTimer>();
        
        createDxgiFactory();
        queryDxgiFactoryInfo();
        checkDxgiFactoryConfigs();

        m_dxgiFactoryInfo.setting.selectAdapter(createInfo.adapterIndex);

        // The letterbox will not be created until selectAdapter called.
        m_letterbox->color = createInfo.letterboxColor;

        if (createInfo.fullscreen) m_window.enterFullscreenMode();
    }

    Renderer::~Renderer()
    {
        waitGpuCommand();
    }

    Renderer::Window::Window(Renderer* master, HWND ptr)
        : EnableMasterPtrType(master), ptr(ptr) { }

    void Renderer::Window::onResize()
    {
        GetWindowRect(ptr, &m_windowRect);
        GetClientRect(ptr, &m_clientRect);
    }

    const RECT& Renderer::Window::clientRect() const
    {
        return m_clientRect;
    }

    UINT Renderer::Window::clientWidth() const
    {
        return math_utils::width(m_clientRect);
    }

    UINT Renderer::Window::clientHeight() const
    {
        return math_utils::height(m_clientRect);
    }

    const RECT& Renderer::Window::windowRect() const
    {
        return m_windowRect;
    }

    UINT Renderer::Window::windowWidth() const
    {
        return math_utils::width(m_windowRect);
    }

    UINT Renderer::Window::windowHeight() const
    {
        return math_utils::height(m_windowRect);
    }

    bool Renderer::Window::fullscreen() const
    {
        return m_fullscreen;
    }

    const Renderer::Window::OriginalInfo& Renderer::Window::originalInfo() const
    {
        return m_originalInfo;
    }

    void Renderer::Window::enterFullscreenMode() const
    {
        Renderer* rndr = m_master;
        THROW_IF_NULL(rndr);

        if (m_fullscreen) return;

        // Save these fields so they can be restored when exiting fullscreen.
        GetWindowRect(ptr, &m_originalInfo.windowRect);
        m_originalInfo.style = GetWindowLong(ptr, GWL_STYLE);

        // Make the window borderless so the client area can fill the screen.
        SetWindowLong(ptr, GWL_STYLE, WS_POPUP);

        RECT fullRect = {};
        // Try to query the size of current output screen from the swap chain.
        try
        {
            ComPtr<IDXGIOutput> output;
            THROW_IF_FAILED(rndr->m_swapChain->GetContainingOutput(&output));

            DXGI_OUTPUT_DESC outputDesc;
            THROW_IF_FAILED(output->GetDesc(&outputDesc));

            fullRect = outputDesc.DesktopCoordinates;
        }
        catch (...) // Call GDI interface to query it from the primary device.
        {
            DEVMODE devMode = {};
            devMode.dmSize = sizeof(DEVMODE);

            EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

            fullRect =
            {
                devMode.dmPosition.x,
                devMode.dmPosition.y,
                devMode.dmPosition.x + (LONG)devMode.dmPelsWidth,
                devMode.dmPosition.y + (LONG)devMode.dmPelsHeight
            };
        }

        SetWindowPos(
            ptr,
            HWND_TOPMOST,
            fullRect.left,
            fullRect.top,
            (int)math_utils::width(fullRect),
            (int)math_utils::height(fullRect),
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ShowWindow(ptr, SW_SHOW);

        m_fullscreen = true;
        GetWindowRect(ptr, &m_windowRect);
        GetClientRect(ptr, &m_clientRect);
    }

    void Renderer::Window::restoreWindowedMode() const
    {
        if (!m_fullscreen) return;

        SetWindowLong(ptr, GWL_STYLE, m_originalInfo.style);

        SetWindowPos(
            ptr,
            HWND_TOP,
            m_originalInfo.windowRect.left,
            m_originalInfo.windowRect.top,
            (int)math_utils::width(m_originalInfo.windowRect),
            (int)math_utils::height(m_originalInfo.windowRect),
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ShowWindow(ptr, SW_SHOW);

        m_fullscreen = false;
        GetWindowRect(ptr, &m_windowRect);
        GetClientRect(ptr, &m_clientRect);
    }

    const Renderer::Window& Renderer::window() const
    {
        return m_window;
    }

    void Renderer::onWindowResize()
    {
        m_window.onResize();

        flushCmdQueue();
        resetCmdList();
        clearInterpStates();

        resizeSwapChain();
        // The back buffer index will be reset after the swap chain resized.
        m_currFrameIndex = m_swapChain->GetCurrentBackBufferIndex();

        createBackBuffers();
        createSceneBuffer();
        createWrappedBuffer();

        submitCmdList();
        flushCmdQueue();

        if (m_d3d12DeviceInfo.setting.m_resolutionScaling)
        {
            auto& dispMode = m_d3d12DeviceInfo.setting.currDisplayMode();

            m_letterbox->resize(
                dispMode.Width,
                dispMode.Height,
                m_window.clientWidth(),
                m_window.clientHeight());
        }
    }

    void Renderer::renderNextFrame()
    {
        waitCurrFrameResource();

        currFrameResource()->resetCmdList(m_cmdList.Get());

        if (!skipUpdating)
        {
            update();
        }
        clearSceneBuffer();

        submitCmdList();

        for (auto& layer : cmdLayers)
        {
            if (layer->enabled)
            {
                layer->resetCmdList(m_cmdList.Get(), m_currFrameIndex);

                if (std::holds_alternative<CommandLayer::D3D12Target>(layer->drawTarget))
                {
                    drawD3d12Target(std::get<CommandLayer::D3D12Target>(layer->drawTarget));
                }
                if (std::holds_alternative<CommandLayer::D2D1Target>(layer->drawTarget))
                {
                    drawD2d1Target(std::get<CommandLayer::D2D1Target>(layer->drawTarget));
                }
                submitCmdList();
            }
        }
        present();

        m_timer->tick();
    }

    UINT Renderer::DxgiFactoryInfo::Setting::currSelectedAdapterIndex() const
    {
        return m_currSelectedAdapterIndex;
    }

    IDXGIAdapter* Renderer::DxgiFactoryInfo::Setting::currSelectedAdapter() const
    {
        DxgiFactoryInfo* info = m_master;
        THROW_IF_NULL(info);

        return info->property.availableAdapters.at(m_currSelectedAdapterIndex).Get();
    }

    void Renderer::DxgiFactoryInfo::Setting::selectAdapter(UINT index) const
    {
        DxgiFactoryInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        auto& target = info->setting.m_currSelectedAdapterIndex;
        auto operation = [&]
        {
            target = index;
            rndr->checkAdapterConfig();
        };
        cpp_lang_utils::restoreFromException(target, operation);

        rndr->createD3d12Device();
        rndr->queryD3d12DeviceInfo();
        rndr->checkD3d12DeviceConfigs();

        rndr->createFence();
        rndr->createCommandObjects();

        if (rndr->m_letterbox == nullptr)
        {
            rndr->m_letterbox = std::make_unique<Letterbox>(rndr, Letterbox::Token{});
        }
        rndr->resetCmdList();

        rndr->createFrameResources();

        rndr->createRtvHeap();
        if (rndr->m_d3d12DeviceInfo.setting.m_resolutionScaling) rndr->createSrvHeap();

        rndr->m_window.onResize();
        rndr->createSwapChain();

        rndr->createD3d11On12Objects();
        rndr->createD2d1Objects();

        if (rndr->m_d3d12DeviceInfo.setting.m_resolutionScaling)
        {
            rndr->m_letterbox->setEnabled(true);
        }
        else rndr->m_letterbox->setEnabled(false);

        rndr->submitCmdList();
        rndr->flushCmdQueue();

        rndr->m_timer->start();

        rndr->onWindowResize();
    }

    UINT Renderer::DxgiFactoryInfo::Setting::syncInterval() const
    {
        return m_syncInterval;
    }

    void Renderer::DxgiFactoryInfo::Setting::setSyncInterval(UINT count) const
    {
        DxgiFactoryInfo* info = m_master;
        THROW_IF_NULL(info);

        if (count > 4 || (count != 0 && info->setting.m_allowTearing))
        {
            return; // requires 0 ~ 4 and must be 0 when allow tearing
        }
        else m_syncInterval = count;
    }

    bool Renderer::DxgiFactoryInfo::Setting::allowTearing() const
    {
        return m_allowTearing;
    }

    void Renderer::DxgiFactoryInfo::Setting::setAllowTearing(bool value) const
    {
        DxgiFactoryInfo* info = m_master;
        THROW_IF_NULL(info);

        if (value && !info->feature.allowTearing)
        {
            return; // tearing not supported
        }
        else m_allowTearing = value;
    }

    IDXGIFactory6* Renderer::dxgiFactory() const
    {
        return m_dxgiFactory.Get();
    }

    const Renderer::DxgiFactoryInfo& Renderer::dxgiFactoryInfo() const
    {
        return m_dxgiFactoryInfo;
    }

    void Renderer::createDxgiFactory()
    {
        THROW_IF_FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_dxgiFactory)));
    }

    void Renderer::queryDxgiFactoryInfo()
    {
        queryDxgiFactoryProperties();
        queryDxgiFactoryFeatures();
    }

    void Renderer::queryDxgiFactoryProperties()
    {
        queryAvailableAdapters();
    }

    void Renderer::queryAvailableAdapters()
    {
        m_dxgiFactoryInfo.property.availableAdapters.clear();

        UINT adapterIndex = 0;
        ComPtr<IDXGIAdapter> adapter = {};
        while (m_dxgiFactory->EnumAdapters(adapterIndex++, &adapter) != DXGI_ERROR_NOT_FOUND)
        {
            m_dxgiFactoryInfo.property.availableAdapters.push_back(adapter);
        }
    }

    void Renderer::queryDxgiFactoryFeatures()
    {
        queryTearingSupport();
    }

    void Renderer::queryTearingSupport()
    {
        BOOL allowTearing = FALSE;

        HRESULT hr = m_dxgiFactory->CheckFeatureSupport(
            DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));

        // Make sure the result is returned by succeeded operation.
        m_dxgiFactoryInfo.feature.allowTearing = SUCCEEDED(hr) && allowTearing;
    }

    void Renderer::checkDxgiFactoryConfigs()
    {
        checkAdapterConfig();
        checkSyncIntervalConfig();
        checkTearingConfig();
    }

    void Renderer::checkAdapterConfig()
    {
        auto& availableAdpts = m_dxgiFactoryInfo.property.availableAdapters;
        auto& currSelectedAdptIndex = m_dxgiFactoryInfo.setting.m_currSelectedAdapterIndex;

        if (currSelectedAdptIndex >= availableAdpts.size())
        {
            auto descText = L"Selected adapter index out of range. A total of " +
                            std::to_wstring(availableAdpts.size()) +
                            L" adapters are available under current environment:\n";

            for (size_t i = 0; i < availableAdpts.size(); ++i)
            {
                DXGI_ADAPTER_DESC adptDesc;
                availableAdpts[i]->GetDesc(&adptDesc);
                descText += (L"Adapter " + std::to_wstring(i) + L": " + adptDesc.Description + L"\n");
            }
            THROW_ERROR(descText);
        }
    }

    void Renderer::checkSyncIntervalConfig()
    {
        if (m_dxgiFactoryInfo.setting.m_syncInterval > 4)
        {
            THROW_ERROR(L"Sync-Interval count must be selected from 0 ~ 4.");
        }
        if (m_dxgiFactoryInfo.setting.m_syncInterval != 0 &&
            m_dxgiFactoryInfo.setting.m_allowTearing)
        {
            THROW_ERROR(L"Sync-Interval count must be 0 when allow tearing.");
        }
    }

    void Renderer::checkTearingConfig()
    {
        if (m_dxgiFactoryInfo.setting.m_allowTearing && !m_dxgiFactoryInfo.feature.allowTearing)
        {
            THROW_ERROR(L"Tearing (a.k.a VSync/Off) support is not available.");
        }
    }

    void Renderer::populateDxgiFactorySettings()
    {
        auto& setting = m_dxgiFactoryInfo.setting;

        setting.m_currSelectedAdapterIndex = createInfo.adapterIndex;
        setting.m_syncInterval = createInfo.syncInterval;
        setting.m_allowTearing = createInfo.allowTearing;
    }

    Optional<UINT> Renderer::D3D12DeviceInfo::Feature::queryMsaaQualityLevel(UINT sampleCount) const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaInfo = {};
        msaaInfo.Format = g_renderTargetFormat;
        msaaInfo.SampleCount = sampleCount;
        msaaInfo.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        msaaInfo.NumQualityLevels = 0;

        THROW_IF_FAILED(rndr->m_d3d12Device->CheckFeatureSupport(
            D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaInfo, sizeof(msaaInfo)));

        if (msaaInfo.NumQualityLevels == 0) return std::nullopt;
        else return msaaInfo.NumQualityLevels - 1; // max-level == level-count - 1
    }

    bool Renderer::D3D12DeviceInfo::Setting::resolutionScaling() const
    {
        return m_resolutionScaling;
    }

    UINT Renderer::D3D12DeviceInfo::Setting::currDisplayModeIndex() const
    {
        return m_currDisplayModeIndex;
    }

    const DXGI_MODE_DESC& Renderer::D3D12DeviceInfo::Setting::currDisplayMode() const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        return info->property.availableDisplayModes.at(m_currDisplayModeIndex);
    }

    void Renderer::D3D12DeviceInfo::Setting::setDisplayMode(bool scaling, UINT index) const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        auto& target = info->setting.m_currDisplayModeIndex;
        auto operation = [&]
        {
            target = index;
            rndr->checkDisplayModeConfig();
            info->setting.m_resolutionScaling = scaling;
        };
        cpp_lang_utils::restoreFromException(target, operation);

        rndr->flushCmdQueue();
        rndr->resetCmdList();

        if (info->setting.m_resolutionScaling)
        {
            rndr->createSrvHeap();
            rndr->m_letterbox->setEnabled(true);
        }
        else rndr->m_letterbox->setEnabled(false);

        rndr->submitCmdList();
        rndr->flushCmdQueue();

        rndr->onWindowResize();
    }

    ID3D12Device* Renderer::d3d12Device() const
    {
        return m_d3d12Device.Get();
    }

    const Renderer::D3D12DeviceInfo& Renderer::d3d12DeviceInfo() const
    {
        return m_d3d12DeviceInfo;
    }

    void Renderer::createD3d12Device()
    {
#ifdef _DEBUG
        // Enable the debug layer in advance to debug device creating.
        debug_utils::enableD3d12DebugLayer();
#endif

        auto currAdpt = m_dxgiFactoryInfo.setting.currSelectedAdapter();

        THROW_IF_FAILED(D3D12CreateDevice(
            currAdpt, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_d3d12Device)));

#ifdef _DEBUG
        debug_utils::suppressWarnings(m_d3d12Device.Get());
#endif
    }

    void Renderer::queryD3d12DeviceInfo()
    {
        queryD3d12DeviceProperties();
        queryD3d12DeviceFeatures();
    }

    void Renderer::queryD3d12DeviceProperties()
    {
        queryDescHandleIncrementSizes();
        queryAvailableDisplayModes();
    }

    void Renderer::queryDescHandleIncrementSizes()
    {
        m_d3d12DeviceInfo.property.descHandleIncrementSize.RTV =
            m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        m_d3d12DeviceInfo.property.descHandleIncrementSize.DSV =
            m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

        m_d3d12DeviceInfo.property.descHandleIncrementSize.CBV_SRV_UAV =
            m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    void Renderer::queryAvailableDisplayModes()
    {
        auto currAdpt = m_dxgiFactoryInfo.setting.currSelectedAdapter();

        ComPtr<IDXGIOutput> output;
        // TODO: Support multiple display outputs.
        THROW_IF_FAILED(currAdpt->EnumOutputs(0, &output));

        UINT modeCount = 0;
        output->GetDisplayModeList(
            g_renderTargetFormat,
            0,
            &modeCount,
            nullptr);

        m_d3d12DeviceInfo.property.availableDisplayModes.resize(modeCount);
        output->GetDisplayModeList(
            g_renderTargetFormat,
            0,
            &modeCount,
            m_d3d12DeviceInfo.property.availableDisplayModes.data());
    }

    void Renderer::queryD3d12DeviceFeatures()
    {
        queryRootSignatureFeature();
    }

    void Renderer::queryRootSignatureFeature()
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSigInfo = {};
        rootSigInfo.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

        THROW_IF_FAILED(m_d3d12Device->CheckFeatureSupport(
            D3D12_FEATURE_ROOT_SIGNATURE,
            &rootSigInfo, sizeof(rootSigInfo)));

        auto& feature = m_d3d12DeviceInfo.feature.rootSignature;
        feature.HighestVersion = rootSigInfo.HighestVersion;
    }

    void Renderer::checkD3d12DeviceConfigs()
    {
        checkDisplayModeConfig();
    }

    void Renderer::checkDisplayModeConfig()
    {
        auto& currDispModeIndex = m_d3d12DeviceInfo.setting.m_currDisplayModeIndex;
        auto& availableDispModes = m_d3d12DeviceInfo.property.availableDisplayModes;

        if (currDispModeIndex >= availableDispModes.size())
        {            
            auto descText = L"Specified display mode not supported. A total of " +
                            std::to_wstring(availableDispModes.size()) +
                            L" display modes are available on current selected GPU device:\n";

            for (size_t n = 0; n < availableDispModes.size(); ++n)
            {
                auto& mode = availableDispModes[n];
                descText += (L"Display mode " + std::to_wstring(n) + L": "
                             L"width " + std::to_wstring(mode.Width) + L", "
                             L"height " + std::to_wstring(mode.Height) + L"\n");
            }
            THROW_ERROR(descText);
        }
    }

    void Renderer::populateD3d12DeviceSettings()
    {
        auto& setting = m_d3d12DeviceInfo.setting;

        setting.m_resolutionScaling = createInfo.resolutionScaling;
        setting.m_currDisplayModeIndex = createInfo.displayModeIndex;
    }

    ID3D12Fence* Renderer::fence() const
    {
        return m_fence.Get();
    }

    UINT64 Renderer::fenceValue() const
    {
        return m_fenceValue;
    }

    void Renderer::createFence()
    {
        THROW_IF_FAILED(m_d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    }

    ID3D12CommandQueue* Renderer::cmdQueue() const
    {
        return m_cmdQueue.Get();
    }

    ID3D12CommandAllocator* Renderer::cmdAlloc() const
    {
        return m_cmdAlloc.Get();
    }

    ID3D12GraphicsCommandList* Renderer::cmdList() const
    {
        return m_cmdList.Get();
    }

    void Renderer::createCommandObjects()
    {
        // Command Queue
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        THROW_IF_FAILED(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_cmdQueue)));

        // Command Allocator
        THROW_IF_FAILED(m_d3d12Device->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAlloc)));

        // Command List
        THROW_IF_FAILED(m_d3d12Device->CreateCommandList(
            0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&m_cmdList)));

        m_cmdList->Close(); // The command list must be closed before reset.
    }

    const Renderer::FrameResourceArray& Renderer::frameResources() const
    {
        return m_frameResources;
    }

    UINT Renderer::currFrameIndex() const
    {
        return m_currFrameIndex;
    }

    FrameResource* Renderer::currFrameResource() const
    {
        return m_frameResources.at(m_currFrameIndex).get();
    }

    void Renderer::createFrameResources()
    {
        for (auto& frameRes : m_frameResources)
        {
            frameRes = std::make_unique<FrameResource>(m_d3d12Device.Get());
        }
    }

    ID3D11On12Device* Renderer::d3d11On12Device() const
    {
        return m_d3d11On12Device.Get();
    }

    ID3D11DeviceContext* Renderer::d3d11DeviceContext() const
    {
        return m_d3d11DeviceContext.Get();
    }

    void Renderer::createD3d11On12Objects()
    {
        ComPtr<ID3D11Device> d3d11Device;
        THROW_IF_FAILED(D3D11On12CreateDevice
        (
            /* pDevice             */ m_d3d12Device.Get(),
            /* Flags               */ D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            /* pFeatureLevels      */ nullptr,
            /* FeatureLevels       */ 0,
            /* ppCommandQueues     */ (IUnknown**)m_cmdQueue.GetAddressOf(),
            /* NumQueues           */ 1,
            /* NodeMask            */ 0,
            /* ppDevice            */ &d3d11Device,
            /* ppImmediateContext  */ &m_d3d11DeviceContext,
            /* pChosenFeatureLevel */ nullptr
        ));
        THROW_IF_FAILED(d3d11Device.As(&m_d3d11On12Device));
    }

    ID2D1Factory1* Renderer::d2d1Factory() const
    {
        return m_d2d1Factory.Get();
    }

    ID2D1Device* Renderer::d2d1Device() const
    {
        return m_d2d1Device.Get();
    }

    ID2D1DeviceContext* Renderer::d2d1DeviceContext() const
    {
        return m_d2d1DeviceContext.Get();
    }

    IDWriteFactory3* Renderer::dwriteFactory() const
    {
        return m_dwriteFactory.Get();
    }

    void Renderer::createD2d1Objects()
    {
        D2D1_FACTORY_OPTIONS options = {};
#ifdef _DEBUG
        options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
        options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif
        // D2D1 Factory
        THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(m_d2d1Factory), &options, &m_d2d1Factory));

        // D2D1 Device
        ComPtr<IDXGIDevice> dxgiDevice;
        THROW_IF_FAILED(m_d3d11On12Device.As(&dxgiDevice));
        THROW_IF_FAILED(m_d2d1Factory->CreateDevice(dxgiDevice.Get(), &m_d2d1Device));

        // D2D1 Device Context
        THROW_IF_FAILED(m_d2d1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2d1DeviceContext));

        // DWrite Factory
        THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_dwriteFactory), &m_dwriteFactory));
    }

    IDXGISwapChain3* Renderer::swapChain() const
    {
        return m_swapChain.Get();
    }

    void Renderer::createSwapChain()
    {
        // The architecture follows the modern rendering mechanism in general,
        // and one of the most useful features is that DirectX 12 3D supports
        // uncapped FPS while keeping the screen not tearing, which is achieved
        // by placing the window into the sandbox (DWM, desktop window manager).
        //
        // To benefit the most from DX12, these configs are needed:
        //
        // 1. Always keep the render-target in windowed-mode.
        //    Use a borderless window to simulate the false fullscreen.
        //
        // 2. Init the swap chain in Flip-Mode (NO Blt-Mode).
        //
        // 3. Enable tearing (Vsync-Off) when it is supported.
        //
        // 4. Create multiple back buffers for the swap chain.

        DXGI_SWAP_CHAIN_DESC1 desc = {};

        // The resolution of the buffers follows the resolution of the window.
        // 
        // We will create a separate buffer outside the swap chain as the
        // intermediate render target, which will be resized to follow the
        // resolution of the actual scene.

        desc.Width = m_window.clientWidth();
        desc.Height = m_window.clientHeight();
        
        desc.Format = g_renderTargetFormat;

        // DirectX 12 3D does not support creating MSAA swap chain anymore.
        // In fact, it is the Flip-Mode that disables the direct MSAA usage,
        // and this is achieved by using DXGI_SWAP_EFFECT_FLIP_DISCARD.
        // 
        // In traditional methods, the back buffers of a MSAA swap chain will
        // be resolved automatically during presenting, but it is not allowed
        // in UWP programs.  The modern solution is to create an intermediate
        // MSAA render target.

        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;

        // Under current architecture, the renderer only supports the max
        // FPS of (MaxLatency - 1) * Monitor_RefreshRate, where the default
        // latency of 3 means the FPS can't go higher than 2 * RefreshRate,
        // so for a 60_Hz monitor the FPS can't go above 120 (enough though).

        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = (UINT)m_backBuffers.size();

        desc.Scaling = DXGI_SCALING_NONE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // NO direct MSAA!
        desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

        // To adjust the latency, FRAME_LATENCY_WAITABLE_OBJECT must be used.

        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH |
            DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

        if (m_dxgiFactoryInfo.feature.allowTearing)
        {
            desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        }
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsdesc = { .Windowed = TRUE };

        ComPtr<IDXGISwapChain1> tmpSwapChain;
        THROW_IF_FAILED(m_dxgiFactory->CreateSwapChainForHwnd(
            m_cmdQueue.Get(), m_window.ptr, &desc, &fsdesc, nullptr, &tmpSwapChain));

        THROW_IF_FAILED(tmpSwapChain.As(&m_swapChain));
        m_currFrameIndex = m_swapChain->GetCurrentBackBufferIndex();

        if (m_dxgiFactoryInfo.feature.allowTearing)
        {
            // IDXGIFactory::MakeWindowAssociation must be called on the
            // factory associated with the target HWND swap chain.
            // 
            // We can guarantee that by calling IDXGIObject::GetParent on the
            // swap chain to locate the factory.

            ComPtr<IDXGIFactory> tmpDxgiFactory;
            m_swapChain->GetParent(IID_PPV_ARGS(&tmpDxgiFactory));

            // When tearing is available we will handle "ALT+Enter" in
            // fnWndProc rather than let the factory set fullscreen state.

            THROW_IF_FAILED(tmpDxgiFactory->MakeWindowAssociation(
                m_window.ptr, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER));
        }
    }

    void Renderer::resizeSwapChain()
    {
        for (auto& buffer : m_backBuffers)
        {
            buffer.Reset();
        }
        DXGI_SWAP_CHAIN_DESC desc;
        m_swapChain->GetDesc(&desc);

        THROW_IF_FAILED(m_swapChain->ResizeBuffers
        (
            /* BufferCount    */ (UINT)m_backBuffers.size(),
            /* Width          */ m_window.clientWidth(),
            /* Height         */ m_window.clientHeight(),
            /* NewFormat      */ desc.BufferDesc.Format,
            /* SwapChainFlags */ desc.Flags
        ));
    }

    ID3D12DescriptorHeap* Renderer::rtvHeap() const
    {
        return m_rtvHeap.Get();
    }

    ID3D12DescriptorHeap* Renderer::srvHeap() const
    {
        return m_srvHeap.Get();
    }

    void Renderer::createRtvHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};

        // Back Buffers + Scene Buffer
        desc.NumDescriptors = (UINT)m_backBuffers.size() + 1;

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        THROW_IF_FAILED(m_d3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap)));
    }

    void Renderer::createSrvHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};

        // Scene Buffer
        desc.NumDescriptors = 1;

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        THROW_IF_FAILED(m_d3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_srvHeap)));
    }

    D3D12_CPU_DESCRIPTOR_HANDLE Renderer::getRtvHandle(UINT offsetIndex) const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
            (INT)offsetIndex,
            (UINT)m_d3d12DeviceInfo.property.descHandleIncrementSize.RTV);
    }

    const Renderer::BackBufferArray& Renderer::backBuffers() const
    {
        return m_backBuffers;
    }

    ID3D12Resource* Renderer::currBackBuffer() const
    {
        return m_backBuffers.at(m_currFrameIndex).Get();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE Renderer::backRtvHandle() const
    {
        return getRtvHandle(m_currFrameIndex);
    }

    UINT Renderer::getSceneWidth() const
    {
        if (m_d3d12DeviceInfo.setting.m_resolutionScaling)
        {
            return m_d3d12DeviceInfo.setting.currDisplayMode().Width;
        }
        else return m_window.clientWidth();
    }

    UINT Renderer::getSceneHeight() const
    {
        if (m_d3d12DeviceInfo.setting.m_resolutionScaling)
        {
            return m_d3d12DeviceInfo.setting.currDisplayMode().Height;
        }
        else return m_window.clientHeight();
    }

    ID3D12Resource* Renderer::sceneBuffer() const
    {
        return m_sceneBuffer.Get();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE Renderer::sceneRtvHandle() const
    {
        return getRtvHandle((UINT)m_backBuffers.size());
    }

    D3D12_CPU_DESCRIPTOR_HANDLE Renderer::sceneSrvhandle() const
    {
        return m_srvHeap->GetCPUDescriptorHandleForHeapStart();
    }

    ID3D11Resource* Renderer::wrappedBuffer() const
    {
        return m_wrappedBuffer.Get();
    }

    ID2D1Bitmap1* Renderer::d2d1RenderTarget() const
    {
        return m_d2d1RenderTarget.Get();
    }

    void Renderer::createBackBuffers()
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        for (UINT i = 0; i < m_backBuffers.size(); ++i)
        {
            THROW_IF_FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i])));

            m_d3d12Device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, handle);

            handle.Offset(1, (UINT)m_d3d12DeviceInfo.property.descHandleIncrementSize.RTV);
        }
    }

    void Renderer::createSceneBuffer()
    {
        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto desc = CD3DX12_RESOURCE_DESC::Tex2D
        (
            /* format    */ g_renderTargetFormat,
            /* width     */ getSceneWidth(),
            /* height    */ getSceneHeight(),
            /* arraySize */ 1,
            /* mipLevels */ 1
        );
        desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = g_renderTargetFormat;
        memcpy(clearValue.Color, m_sceneColor, 4 * sizeof(FLOAT));

        THROW_IF_FAILED(m_d3d12Device->CreateCommittedResource
        (
            /* pHeapProperties      */ &prop,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &desc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_COMMON,
            /* pOptimizedClearValue */ &clearValue,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&m_sceneBuffer)
        ));
        m_d3d12Device->CreateRenderTargetView
        (
            /* pResource      */ m_sceneBuffer.Get(),
            /* pDesc          */ nullptr,
            /* DestDescriptor */ sceneRtvHandle()
        );
        if (m_d3d12DeviceInfo.setting.m_resolutionScaling)
        {
            m_d3d12Device->CreateShaderResourceView
            (
                /* pResource      */ m_sceneBuffer.Get(),
                /* pDesc          */ nullptr,
                /* DestDescriptor */ sceneSrvhandle()
            );
        }
    }

    void Renderer::createWrappedBuffer()
    {
        FLOAT dpi = 96.0f;
        if (createInfo.dpi.has_value())
        {
            dpi = createInfo.dpi.value();
        }
        else dpi = (FLOAT)GetDpiForWindow(m_window.ptr);

        auto props = D2D1::BitmapProperties1
        (
            /* bitmapOptions */ D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            /* pixelFormat   */ D2D1::PixelFormat(g_renderTargetFormat, D2D1_ALPHA_MODE_PREMULTIPLIED),
            /* dpiX          */ dpi,
            /* dpiY          */ dpi
        );
        D3D11_RESOURCE_FLAGS flags = {};
        flags.BindFlags = D3D11_BIND_RENDER_TARGET;

        THROW_IF_FAILED(m_d3d11On12Device->CreateWrappedResource
        (
            /* pResource12  */ m_sceneBuffer.Get(),
            /* pFlags11     */ &flags,
            /* InState      */ D3D12_RESOURCE_STATE_COMMON,
            /* OutState     */ D3D12_RESOURCE_STATE_COMMON,
            /* riid         */      
            /* ppResource11 */ IID_PPV_ARGS(&m_wrappedBuffer)
        ));
        ComPtr<IDXGISurface> surface;
        THROW_IF_FAILED(m_wrappedBuffer.As(&surface));

        THROW_IF_FAILED(m_d2d1DeviceContext->CreateBitmapFromDxgiSurface
        (
            /* surface          */ surface.Get(),
            /* bitmapProperties */ &props,
            /* bitmap           */ &m_d2d1RenderTarget
        ));
    }

    void Renderer::clearInterpStates()
    {
        m_d2d1DeviceContext->SetTarget(nullptr);

        m_d3d11DeviceContext->ClearState();
        m_d3d11DeviceContext->Flush();
    }

    void Renderer::resetCmdList()
    {
        THROW_IF_FAILED(m_cmdAlloc->Reset());
        THROW_IF_FAILED(m_cmdList->Reset(m_cmdAlloc.Get(), nullptr));
    }

    void Renderer::submitCmdList()
    {
        THROW_IF_FAILED(m_cmdList->Close());
        ID3D12CommandList* ppCmdList[] = { m_cmdList.Get() };
        m_cmdQueue->ExecuteCommandLists(NUM_ARR_ARGS(ppCmdList));
    }

    void Renderer::flushCmdQueue()
    {
        ++m_fenceValue;
        THROW_IF_FAILED(m_cmdQueue->Signal(m_fence.Get(), m_fenceValue));

        if (m_fence->GetCompletedValue() < m_fenceValue)
        {
            HANDLE hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
            THROW_IF_NULL(hEvent);
            // m_fenceValue ==> wait all submitted commands
            THROW_IF_FAILED(m_fence->SetEventOnCompletion(m_fenceValue, hEvent));
            WaitForSingleObject(hEvent, INFINITE);
        }
    }

    void Renderer::beginGpuCommand()
    {
        m_d3d11DeviceContext->Flush();

        flushCmdQueue();
        resetCmdList();
    }

    void Renderer::endGpuCommand()
    {
        submitCmdList();
        flushCmdQueue();
    }

    void Renderer::waitGpuCommand()
    {
        m_d3d11DeviceContext->Flush();
        flushCmdQueue();
    }

    void Renderer::waitCurrFrameResource()
    {
        if (currFrameResource()->m_fenceValue != 0 &&
            m_fence->GetCompletedValue() < currFrameResource()->m_fenceValue)
        {
            HANDLE hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
            THROW_IF_NULL(hEvent);
            // FrameResource::m_fenceValue ==> wait commands submitted in previous render pass
            THROW_IF_FAILED(m_fence->SetEventOnCompletion(currFrameResource()->m_fenceValue, hEvent));
            WaitForSingleObject(hEvent, INFINITE);
            CloseHandle(hEvent);
        }
    }

    void Renderer::update()
    {
        for (auto& layer : cmdLayers)
        {
            if (std::holds_alternative<CommandLayer::D3D12Target>(layer->drawTarget))
            {
                for (auto& elem : std::get<CommandLayer::D3D12Target>(layer->drawTarget))
                {
                    if (elem.first->isD3d12LayerVisible())
                    {
                        elem.first->onRendererUpdateLayer(this);
                    }
                    for (auto& obj : elem.second)
                    {
                        if (obj->isD3d12ObjectVisible())
                        {
                            obj->onRendererUpdateObject(this);
                        }
                    }
                }
            }
            else if (std::holds_alternative<CommandLayer::D2D1Target>(layer->drawTarget))
            {
                for (auto& elem : std::get<CommandLayer::D2D1Target>(layer->drawTarget))
                {
                    if (elem->isD2d1ObjectVisible())
                    {
                        elem->onRendererUpdateObject2D(this);
                    }
                }
            }
        }
    }

    void Renderer::present()
    {
        m_letterbox->present();

        auto& syncInterval = m_dxgiFactoryInfo.setting.m_syncInterval;

        UINT presentFlags = 0;
        if (m_dxgiFactoryInfo.setting.m_allowTearing)
        {
            presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
        }
        THROW_IF_FAILED(m_swapChain->Present(syncInterval, presentFlags));

        currFrameResource()->m_fenceValue = ++m_fenceValue;
        THROW_IF_FAILED(m_cmdQueue->Signal(m_fence.Get(), m_fenceValue));

        m_currFrameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }

    void Renderer::clearSceneBuffer()
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition
        (
            m_sceneBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_RENDER_TARGET
        );
        m_cmdList->ResourceBarrier(1, &barrier);

        auto rtvHandle = sceneRtvHandle();
        m_cmdList->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);
        m_cmdList->ClearRenderTargetView(rtvHandle, m_sceneColor, 0, nullptr);

        graph_utils::revertBarrier(1, &barrier);
        m_cmdList->ResourceBarrier(1, &barrier);
    }

    const XMVECTORF32& Renderer::sceneColor() const
    {
        return m_sceneColor;
    }

    void Renderer::setSceneColor(const XMVECTORF32& color)
    {
        m_sceneColor = color;

        // Recreate the scene buffer with the new color to optimize clearing.

        flushCmdQueue();
        resetCmdList();
        clearInterpStates();

        createSceneBuffer();
        createWrappedBuffer();

        submitCmdList();
        flushCmdQueue();
    }

    TickTimer* Renderer::timer() const
    {
        return m_timer.get();
    }

    Letterbox* Renderer::letterbox() const
    {
        return m_letterbox.get();
    }

    Renderer::CommandLayer::CommandLayer(ID3D12Device* device)
    {
        for (auto& cmdAlloc : m_cmdAllocs)
        {
            THROW_IF_FAILED(device->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc)));
        }
    }

    void Renderer::CommandLayer::setPriority(int value)
    {
        m_priority = value;
    }

    void Renderer::CommandLayer::resetCmdList(ID3D12GraphicsCommandList* cmdList, size_t index)
    {
        auto& cmdAlloc = m_cmdAllocs.at(index);

        cmdAlloc->Reset();
        cmdList->Reset(cmdAlloc.Get(), nullptr);
    }

    void Renderer::drawD3d12Target(CommandLayer::D3D12Target& target)
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition
        (
            m_sceneBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_RENDER_TARGET
        );
        m_cmdList->ResourceBarrier(1, &barrier);

        for (auto& layer : target)
        {
            if (layer.first->isD3d12LayerVisible())
            {
                layer.first->onRendererDrawD3d12Layer(this);
            }
            for (auto& obj : layer.second)
            {
                if (obj->isD3d12ObjectVisible())
                {
                    obj->onRendererDrawD3d12Object(this);
                }
            }
        }
        graph_utils::revertBarrier(1, &barrier);
        m_cmdList->ResourceBarrier(1, &barrier);
    }

    void Renderer::drawD2d1Target(CommandLayer::D2D1Target& target)
    {
        for (auto& obj2d : target)
        {
            if (obj2d->isD2d1ObjectVisible())
            {
                obj2d->onRendererDrawD2d1Layer(this);
            }
        }
        m_d3d11On12Device->AcquireWrappedResources(m_wrappedBuffer.GetAddressOf(), 1);

        // It is recommended to call SetTarget before BeginDraw.
        // The program may crash if the previous target is a synchronized
        // resource and still bound to the context when calling BeginDraw.
        m_d2d1DeviceContext->SetTarget(m_d2d1RenderTarget.Get());

        m_d2d1DeviceContext->BeginDraw();
        m_d2d1DeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

        for (auto& obj2d : target)
        {
            if (obj2d->isD2d1ObjectVisible())
            {
                obj2d->onRendererDrawD2d1Object(this);
            }
        }
        THROW_IF_FAILED(m_d2d1DeviceContext->EndDraw());

        m_d3d11On12Device->ReleaseWrappedResources(m_wrappedBuffer.GetAddressOf(), 1);
        m_d3d11DeviceContext->Flush();
    }

    D2D1_ANTIALIAS_MODE Renderer::getAntialiasMode2D() const
    {
        return m_d2d1DeviceContext->GetAntialiasMode();
    }

    void Renderer::setAntialiasMode2D(D2D1_ANTIALIAS_MODE mode)
    {
        m_d2d1DeviceContext->SetAntialiasMode(mode);
    }

    D2D1_TEXT_ANTIALIAS_MODE Renderer::getTextAntialiasMode() const
    {
        return m_d2d1DeviceContext->GetTextAntialiasMode();
    }

    void Renderer::setTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE mode)
    {
        m_d2d1DeviceContext->SetTextAntialiasMode(mode);
    }

    Renderer::TextRenderingSettings Renderer::getDefaultTextRenderingMode() const
    {
        ComPtr<IDWriteRenderingParams> params;
        THROW_IF_FAILED(m_dwriteFactory->CreateRenderingParams(&params));

        return
        {
            params->GetGamma(),
            params->GetEnhancedContrast(),
            params->GetClearTypeLevel(),
            params->GetPixelGeometry(),
            params->GetRenderingMode()
        };
    }

    void Renderer::setTextRenderingMode(const TextRenderingSettings& mode)
    {
        ComPtr<IDWriteRenderingParams> params;
        THROW_IF_FAILED(m_dwriteFactory->CreateCustomRenderingParams
        (
            mode.gamma,
            mode.enhancedContrast,
            mode.clearTypeLevel,
            mode.pixelGeometry,
            mode.renderingMode,
            &params
        ));
        m_d2d1DeviceContext->SetTextRenderingParams(params.Get());
    }
}
