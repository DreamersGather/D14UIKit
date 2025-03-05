#include "Common/Precompile.h"

#include "Renderer/Renderer.h"

#include "Common/CppLangUtils/FinalAction.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/GDI.h"

#include "Renderer/GraphUtils/Barrier.h"
#include "Renderer/GraphUtils/Bitmap.h"
#include "Renderer/GraphUtils/ParamHelper.h"
#include "Renderer/InfoUtils.h"
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

        m_composition = createInfo.composition;

        m_sceneColor = createInfo.sceneColor;
        m_layerColor = createInfo.layerColor;

        createDxgiFactory();
        queryDxgiFactoryInfo();
        checkDxgiFactoryConfigs();

        auto& setting = m_dxgiFactoryInfo.setting;
        setting.setAdapter(createInfo.adapterIndex);

        m_window.setFullscreen(createInfo.fullscreen);

        m_timer = std::make_unique<TickTimer>();
    }

    Renderer::~Renderer()
    {
        waitGpuCommand();
    }

    RECT Renderer::queryDesktopRectGDI()
    {
        DEVMODE devMode = {};
        devMode.dmSize = sizeof(devMode);

        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);
        return
        {
            devMode.dmPosition.x,
            devMode.dmPosition.y,
            devMode.dmPosition.x + (LONG)devMode.dmPelsWidth,
            devMode.dmPosition.y + (LONG)devMode.dmPelsHeight
        };
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

    const Renderer::Window::OriginalInfo& Renderer::Window::originalInfo() const
    {
        return m_originalInfo;
    }

    bool Renderer::Window::fullscreen() const
    {
        return m_fullscreen;
    }

    void Renderer::Window::setFullscreen(bool value) const
    {
        if (m_fullscreen != value)
        {
            if (m_fullscreen = value)
            {
                Renderer* rndr = m_master;
                THROW_IF_NULL(rndr);

                // Save these fields so they can be restored when exiting fullscreen.
                GetWindowRect(ptr, &m_originalInfo.windowRect);
                m_originalInfo.style = (UINT)GetWindowLongPtr(ptr, GWL_STYLE);

                // Make the window borderless so the client area can fill the screen.
                SetWindowLongPtr(ptr, GWL_STYLE, WS_POPUP);

                RECT fullRect = {};
                if (rndr->m_composition)
                {
                    // Because GetContainingOutput is invalid for the swap chain
                    // created with IDXGIFactory2::CreateSwapChainForComposition,
                    // we have to directly query the desktop coordinate from GDI.
                    fullRect = queryDesktopRectGDI();
                }
                else // Query the size of current output screen from the swap chain.
                {
                    ComPtr<IDXGIOutput> output = {};
                    THROW_IF_FAILED(rndr->m_swapChain->GetContainingOutput(&output));

                    DXGI_OUTPUT_DESC outputDesc = {};
                    THROW_IF_FAILED(output->GetDesc(&outputDesc));

                    fullRect = outputDesc.DesktopCoordinates;
                }
                SetWindowPos
                (
                /* hWnd            */ ptr,
                /* hWndInsertAfter */ HWND_TOPMOST,
                /* X               */ fullRect.left,
                /* Y               */ fullRect.top,
                /* cx              */ (int)math_utils::width(fullRect),
                /* cy              */ (int)math_utils::height(fullRect),
                /* uFlags          */ SWP_FRAMECHANGED | SWP_NOACTIVATE
                );
                ShowWindow(ptr, SW_SHOW);

                GetWindowRect(ptr, &m_windowRect);
                GetClientRect(ptr, &m_clientRect);
            }
            else // windowed
            {
                SetWindowLongPtr(ptr, GWL_STYLE, m_originalInfo.style);

                SetWindowPos
                (
                /* hWnd            */ ptr,
                /* hWndInsertAfter */ HWND_TOP,
                /* X               */ m_originalInfo.windowRect.left,
                /* Y               */ m_originalInfo.windowRect.top,
                /* cx              */ (int)math_utils::width(m_originalInfo.windowRect),
                /* cy              */ (int)math_utils::height(m_originalInfo.windowRect),
                /* uFlags          */ SWP_FRAMECHANGED | SWP_NOACTIVATE
                );
                ShowWindow(ptr, SW_SHOW);

                GetWindowRect(ptr, &m_windowRect);
                GetClientRect(ptr, &m_clientRect);
            }
        }
    }

    const Renderer::Window& Renderer::window() const
    {
        return m_window;
    }

    void Renderer::onWindowResize()
    {
        m_window.onResize();

        resizeSwapChain();

        if (!m_composition && m_d3d12DeviceInfo.setting.m_scaling)
        {
            auto& dispMode = m_d3d12DeviceInfo.setting.displayMode();

            m_letterbox->resize
            (
            /* sceneWidth   */ dispMode.Width,
            /* sceneHeight  */ dispMode.Height,
            /* windowWidth  */ m_window.clientWidth(),
            /* windowHeight */ m_window.clientHeight()
            );
        }
    }

    void Renderer::renderNextFrame()
    {
        //-------------------------------------------------------------------------
        // Command Allocator Schedule
        //-------------------------------------------------------------------------
        // 1. Renderer: the global unique main allocator (Common)
        // 2. FrameResource: one allocator one frame-resource (Update)
        // 3. CommandLayer: g_bufferCount allocators per layer (Draw)
        //-------------------------------------------------------------------------
        // Note the difference in how FrameResource and CommandLayer manage them:
        //
        // Renderer
        //    |
        //    *--- FrameResource[3]
        //    |          |
        //    |          *--- CommandAllocator
        //    |
        //    *--- CommandLayer
        //              |
        //              *--- CommandAllocator[3]
        //
        // Each FrameResource only manages one CommandAllocator related to itself,
        // while CommandLayer manages all CommandAllocator (size = g_bufferCount)
        // that need to be reset in every render pass (for frame synchronization).
        //-------------------------------------------------------------------------

        waitCurrFrameResource();

        // Update Commands
        currFrameResource()->resetCmdList(m_cmdList.Get());

        if (!skipUpdating)
        {
            update();
        }
        clearRenderTarget();

        submitCmdList();

        for (auto& layer : cmdLayers)
        {
            if (layer->enabled)
            {
                // Draw Commands
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

    UINT Renderer::DxgiFactoryInfo::Setting::adapterIndex() const
    {
        return m_adapterIndex;
    }

    IDXGIAdapter* Renderer::DxgiFactoryInfo::Setting::adapter() const
    {
        DxgiFactoryInfo* info = m_master;
        THROW_IF_NULL(info);

        return info->property.availableAdapters.at(m_adapterIndex).Get();
    }

    void Renderer::DxgiFactoryInfo::Setting::setAdapter(UINT index) const
    {
        DxgiFactoryInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        auto& target = info->setting.m_adapterIndex;
        auto operation = [&]
        {
            target = index;
            rndr->checkAdapterConfig();
        };
        cpp_lang_utils::restoreFromException(target, operation);

        // The command queue has not be created at this point
        // for the case that setAdapter is called in Renderer's ctor.
        if (rndr->m_cmdQueue)
        {
            rndr->waitGpuCommand();
        }
        rndr->createD3d12Device();
        rndr->queryD3d12DeviceInfo();
        rndr->checkD3d12DeviceConfigs();

        rndr->createFence();
        rndr->createCommandObjects();
        rndr->createFrameResources();

        rndr->createD3d11On12Objects();
        rndr->createD2d1Objects();

        rndr->setComposition(rndr->m_composition);
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
#ifdef _DEBUG
        UINT flags = DXGI_CREATE_FACTORY_DEBUG;
#else
        UINT flags = 0;
#endif
        THROW_IF_FAILED(CreateDXGIFactory2
        (
        /* Flags     */ flags,
        /* riid      */
        /* ppFactory */ IID_PPV_ARGS(&m_dxgiFactory)
        ));
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
        BOOL allowTearing = {};

        THROW_IF_FAILED(m_dxgiFactory->CheckFeatureSupport
        (
        /* Feature                */ DXGI_FEATURE_PRESENT_ALLOW_TEARING,
        /* pFeatureSupportData    */ &allowTearing,
        /* FeatureSupportDataSize */ sizeof(allowTearing)
        ));
        m_dxgiFactoryInfo.feature.allowTearing = allowTearing;
    }

    void Renderer::checkDxgiFactoryConfigs()
    {
        checkAdapterConfig();
        checkSyncIntervalConfig();
        checkTearingConfig();
    }

    void Renderer::checkAdapterConfig()
    {
        auto& adapters = m_dxgiFactoryInfo.property.availableAdapters;
        auto& adapterIndex = m_dxgiFactoryInfo.setting.m_adapterIndex;

        if (adapterIndex >= adapters.size())
        {
            auto descText = // list available adapters
                L"Selected adapter index out of range. A total of " +
                std::to_wstring(adapters.size()) +
                L" adapters are available under current environment:\n";

            for (size_t n = 0; n < adapters.size(); ++n)
            {
                DXGI_ADAPTER_DESC desc = {};
                THROW_IF_FAILED(adapters[n]->GetDesc(&desc));

                descText += (L"Adapter " + std::to_wstring(n) + L": " + info_utils::text(desc) + L"\n");
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
        if (m_dxgiFactoryInfo.setting.m_allowTearing &&
           !m_dxgiFactoryInfo.feature.allowTearing)
        {
            THROW_ERROR(L"Tearing (required for VRR displays) is not supported.");
        }
    }

    void Renderer::populateDxgiFactorySettings()
    {
        auto& setting = m_dxgiFactoryInfo.setting;

        setting.m_adapterIndex = createInfo.adapterIndex;
        setting.m_syncInterval = createInfo.syncInterval;
        setting.m_allowTearing = createInfo.allowTearing;
    }

    Optional<UINT> Renderer::D3D12DeviceInfo::Feature::queryMsaaQualityLevel(UINT sampleCount) const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaInfo =
        {
            .Format      = g_renderTargetFormat,
            .SampleCount = sampleCount,
        };
        THROW_IF_FAILED(rndr->m_d3d12Device->CheckFeatureSupport
        (
        /* Feature                */ D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        /* pFeatureSupportData    */ &msaaInfo,
        /* FeatureSupportDataSize */ sizeof(msaaInfo)
        ));
        if (msaaInfo.NumQualityLevels == 0)
        {
            return std::nullopt;
        }
        // MSAA max-level = (level-count - 1)
        return msaaInfo.NumQualityLevels - 1;
    }

    void Renderer::D3D12DeviceInfo::Setting::updateDisplayMode() const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        if (!rndr->m_composition)
        {
            if (info->setting.m_scaling)
            {
                rndr->beginGpuCommand();

                rndr->createSrvHeap();
                rndr->m_letterbox->setEnabled(true);

                rndr->endGpuCommand();
            }
            else // free scaling mode
            {
                rndr->waitGpuCommand();

                rndr->m_srvHeap.Reset();
                rndr->m_letterbox->setEnabled(false);
            }
            rndr->onWindowResize();
        }
    }

    UINT Renderer::D3D12DeviceInfo::Setting::outputIndex() const
    {
        return m_outputIndex;
    }

    IDXGIOutput* Renderer::D3D12DeviceInfo::Setting::output() const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        return info->property.availableOutputs.at(m_outputIndex).Get();
    }

    void Renderer::D3D12DeviceInfo::Setting::setOutput(UINT index) const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        auto& target = info->setting.m_outputIndex;
        auto operation = [&]
        {
            target = index;
            rndr->checkOutputConfig();
        };
        cpp_lang_utils::restoreFromException(target, operation);

        rndr->queryAvailableDisplayModes();

        setScaling(false);
        setDisplayMode(0);
    }

    bool Renderer::D3D12DeviceInfo::Setting::scaling() const
    {
        return m_scaling;
    }

    void Renderer::D3D12DeviceInfo::Setting::setScaling(bool value) const
    {
        m_scaling = value;

        updateDisplayMode();
    }

    UINT Renderer::D3D12DeviceInfo::Setting::displayModeIndex() const
    {
        return m_displayModeIndex;
    }

    const DXGI_MODE_DESC& Renderer::D3D12DeviceInfo::Setting::displayMode() const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        return info->property.availableDisplayModes.at(m_displayModeIndex);
    }

    void Renderer::D3D12DeviceInfo::Setting::setDisplayMode(UINT index) const
    {
        D3D12DeviceInfo* info = m_master;
        THROW_IF_NULL(info);

        Renderer* rndr = info->m_master;
        THROW_IF_NULL(rndr);

        auto& target = info->setting.m_displayModeIndex;
        auto operation = [&]
        {
            target = index;
            rndr->checkDisplayModeConfig();
        };
        cpp_lang_utils::restoreFromException(target, operation);

        updateDisplayMode();
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
        auto adapter = m_dxgiFactoryInfo.setting.adapter();

        THROW_IF_FAILED(D3D12CreateDevice
        (
        /* pAdapter            */ adapter,
        /* MinimumFeatureLevel */ D3D_FEATURE_LEVEL_12_0,
        /* riid                */
        /* ppDevice            */ IID_PPV_ARGS(&m_d3d12Device)
        ));
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
        queryDescHandleSizes();
        queryAvailableOutputs();
        queryAvailableDisplayModes();
    }

    void Renderer::queryDescHandleSizes()
    {
#define GET_DESC_HANDLE_SIZE(Type) do \
{ \
    m_d3d12DeviceInfo.property.descHandleSize.Type = \
    m_d3d12Device->GetDescriptorHandleIncrementSize \
    (D3D12_DESCRIPTOR_HEAP_TYPE_##Type); \
} while (0)

        GET_DESC_HANDLE_SIZE(RTV);
        GET_DESC_HANDLE_SIZE(DSV);
        GET_DESC_HANDLE_SIZE(CBV_SRV_UAV);

#undef GET_DESC_HANDLE_SIZE
    }

    void Renderer::queryAvailableOutputs()
    {
        auto adapter = m_dxgiFactoryInfo.setting.adapter();

        UINT outputIndex = 0;
        ComPtr<IDXGIOutput> output = {};
        while (adapter->EnumOutputs(outputIndex++, &output) != DXGI_ERROR_NOT_FOUND)
        {
            m_d3d12DeviceInfo.property.availableOutputs.push_back(output);
        }
    }

    void Renderer::queryAvailableDisplayModes()
    {
        auto output = m_d3d12DeviceInfo.setting.output();

        UINT modeCount = 0;
        THROW_IF_FAILED(output->GetDisplayModeList
        (
        /* EnumFormat */ g_renderTargetFormat,
        /* Flags      */ 0,
        /* pNumModes  */ &modeCount,
        /* pDesc      */ nullptr
        ));
        auto& modes = m_d3d12DeviceInfo.property.availableDisplayModes;
        modes.resize(modeCount);

        THROW_IF_FAILED(output->GetDisplayModeList
        (
        /* EnumFormat */ g_renderTargetFormat,
        /* Flags      */ 0,
        /* pNumModes  */ &modeCount,
        /* pDesc      */ modes.data()
        ));
    }

    void Renderer::queryD3d12DeviceFeatures()
    {
        queryRootSignatureFeature();
    }

    void Renderer::queryRootSignatureFeature()
    {
        D3D12_FEATURE_DATA_ROOT_SIGNATURE rootSigInfo =
        {
            .HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0
        };
        THROW_IF_FAILED(m_d3d12Device->CheckFeatureSupport
        (
        /* Feature                */ D3D12_FEATURE_ROOT_SIGNATURE,
        /* pFeatureSupportData    */ &rootSigInfo,
        /* FeatureSupportDataSize */ sizeof(rootSigInfo)
        ));
        auto& feature = m_d3d12DeviceInfo.feature.rootSignature;
        feature.HighestVersion = rootSigInfo.HighestVersion;
    }

    void Renderer::checkD3d12DeviceConfigs()
    {
        checkOutputConfig();
        checkDisplayModeConfig();
    }

    void Renderer::checkOutputConfig()
    {
        auto& outputIndex = m_d3d12DeviceInfo.setting.m_outputIndex;
        auto& availableOutputs = m_d3d12DeviceInfo.property.availableOutputs;

        if (outputIndex >= availableOutputs.size())
        {
            auto descText = // list available outputs
                L"Specified output does not exist. A total of " +
                std::to_wstring(availableOutputs.size()) +
                L" outputs are available on current selected GPU device:\n";

            for (size_t n = 0; n < availableOutputs.size(); ++n)
            {
                auto& output = availableOutputs[n];

                DXGI_OUTPUT_DESC desc = {};
                THROW_IF_FAILED(output->GetDesc(&desc));

                descText += (L"Output " + std::to_wstring(n) + L": " + info_utils::text(desc) + L"\n");
            }
            THROW_ERROR(descText);
        }
    }

    void Renderer::checkDisplayModeConfig()
    {
        auto& displayModeIndex = m_d3d12DeviceInfo.setting.m_displayModeIndex;
        auto& availableDisplayModes = m_d3d12DeviceInfo.property.availableDisplayModes;

        if (displayModeIndex >= availableDisplayModes.size())
        {
            auto descText = // list available display modes
                L"Specified display mode not supported. A total of " +
                std::to_wstring(availableDisplayModes.size()) +
                L" display modes are available on current selected GPU device:\n";

            for (size_t n = 0; n < availableDisplayModes.size(); ++n)
            {
                auto& mode = availableDisplayModes[n];
                descText += (L"DisplayMode " + std::to_wstring(n) + L": " + info_utils::text(mode) + L"\n");
            }
            THROW_ERROR(descText);
        }
    }

    void Renderer::populateD3d12DeviceSettings()
    {
        auto& setting = m_d3d12DeviceInfo.setting;

        setting.m_outputIndex = createInfo.outputIndex;
        setting.m_scaling = createInfo.scaling;
        setting.m_displayModeIndex = createInfo.displayModeIndex;
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
        THROW_IF_FAILED(m_d3d12Device->CreateFence
        (
        /* InitialValue */ m_fenceValue,
        /* Flags        */ D3D12_FENCE_FLAG_NONE,
        /* riid         */
        /* ppFence      */ IID_PPV_ARGS(&m_fence)
        ));
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
        D3D12_COMMAND_QUEUE_DESC desc =
        {
            .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
            .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE
        };
        THROW_IF_FAILED(m_d3d12Device->CreateCommandQueue
        (
        /* pDesc          */ &desc,
        /* riid           */
        /* ppCommandQueue */ IID_PPV_ARGS(&m_cmdQueue)
        ));
        THROW_IF_FAILED(m_d3d12Device->CreateCommandAllocator
        (
        /* type               */ desc.Type,
        /* riid               */
        /* ppCommandAllocator */ IID_PPV_ARGS(&m_cmdAlloc)
        ));
        THROW_IF_FAILED(m_d3d12Device->CreateCommandList
        (
        /* nodeMask          */ 0,
        /* type              */ desc.Type,
        /* pCommandAllocator */ m_cmdAlloc.Get(),
        /* pInitialState     */ nullptr,
        /* riid              */
        /* ppCommandList     */ IID_PPV_ARGS(&m_cmdList)
        ));
        // The command list must be closed before reset.
        THROW_IF_FAILED(m_cmdList->Close());
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
        // Required for Direct2D interoperability with Direct3D resources.
        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        ComPtr<ID3D11Device> d3d11Device = {};
        THROW_IF_FAILED(D3D11On12CreateDevice
        (
        /* pDevice             */ m_d3d12Device.Get(),
        /* Flags               */ flags,
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
        THROW_IF_FAILED(D2D1CreateFactory
        (
        /* factoryType     */ D2D1_FACTORY_TYPE_SINGLE_THREADED,
        /* riid            */ __uuidof(m_d2d1Factory),
        /* pFactoryOptions */ &options,
        /* ppIFactory      */ &m_d2d1Factory)
        );
        ComPtr<IDXGIDevice> dxgiDevice = {};
        THROW_IF_FAILED(m_d3d11On12Device.As(&dxgiDevice));
        THROW_IF_FAILED(m_d2d1Factory->CreateDevice
        (
        /* dxgiDevice */ dxgiDevice.Get(),
        /* d2dDevice  */ &m_d2d1Device)
        );
        THROW_IF_FAILED(m_d2d1Device->CreateDeviceContext
        (
        /* options       */ D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        /* deviceContext */ &m_d2d1DeviceContext)
        );
        THROW_IF_FAILED(DWriteCreateFactory
        (
        /* factoryType */ DWRITE_FACTORY_TYPE_SHARED,
        /* iid         */ __uuidof(m_dwriteFactory),
        /* factory     */ &m_dwriteFactory)
        );
    }

    bool Renderer::composition() const
    {
        return m_composition;
    }

    void Renderer::setComposition(bool value)
    {
        // Depending on the user-specified composition flag (configurable),
        // the back buffer will be obtained from different swap chains,
        // so the objects that need to be set (initialized) also differ:
        //
        // 1. When composition=true, the back buffer is obtained from
        // the composition swap chain, and dcompObjects need to be created.
        //
        // 2. When composition=false, the back buffer is obtained from
        // the d3d12CmdQueue swap chain, where letterBox needs to be set;
        // rtvHeap, srvHeap, sceneBuffer, wrappedBuffer need to be created.

        if (m_composition = value)
        {
            waitGpuCommand();

            createDcompObjects();

            m_rtvHeap.Reset();
            m_srvHeap.Reset();

            m_letterbox.reset();
        }
        else // self-maintained back buffers
        {
            beginGpuCommand();

            m_dcompDevice.Reset();
            m_dcompVisual.Reset();
            m_dcompTarget.Reset();

            createRtvHeap();
            if (m_d3d12DeviceInfo.setting.m_scaling)
            {
                createSrvHeap();
            }
            else m_srvHeap.Reset();

            m_letterbox = std::make_unique<Letterbox>(this, Letterbox::Token{});
            m_letterbox->setEnabled(m_d3d12DeviceInfo.setting.m_scaling);

            endGpuCommand();
        }
        // Ensure the swap chain is created with the actual window size.
        m_window.onResize(); createSwapChain(); onWindowResize();
    }

    Optional<IDCompositionDevice*> Renderer::dcompDevice() const
    {
        if (m_composition)
        {
            return m_dcompDevice.Get();
        }
        return std::nullopt;
    }

    Optional<IDCompositionTarget*> Renderer::dcompTarget() const
    {
        if (m_composition)
        {
            return m_dcompTarget.Get();
        }
        return std::nullopt;
    }

    Optional<IDCompositionVisual*> Renderer::dcompVisual() const
    {
        if (m_composition)
        {
            return m_dcompVisual.Get();
        }
        return std::nullopt;
    }

    void Renderer::createDcompObjects()
    {
        // FIXME: DCompositionCreateDevice(D3D11On12Device) throws a warning,
        // but the application runs normally. The reason is currently unclear.

        ComPtr<IDXGIDevice> dxgiDevice = {};
        THROW_IF_FAILED(m_d3d11On12Device.As(&dxgiDevice));
        THROW_IF_FAILED(DCompositionCreateDevice
        (
        /* dxgiDevice         */ dxgiDevice.Get(),
        /* iid                */
        /* dcompositionDevice */ IID_PPV_ARGS(&m_dcompDevice)
        ));
        THROW_IF_FAILED(m_dcompDevice->CreateTargetForHwnd
        (
        /* hwnd    */ m_window.ptr,
        /* topmost */ TRUE,
        /* target  */ &m_dcompTarget)
        );
        THROW_IF_FAILED(m_dcompDevice->CreateVisual(&m_dcompVisual));
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
        // 3. Enable tearing (required for VRR) when it is supported.
        //
        // 4. Create multiple back buffers for the swap chain.

        DXGI_SWAP_CHAIN_DESC1 desc = {};

        // The resolution of the buffers follows the resolution of the window.
        //
        // If this is a d3d12CmdQueue swap chain, we will also maintain
        // a separate buffer outside the swap chain as the intermediate target,
        // which will be resized to follow the resolution of the actual scene.

        desc.Width = m_window.clientWidth();
        desc.Height = m_window.clientHeight();

        desc.Format = g_renderTargetFormat;

        // DirectX 12 does not support creating MSAA swap chain anymore.
        // In fact, it is the Flip-Mode that disables the multisampling,
        // and this is restricted by using DXGI_SWAP_EFFECT_FLIP* flags.
        // The modern solution is to create an intermediate MSAA buffer.

        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;

        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = FrameResource::g_bufferCount;

        if (m_composition)
        {
            desc.Scaling = DXGI_SCALING_STRETCH;
            desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
        }
        else // d3d12CmdQueue swap chain
        {
            desc.Scaling = DXGI_SCALING_NONE;
            desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        }

        if (m_dxgiFactoryInfo.feature.allowTearing)
        {
            desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        }
        // DirectX does not provide an interface to create IDXGISwapChainX (where X > 1),
        // so here we first create IDXGISwapChain1 and then forward it to the higher one.
        ComPtr<IDXGISwapChain1> swapChain = {};

        clearSwapChainRefs();
        m_swapChain.Reset();

        // Deferred Destruction Issues with Flip Presentation Swap Chains:
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-flush
        //
        // Call ID3D11DeviceContext::ClearState/Flush to force destroying the swap chain.

        m_d3d11DeviceContext->ClearState();
        m_d3d11DeviceContext->Flush();

        if (m_composition)
        {
            ComPtr<IDXGIDevice> dxgiDevice = {};
            THROW_IF_FAILED(m_d3d11On12Device.As(&dxgiDevice));
            THROW_IF_FAILED(m_dxgiFactory->CreateSwapChainForComposition
            (
            /* pDevice           */ dxgiDevice.Get(),
            /* pDesc             */ &desc,
            /* pRestrictToOutput */ nullptr,
            /* ppSwapChain       */ &swapChain
            ));
        }
        else // d3d12CmdQueue swap chain
        {
            THROW_IF_FAILED(m_dxgiFactory->CreateSwapChainForHwnd
            (
            /* pDevice           */ m_cmdQueue.Get(),
            /* hWnd              */ m_window.ptr,
            /* pDesc             */ &desc,
            /* pFullscreenDesc   */ nullptr,
            /* pRestrictToOutput */ nullptr,
            /* ppSwapChain       */ &swapChain
            ));
        }
        THROW_IF_FAILED(swapChain.As(&m_swapChain));
        m_currFrameIndex = m_swapChain->GetCurrentBackBufferIndex();

        if (!m_composition && m_dxgiFactoryInfo.feature.allowTearing)
        {
            // IDXGIFactory::MakeWindowAssociation can be called only on the
            // factory associated with the target d3d12CmdQueue swap chain,
            // and we could guarantee that by calling IDXGIObject::GetParent
            // of the target swap chain to locate the correct factory object.

            ComPtr<IDXGIFactory> dxgiFactory = {};
            m_swapChain->GetParent(IID_PPV_ARGS(&dxgiFactory));

            // When tearing is supported we will handle "ALT+Enter" in
            // fnWndProc rather than let the factory set fullscreen state.

            THROW_IF_FAILED(dxgiFactory->MakeWindowAssociation(
                m_window.ptr, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER));
        }
    }

    void Renderer::resizeSwapChain()
    {
        waitGpuCommand();

        // You can't resize a swap chain unless you release
        // all outstanding references to its back buffers.
        clearSwapChainRefs();
        m_d3d11DeviceContext->ClearState();

        DXGI_SWAP_CHAIN_DESC desc = {};
        THROW_IF_FAILED(m_swapChain->GetDesc(&desc));

        THROW_IF_FAILED(m_swapChain->ResizeBuffers
        (
        /* BufferCount    */ FrameResource::g_bufferCount,
        /* Width          */ m_window.clientWidth(),
        /* Height         */ m_window.clientHeight(),
        /* NewFormat      */ desc.BufferDesc.Format,
        /* SwapChainFlags */ desc.Flags
        ));
        // The back buffer index will be reset after the swap chain resized.
        m_currFrameIndex = m_swapChain->GetCurrentBackBufferIndex();

        if (m_composition)
        {
            m_sceneBuffer.Reset();
            m_wrappedBuffer.Reset();

            createRenderTarget();
        }
        else // self-maintained back buffers
        {
            createBackBuffers();
            createSceneBuffer();
            // m_renderTarget will be recreated with the wrapped buffer.
            createWrappedBuffer();
        }
    }

    Optional<ID3D12DescriptorHeap*> Renderer::rtvHeap() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_rtvHeap.Get();
    }

    Optional<ID3D12DescriptorHeap*> Renderer::srvHeap() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_srvHeap.Get();
    }

    void Renderer::createRtvHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc =
        {
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
            // Back Buffers + Scene Buffer
            .NumDescriptors = (UINT)m_backBuffers.size() + 1
        };
        THROW_IF_FAILED(m_d3d12Device->CreateDescriptorHeap
        (
        /* pDescriptorHeapDesc */ &desc,
        /* riid                */
        /* ppvHeap             */ IID_PPV_ARGS(&m_rtvHeap)
        ));
    }

    void Renderer::createSrvHeap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc =
        {
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
            .NumDescriptors = 1, // Scene Buffer
            .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
        };
        THROW_IF_FAILED(m_d3d12Device->CreateDescriptorHeap
        (
        /* pDescriptorHeapDesc */ &desc,
        /* riid                */
        /* ppvHeap             */ IID_PPV_ARGS(&m_srvHeap)
        ));
    }

    Optional<D3D12_CPU_DESCRIPTOR_HANDLE> Renderer::getRtvHandle(UINT offsetIndex) const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return CD3DX12_CPU_DESCRIPTOR_HANDLE
        (
        /* other                   */ m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        /* offsetInDescriptors     */ (INT)offsetIndex,
        /* descriptorIncrementSize */ (UINT)m_d3d12DeviceInfo.property.descHandleSize.RTV
        );
    }

    Optional<ID3D12Resource*> Renderer::getBackBuffer(UINT index) const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_backBuffers.at(index).Get();
    }

    Optional<ID3D12Resource*> Renderer::currBackBuffer() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_backBuffers.at(m_currFrameIndex).Get();
    }

    Optional<D3D12_CPU_DESCRIPTOR_HANDLE> Renderer::backRtvHandle() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return getRtvHandle(m_currFrameIndex);
    }

    UINT Renderer::getSceneWidth() const
    {
        if (m_d3d12DeviceInfo.setting.m_scaling)
        {
            return m_d3d12DeviceInfo.setting.displayMode().Width;
        }
        else return m_window.clientWidth();
    }

    UINT Renderer::getSceneHeight() const
    {
        if (m_d3d12DeviceInfo.setting.m_scaling)
        {
            return m_d3d12DeviceInfo.setting.displayMode().Height;
        }
        else return m_window.clientHeight();
    }

    Optional<ID3D12Resource*> Renderer::sceneBuffer() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_sceneBuffer.Get();
    }

    Optional<D3D12_CPU_DESCRIPTOR_HANDLE> Renderer::sceneRtvHandle() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return getRtvHandle((UINT)m_backBuffers.size());
    }

    Optional<D3D12_CPU_DESCRIPTOR_HANDLE> Renderer::sceneSrvhandle() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_srvHeap->GetCPUDescriptorHandleForHeapStart();
    }

    Optional<ID3D11Resource*> Renderer::wrappedBuffer() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_wrappedBuffer.Get();
    }

    ID2D1Bitmap1* Renderer::renderTarget() const
    {
        return m_renderTarget.Get();
    }

    void Renderer::createBackBuffers()
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        for (UINT i = 0; i < m_backBuffers.size(); ++i)
        {
            THROW_IF_FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i])));

            m_d3d12Device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, handle);

            handle.Offset(1, (UINT)m_d3d12DeviceInfo.property.descHandleSize.RTV);
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
        // sceneRtvHandle is guaranteed to be valid when composition=false
        m_d3d12Device->CreateRenderTargetView
        (
        /* pResource      */ m_sceneBuffer.Get(),
        /* pDesc          */ nullptr,
        /* DestDescriptor */ sceneRtvHandle().value()
        );
        if (m_d3d12DeviceInfo.setting.m_scaling)
        {
            // sceneSrvhandle is guaranteed to be valid when composition=false
            m_d3d12Device->CreateShaderResourceView
            (
            /* pResource      */ m_sceneBuffer.Get(),
            /* pDesc          */ nullptr,
            /* DestDescriptor */ sceneSrvhandle().value()
            );
        }
    }

    void Renderer::createWrappedBuffer()
    {
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
        ComPtr<IDXGISurface> surface = {};
        THROW_IF_FAILED(m_wrappedBuffer.As(&surface));

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
        THROW_IF_FAILED(m_d2d1DeviceContext->CreateBitmapFromDxgiSurface
        (
        /* surface          */ surface.Get(),
        /* bitmapProperties */ &props,
        /* bitmap           */ &m_renderTarget
        ));
    }

    void Renderer::createRenderTarget()
    {
        ComPtr<IDXGISurface> surface = {};
        // For DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        // only the swap chain's zero-index buffer can be read from and written to.
        // The swap chain's buffers with indexes greater than zero can only be read from.
        THROW_IF_FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)));

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
        THROW_IF_FAILED(m_d2d1DeviceContext->CreateBitmapFromDxgiSurface
        (
        /* surface          */ surface.Get(),
        /* bitmapProperties */ props,
        /* bitmap           */ &m_renderTarget
        ));
    }

    void Renderer::clearSwapChainRefs()
    {
        // back buffers of d3d12CmdQueue swap chain
        for (auto& buffer : m_backBuffers)
        {
            buffer.Reset();
        }
        // SetTarget also increases the reference count!
        m_d2d1DeviceContext->SetTarget(nullptr);

        // back buffer of dcomposition swap chain
        m_renderTarget.Reset();
    }

    void Renderer::resetCmdList(OptParam<ID3D12CommandAllocator*> alloc)
    {
        auto cmdAlloc = alloc.has_value() ? alloc.value() : m_cmdAlloc.Get();
        THROW_IF_FAILED(cmdAlloc->Reset());
        THROW_IF_FAILED(m_cmdList->Reset(cmdAlloc, nullptr));
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
            auto hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
            THROW_IF_NULL(hEvent);
            // m_fenceValue ==> wait all submitted commands
            THROW_IF_FAILED(m_fence->SetEventOnCompletion(m_fenceValue, hEvent));
            WaitForSingleObject(hEvent, INFINITE);
            CloseHandle(hEvent);
        }
    }

    void Renderer::beginGpuCommand()
    {
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
        flushCmdQueue();
    }

    void Renderer::waitCurrFrameResource()
    {
        if (currFrameResource()->m_fenceValue != 0 &&
            m_fence->GetCompletedValue() < currFrameResource()->m_fenceValue)
        {
            auto hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
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
        if (!m_composition)
        {
            m_letterbox->present();
        }
        auto& syncInterval = m_dxgiFactoryInfo.setting.m_syncInterval;

        UINT presentFlags = 0;
        if (m_dxgiFactoryInfo.setting.m_allowTearing)
        {
            presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
        }
        THROW_IF_FAILED(m_swapChain->Present(syncInterval, presentFlags));

        if (m_composition)
        {
            THROW_IF_FAILED(m_dcompVisual->SetContent(m_swapChain.Get()));
            THROW_IF_FAILED(m_dcompTarget->SetRoot(m_dcompVisual.Get()));
            THROW_IF_FAILED(m_dcompDevice->Commit());
        }
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

        // sceneRtvHandle is guaranteed to be valid when composition=false
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = sceneRtvHandle().value();

        m_cmdList->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);
        m_cmdList->ClearRenderTargetView(rtvHandle, m_sceneColor, 0, nullptr);

        graph_utils::revertBarrier(1, &barrier);
        m_cmdList->ResourceBarrier(1, &barrier);
    }

    void Renderer::clearLayerBuffer()
    {
        m_d2d1DeviceContext->SetTarget(m_renderTarget.Get());
        m_d2d1DeviceContext->BeginDraw();

        m_d2d1DeviceContext->Clear(m_layerColor);

        THROW_IF_FAILED(m_d2d1DeviceContext->EndDraw());
    }

    void Renderer::clearRenderTarget()
    {
        if (m_composition)
        {
            clearLayerBuffer();
        }
        else // self-maintained back buffers
        {
            clearSceneBuffer();
        }
    }

    const XMVECTORF32& Renderer::sceneColor() const
    {
        return m_sceneColor;
    }

    void Renderer::setSceneColor(const XMVECTORF32& color)
    {
        m_sceneColor = color;

        // Recreate the scene buffer with the new color to optimize clearing.

        if (!m_composition)
        {
            beginGpuCommand();

            createSceneBuffer();
            createWrappedBuffer();

            endGpuCommand();
        }
    }

    const D2D1_COLOR_F& Renderer::layerColor() const
    {
        return m_layerColor;
    }

    void Renderer::setLayerColor(const D2D1_COLOR_F& color)
    {
        m_layerColor = color;
    }

    TickTimer* Renderer::timer() const
    {
        return m_timer.get();
    }

    Optional<Letterbox*> Renderer::letterbox() const
    {
        if (m_composition)
        {
            return std::nullopt;
        }
        return m_letterbox.get();
    }

    Renderer::CommandLayer::CommandLayer(ID3D12Device* device)
    {
        for (auto& cmdAlloc : m_cmdAllocs)
        {
            THROW_IF_FAILED(device->CreateCommandAllocator
            (
            /* type               */ D3D12_COMMAND_LIST_TYPE_DIRECT,
            /* riid               */
            /* ppCommandAllocator */ IID_PPV_ARGS(&cmdAlloc)
            ));
        }
    }

    void Renderer::CommandLayer::resetCmdList(ID3D12GraphicsCommandList* cmdList, size_t index)
    {
        auto& cmdAlloc = m_cmdAllocs.at(index);

        cmdAlloc->Reset();
        cmdList->Reset(cmdAlloc.Get(), nullptr);
    }

    void Renderer::drawD3d12Target(CommandLayer::D3D12Target& target)
    {
        D3D12_RESOURCE_BARRIER barrier = {};
        if (!m_composition)
        {
            barrier = CD3DX12_RESOURCE_BARRIER::Transition
            (
                m_sceneBuffer.Get(),
                D3D12_RESOURCE_STATE_COMMON,
                D3D12_RESOURCE_STATE_RENDER_TARGET
            );
            m_cmdList->ResourceBarrier(1, &barrier);
        }
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
        if (!m_composition)
        {
            graph_utils::revertBarrier(1, &barrier);
            m_cmdList->ResourceBarrier(1, &barrier);
        }
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
        if (!m_composition)
        {
            m_d3d11On12Device->AcquireWrappedResources(m_wrappedBuffer.GetAddressOf(), 1);
        }
        // It is recommended to call SetTarget before BeginDraw.
        // The program may crash if the previous target is a synchronized
        // resource and still bound to the context when calling BeginDraw.
        m_d2d1DeviceContext->SetTarget(m_renderTarget.Get());

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

        if (!m_composition)
        {
            m_d3d11On12Device->ReleaseWrappedResources(m_wrappedBuffer.GetAddressOf(), 1);
        }
        // Submits the D3D11 command list to the shared command queue.
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
        ComPtr<IDWriteRenderingParams> params = {};
        THROW_IF_FAILED(m_dwriteFactory->CreateRenderingParams(&params));
        return
        {
            .gamma            = params->GetGamma(),
            .enhancedContrast = params->GetEnhancedContrast(),
            .clearTypeLevel   = params->GetClearTypeLevel(),
            .pixelGeometry    = params->GetPixelGeometry(),
            .renderingMode    = params->GetRenderingMode()
        };
    }

    void Renderer::setTextRenderingMode(const TextRenderingSettings& mode)
    {
        ComPtr<IDWriteRenderingParams> params = {};
        THROW_IF_FAILED(m_dwriteFactory->CreateCustomRenderingParams
        (
        /* gamma            */ mode.gamma,
        /* enhancedContrast */ mode.enhancedContrast,
        /* clearTypeLevel   */ mode.clearTypeLevel,
        /* pixelGeometry    */ mode.pixelGeometry,
        /* renderingMode    */ mode.renderingMode,
        /* renderingParams  */ &params
        ));
        m_d2d1DeviceContext->SetTextRenderingParams(params.Get());
    }
}
