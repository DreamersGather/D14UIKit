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

    void MainWindow::showNormal()
    {
        pimpl->uiobj->setDisplayState(uikit::Window::DisplayState::Normal);
    }

    void MainWindow::showMinimized()
    {
        pimpl->uiobj->setDisplayState(uikit::Window::DisplayState::Minimized);
    }

    void MainWindow::showMaximized()
    {
        pimpl->uiobj->setDisplayState(uikit::Window::DisplayState::Maximized);
    }
}
