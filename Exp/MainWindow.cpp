#include "Common/Precompile.h"

#include "Inc/MainWindow.h"

#include "UIKit/MainWindow.h"

using namespace d14engine;

namespace d14uikit
{
    struct MainWindow::Impl
    {
        SharedPtr<uikit::MainWindow> inst = {};

        Impl(
            const std::wstring& title = L"Untitled",
            float captionPanelHeight = 32.0f,
            float decorativeBarHeight = 4.0f)
            :
            inst(uikit::makeRootUIObject<uikit::MainWindow>(
                title,
                captionPanelHeight,
                decorativeBarHeight)) { }
    };

    MainWindow::MainWindow(
        const std::wstring& title,
        float captionPanelHeight,
        float decorativeBarHeight)
        :
        pimpl(std::make_unique<Impl>(
            title,
            captionPanelHeight,
            decorativeBarHeight)) { }

    MainWindow::~MainWindow() { }
}
