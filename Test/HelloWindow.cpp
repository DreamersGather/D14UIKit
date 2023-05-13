#include "Application.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"HelloWindow"

int main()
{
    Application app(DEMO_NAME);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);

    //------------------------------------------- Set UI event callacks.

    return app.run();
}
