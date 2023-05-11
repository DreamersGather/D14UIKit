#include "Application.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"DemoTemplate"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI"))
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd;

    //------------------------------------------- Set UI event callacks.

    return app.run();
}
