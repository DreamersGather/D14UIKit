#include "Application.h"
#include "MainWindow.h"

using namespace d14uikit;

int wmain(int argc, wchar_t* argv[])
{
    Application app(argc, argv);

    MainWindow wnd(L"D14UIKit");

    return app.run();
}
