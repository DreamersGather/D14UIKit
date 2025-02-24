#include <atomic>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

#include "Application.h"
#include "Callback.h"
#include "FilledButton.h"
#include "Label.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"AsyncPolling"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setSize({ 600, 266 });

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);

    Panel clntArea;
    mwnd.setContent(&clntArea);

    FilledButton startBtn;
    startBtn.setParent(&clntArea);
    startBtn.setSize({ 200, 50 });
    startBtn.setPosition({ 50, 50 });
    startBtn.setRoundRadius(5);
    startBtn.setText(L"Start Progress");

    FilledButton controlBtn;
    controlBtn.setParent(&clntArea);
    controlBtn.setSize({ 200, 50 });
    controlBtn.setPosition({ 275, 50 });
    controlBtn.setRoundRadius(5);
    controlBtn.setText(L"Pause Progress");

    constexpr int full = 500;
    auto color = app.themeStyle().color;

    Label progressLbl;
    progressLbl.setParent(&clntArea);
    progressLbl.setSize({ 50, 50 });
    progressLbl.setPosition({ 500, 50 });
    progressLbl.setText(L"0/" + std::to_wstring(full));
    auto font = Font(L"Default/Normal/14");
    progressLbl.setFont(&font);
    progressLbl.setHorzAlign(Label::HCenter);

    Panel progressArea;
    progressArea.setParent(&clntArea);
    progressArea.setSize({ full, 30 });
    progressArea.setPosition({ 50, 150 });
    progressArea.setOutlineWidth(2);
    progressArea.setOutlineColor(color);
    progressArea.setOutlineOpacity(0.5f);
    progressArea.setRoundRadius(5);

    Label challengeLbl;
    challengeLbl.setParent(&progressArea);
    challengeLbl.setPosition({ 10, 0 });
    challengeLbl.setHeight(30);
    challengeLbl.setText(L"Challenge: Use the Pause/Resume button to get a 499/500!");
    challengeLbl.setFontSize(12);

    Panel progressBar;
    progressBar.setParent(&clntArea);
    progressBar.moveAbove(&progressArea);
    progressBar.setSize({ 0, 30 });
    progressBar.setPosition({ 50, 150 });
    progressBar.setColor(color);
    progressBar.setOpacity(0.5f);
    progressBar.setRoundRadius(5);

    //------------------------------------------- Set UI event callbacks.

    std::atomic<int> curr = 0;
    std::atomic<bool> work = true;
    std::atomic<bool> done = false;

    startBtn.D14_onMouseButtonRelease(clkp, e, &)
    {
        if (e->left())
        {
            startBtn.setEnabled(false);
            // render continuously for polling
            progressBar.setAnimState(true);

            // new round: reset thread state
            curr = 0;
            work.notify_one();
            done = false;
            progressBar.setWidth(0);

            std::thread([&]
            {
                for (int i = 1; i <= full; ++i)
                {
                    work.wait(false);
                    std::this_thread::sleep_for(2ms);
                    curr = i;
                }
                done = true;
            })
            .detach();
        }
    };
    controlBtn.D14_onMouseButtonRelease(clkp, e, &)
    {
        if (e->left())
        {
            if (work)
            {
                work = false;
                controlBtn.setText(L"Resume Progress");
            }
            else
            {
                work = true;
                controlBtn.setText(L"Pause Progress");
            }
            work.notify_one();
        }
    };

    ///////////////////////////////
    // Polling in each frame !!! //
    ///////////////////////////////

    progressLbl.D14_onUpdate(p, &)
    {
        progressLbl.setText(std::to_wstring(curr) + L"/" + std::to_wstring(full));
    };
    progressBar.D14_onUpdate(p, &)
    {
        if (done)
        {
            startBtn.setEnabled(true);
            progressBar.setAnimState(false);
        }
        else progressBar.setWidth(curr);
    };

    return app.run();
}
