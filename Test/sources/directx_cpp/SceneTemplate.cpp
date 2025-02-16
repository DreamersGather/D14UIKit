﻿#define AGILITY_SDK false
#define DX_COMPILER false

#if AGILITY_SDK
#define AGILITY_VER 615
#ifdef _WIN64
#define AGILITY_PATH "library/DirectX/x64/"
#else
#define AGILITY_PATH "library/DirectX/x86/"
#endif
#endif
// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#header-include-order
// It is recommended to include Agility SDK headers before the Windows 10 SDK to avoid conflicts.
#include "DirectX-Header.h"

#include <wrl.h>

using Microsoft::WRL::ComPtr;

#if DX_COMPILER
ComPtr<IDxcUtils> dxcUtils;
ComPtr<IDxcCompiler3> dxcCompiler;
ComPtr<IDxcIncludeHandler> dxcDefaultIncludeHandler;
#endif

#include "Application.h"
#include "Callback.h"
#include "Label.h"
#include "MainWindow.h"
#include "RenderObject.h"
#include "ScenePanel.h"

using namespace d14uikit;

#define DEMO_NAME L"SceneTemplate"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setMinSize(app.size());
    app.setResizable(true);

#if DX_COMPILER
    //////////////////////////
    // Initialize DXC Tools //
    //////////////////////////
    {
        std::wstring exePath(MAX_PATH, 0);
        GetModuleFileName(nullptr, exePath.data(), MAX_PATH);
        // There is no need to check the result of find_last_of
        // because the exe-path is guaranteed to contain a "\".
        exePath.resize(exePath.find_last_of(L'\\') + 1);

        // Set this before calling AddDllDirectory to ensure that
        // subsequent LoadLibrary calls search user-defined paths.
        SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
#ifdef _WIN64
        std::wstring libPath = L"library/DirectXShaderCompiler/x64";
#else
        std::wstring libPath = L"library/DirectXShaderCompiler/x86";
#endif
        AddDllDirectory((exePath + libPath).c_str());

        THROW_IF_FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils)));
        THROW_IF_FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)));
        THROW_IF_FAILED(dxcUtils->CreateDefaultIncludeHandler(&dxcDefaultIncludeHandler));
    }
#endif
    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);

    ScenePanel scene;
    mwnd.setContent(&scene);

    /////////////////////////
    // Build-Scene  START  //
    /////////////////////////

    /////////////////////////
    // Build-Scene   END   //
    /////////////////////////

    scene.setAnimState(true);

    Label fpsLbl(L"FPS: None");
    fpsLbl.setParent(&scene);
    fpsLbl.setPosition({ 22, 22 });

    //------------------------------------------- Set UI event callbacks.

     // This will be called before the renderer drawing each frame.
    fpsLbl.D14_onUpdate(p, &)
    {
        static int fps = 0;
        if (app.fps() != fps)
        {
            fps = app.fps();
            ((Label*)p)->setText(L"FPS: " + std::to_wstring(fps));
        }
    };
    return app.run();
}
