#include "Common/Precompile.h"

#include "UIKit/MainWindow.h"

#include "Common/MathUtils/2D.h"
#include "Common/MathUtils/GDI.h"

#include "UIKit/Application.h"
#include "UIKit/IconLabel.h"
#include "UIKit/PlatformUtils.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    MainWindow::MainWindow(
        ShrdPtrParam<IconLabel> caption,
        float captionPanelHeight,
        float decorativeBarHeight)
        :
        Panel({}, resource_utils::g_solidColorBrush),
        Window(caption, {}, captionPanelHeight, decorativeBarHeight)
    {
        setCornerState(ROUND); // adapt win11 geometry

        operationTarget = OperationTarget::GlobalWin32Window;

        auto& callback = Application::g_app->win32WindowSettings.callback;
        callback.f_onClientAreaSize = [this]
        (const SIZE& pixSize)
        {
            auto dipSize = platform_utils::restoredByDpi(pixSize);
            resize((float)dipSize.cx, (float)dipSize.cy);
        };
        callback.f_onRestoreFromMinimized = [this]
        {
            m_displayState = m_originalDisplayState;
        };
        respondSetForegroundEvent = false; // keep at the bottom-most
    }

    MainWindow::MainWindow(
        WstrParam title,
        float captionPanelHeight,
        float decorativeBarHeight)
        :
        MainWindow(
            IconLabel::labelExpandedLayout(title),
            captionPanelHeight,
            decorativeBarHeight) { }

    void MainWindow::onInitializeFinish()
    {
        Window::onInitializeFinish();

        RECT wndrect = {};
        GetClientRect(Application::g_app->win32Window(), &wndrect);

        auto pixSize = math_utils::size(wndrect);
        auto dipSize = platform_utils::restoredByDpi(pixSize);

        resize((float)dipSize.cx, (float)dipSize.cy);
    }

    void MainWindow::setDisplayState(DisplayState state)
    {
        switch (state)
        {
        case DisplayState::Normal:
        {
            setCornerState(ROUND);

            isDraggable = true;
            m_originalDisplayState = DisplayState::Normal;

            RECT wndrect = {};
            ShowWindow(Application::g_app->win32Window(), SW_NORMAL);
            GetClientRect(Application::g_app->win32Window(), &wndrect);
            auto pixSize = math_utils::size(wndrect);
            auto dipSize = platform_utils::restoredByDpi(pixSize);
            resize((float)dipSize.cx, (float)dipSize.cy);
            break;
        }
        case DisplayState::Minimized:
        {
            ShowWindow(Application::g_app->win32Window(), SW_MINIMIZE);
            break;
        }
        case DisplayState::Maximized:
        {
            setCornerState(DONOTRUOND);

            isDraggable = false;
            m_originalDisplayState = DisplayState::Maximized;

            RECT wndrect = {};
            ShowWindow(Application::g_app->win32Window(), SW_MAXIMIZE);
            GetClientRect(Application::g_app->win32Window(), &wndrect);
            auto pixSize = math_utils::size(wndrect);
            auto dipSize = platform_utils::restoredByDpi(pixSize);
            resize((float)dipSize.cx, (float)dipSize.cy);
            break;
        }
        default: break;
        }
    }

    MainWindow::CornerState MainWindow::cornerState() const
    {
        return m_cornerState;
    }

    void MainWindow::setCornerState(CornerState state)
    {
        if (SUCCEEDED(DwmSetWindowAttribute(
            Application::g_app->win32Window(),
            DWMWA_WINDOW_CORNER_PREFERENCE,
            &state, sizeof(CornerState))))
        {
            m_cornerState = state;
        }
    }

    void MainWindow::onChangeThemeHelper(WstrParam themeName)
    {
        Window::onChangeThemeHelper(themeName);

        if (themeName == L"Light")
        {
            Application::g_app->dxRenderer()->setSceneColor(Colors::White);
        }
        else if (themeName == L"Dark")
        {
            Application::g_app->dxRenderer()->setSceneColor(Colors::Black);
        }
        getAppearance().changeTheme(themeName);
    }

    void MainWindow::onCloseHelper()
    {
        Window::onCloseHelper();

        Application::g_app->exit();
    }

    void MainWindow::onRestoreHelper()
    {
        Window::onRestoreHelper();

        setDisplayState(DisplayState::Normal);
    }

    void MainWindow::onMinimizeHelper()
    {
        Window::onMinimizeHelper();

        setDisplayState(DisplayState::Minimized);
    }

    void MainWindow::onMaximizeHelper()
    {
        Window::onMaximizeHelper();

        setDisplayState(DisplayState::Maximized);
    }
}
