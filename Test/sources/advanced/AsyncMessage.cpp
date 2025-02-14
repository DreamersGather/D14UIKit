#include <chrono>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

#include "Application.h"
#include "Callback.h"
#include "FilledButton.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"AsyncMessage"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setSize({ 550, 186 });

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);

    Panel clntArea;
    mwnd.setContent(&clntArea);

    FilledButton uiLockBtn;
    uiLockBtn.setParent(&clntArea);
    uiLockBtn.setSize({ 200, 50 });
    uiLockBtn.setPosition({ 50, 50 });
    uiLockBtn.setRoundRadius(5);
    uiLockBtn.setText(L"Lock # UI: 0");

    FilledButton mxLockBtn;
    mxLockBtn.setParent(&clntArea);
    mxLockBtn.setSize({ 200, 50 });
    mxLockBtn.setPosition({ 300, 50 });
    mxLockBtn.setRoundRadius(5);
    mxLockBtn.setText(L"Lock # mutex: 0");

    //------------------------------------------- Set UI event callbacks.

    // To avoid race conditions in multithreading,
    // it is necessary to lock read/write operations.
    // Below are commonly used methods:
    // 
    // 1. Lock with UI
    // 
    // Locking by managing UI controls.
    // This approach can address risks caused by user operations,
    // but cannot avoid potential race conditions within the program.
    // 
    // 2. Lock with mutex
    // 
    // Locking using mutex objects.
    // This is the standard locking method for multithreading,
    // and requires careful design by developers.
    // 
    // Developers should choose based on the specific situation,
    // or use both methods simultaneously,
    // ensuring that race conditions are avoided.

    /////////////////////
    // 1. Lock with UI //
    /////////////////////

    int uiLockNum = 0;

    // To implement a callback function on the UI main thread
    // triggered by an event in a child thread, it is necessary to
    // bind a uint64_t ID to the thread event and the callback.
    //
    // This value determines the mapping
    // between the thread event and its corresponding callback,
    // so here we simply choose the address of the variable as the ID
    // (this value is obviously globally unique).
    // 
    // Developers can also choose custom IDs (like 0, 1 and 2 etc.),
    // as long as they ensure there are no conflicts.
    auto uiLockThreadID = (uint64_t)&uiLockNum;

    uiLockBtn.D14_onMouseButtonRelease(clkp, e, &)
    {
        if (e->left())
        {
            // lock
            uiLockBtn.setEnabled(false);

            // start working thread
            std::thread([&]
            {
                std::this_thread::sleep_for(3s);

                ++uiLockNum;

                // Call setThreadEvent in other threads to send messages,
                // and the corresponding callbacks will be called afterward.
                // Note that even though we cannot know the exact time when
                // the callbacks are called, the order can be guaranteed.
                app.setThreadEvent(uiLockThreadID);
            })
            .detach();
        }
    };
    app.addThreadCallback(uiLockThreadID, [&](uint64_t data)
    {
        // unlock
        uiLockBtn.setEnabled(true);

        auto uiLockStr = std::to_wstring(uiLockNum);
        uiLockBtn.setText(L"Lock # UI: " + uiLockStr);
    });

    ////////////////////////
    // 2. Lock with mutex //
    ////////////////////////

    int mxLockNum = 0;
    auto mxLockThreadID = (uint64_t)&mxLockNum;

    std::mutex mx = {};

    mxLockBtn.D14_onMouseButtonRelease(clkp, e, &)
    {
        if (e->left())
        {
            // start working thread
            std::thread([&]
            {
                std::this_thread::sleep_for(3s);

                // mutex block
                mx.lock();
                {
                    ++mxLockNum;
                }
                mx.unlock();

                app.setThreadEvent(mxLockThreadID);
            })
            .detach();
        }
    };
    app.addThreadCallback(mxLockThreadID, [&](uint64_t data)
    {
        auto mxLockStr = std::to_wstring(mxLockNum);
        mxLockBtn.setText(L"Lock # mutex: " + mxLockStr);
    });

    return app.run();
}
