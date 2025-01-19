#include <Windows.h>

#include "Application.h"
#include "Callback.h"
#include "Label.h"
#include "MainWindow.h"
#include "Renderer.h"
#include "RenderObject.h"
#include "ScenePanel.h"

#include "DirectX-Headers/d3dx12.h"
#include "DirectXShaderCompiler/dxcapi.h"

#pragma comment(lib, "dxcompiler.lib")

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

#ifdef _WIN64
    SetDllDirectory(L"library/x64");
#else
    SetDllDirectory(L"library/x86");
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
