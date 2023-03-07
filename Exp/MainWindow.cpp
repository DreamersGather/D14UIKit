#include "Common/Precompile.h"

#include "Inc/MainWindow.h"

#include "UIKit/MainWindow.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    struct MainWindow::Impl : uikit::MainWindow
    {
        Impl::Impl(
            const std::wstring& title,
            float captionPanelHeight,
            float decorativeBarHeight)
            :
            Panel({}, uikit::resource_utils::g_solidColorBrush),
            MainWindow(
                title,
                captionPanelHeight,
                decorativeBarHeight) { }
    };

    MainWindow::MainWindow(
        const std::wstring& title,
        float captionPanelHeight,
        float decorativeBarHeight)
        :
        pimpl(uikit::makeRootUIObject<Impl>(
            title,
            captionPanelHeight,
            decorativeBarHeight)) { }
}
