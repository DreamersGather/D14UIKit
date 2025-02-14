#include "Application.h"
#include "Callback.h"
#include "FrameAnimPanel.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"FrameAnimation"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setSize({ 300, 336 });

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);
    mwnd.setMinimizeButton(false);
    mwnd.setMaximizeButton(false);

    Panel clntArea;
    mwnd.setContent(&clntArea);

    FrameAnimPanel animArea;
    animArea.setParent(&clntArea);
    animArea.setPosition({ 22, 22 });
    animArea.setSize({ 256, 256 });

    std::vector<Image*> framePtrs;
    std::vector<std::unique_ptr<Image>> frames;
    for (int i = 0; i < 12; ++i)
    {
        auto frame = std::make_unique<Image>(L"images/stick_boy/" + std::to_wstring(i) + L".png");

        framePtrs.push_back(frame.get());
        frames.push_back(std::move(frame));
    }
    animArea.setFrames(framePtrs);
    // Applies point-sampler to make the pixel art clear.
    animArea.setFrameInterpMode(BitmapInterpMode::NearestNeighbor);
    animArea.setFrameTimeSpan(0.06f);

    // Notifies the renderer to draw frames continuously.
    animArea.setAnimState(true);

    Label fpsLbl(L"FPS: None");
    fpsLbl.setParent(&clntArea);
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
