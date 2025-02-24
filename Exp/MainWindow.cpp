#include "Common/Precompile.h"

#include "MainWindow.h"

#include "Common.h"

#include "UIKit/MainWindow.h"

using namespace d14engine;

namespace d14uikit
{
    MainWindow::MainWindow(const std::wstring& title)
        :
        MainWindow(uikit::makeRootUIObject<uikit::MainWindow>(title)) { }

    _D14_UIKIT_CTOR(MainWindow)
        :
        Panel(uiobj),
        Window(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    bool MainWindow::immersiveDarkMode() const
    {
        return pimpl->uiobj->immersiveDarkMode();
    }

    bool MainWindow::setImmersiveDarkMode(bool value)
    {
        return pimpl->uiobj->setImmersiveDarkMode(value);
    }

    MainWindow::CornerState MainWindow::cornerState() const
    {
        return (CornerState)pimpl->uiobj->cornerState();
    }

    bool MainWindow::setCornerState(CornerState state)
    {
        return pimpl->uiobj->setCornerState((uikit::MainWindow::CornerState)state);
    }
}
