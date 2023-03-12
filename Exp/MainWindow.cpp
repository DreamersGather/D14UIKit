#include "Common/Precompile.h"

#include "DraggablePanel.h"
#include "MainWindow.h"
#include "Panel.h"
#include "ResizablePanel.h"
#include "Window.h"

#include "UIKit/MainWindow.h"

using namespace d14engine;

namespace d14uikit
{
    MainWindow::MainWindow(const std::wstring& title) : MainWindow(Passkey{})
    {
        pimpl->uiobj = uikit::makeRootUIObject<uikit::MainWindow>(title);

        Panel::pimpl->uiobj = pimpl->uiobj;
        DraggablePanel::pimpl->uiobj = pimpl->uiobj;
        ResizablePanel::pimpl->uiobj = pimpl->uiobj;
        Window::pimpl->uiobj = pimpl->uiobj;

        Panel::initialize();
        DraggablePanel::initialize();
        ResizablePanel::initialize();
        Window::initialize();
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

    MainWindow::MainWindow(Passkey)
        :
        Panel(Panel::Passkey{}),
        Window(Window::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MainWindow::initialize() { }
}
