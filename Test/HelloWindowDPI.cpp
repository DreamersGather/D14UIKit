﻿#include "Application.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"HelloWindowDPI"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);

    //------------------------------------------- Set UI event callbacks.

    return app.run();
}
