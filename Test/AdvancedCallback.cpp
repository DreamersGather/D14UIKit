#include "Application.h"
#include "Callback.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"AdvancedCallback"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI"))
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);
    Panel clntArea;
    mwnd.setContent(&clntArea);

    //------------------------------------------- Set UI event callacks.

    return app.run();
}
