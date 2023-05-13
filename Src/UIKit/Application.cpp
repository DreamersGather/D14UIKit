#include "Common/Precompile.h"

#include "UIKit/Application.h"

#include "Common/CppLangUtils/FinalAction.h"
#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/GDI.h"

#include "Renderer/GraphUtils/Resource.h"
#include "Renderer/TickTimer.h"

#include "UIKit/Appearances/Appearance.h"
#include "UIKit/BitmapUtils.h"
#include "UIKit/Cursor.h"
#include "UIKit/PlatformUtils.h"
#include "UIKit/ResourceUtils.h"
#include "UIKit/TextInputObject.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Application* Application::g_app = nullptr;

    Application::Application(
        int argc,
        wchar_t* argv[],
        const CreateInfo& info)
        :
        createInfo(info)
    {
        g_app = this;

        SetDllDirectory(info.libraryPath.c_str());

        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        initWin32Window();

        initMainRenderer();

        initMiscComponents();
    }

    void Application::initWin32Window()
    {
        HINSTANCE hInstance = GetModuleHandle(_D14_MODULE_NAME);
        
        WNDCLASSEX wndclass = {};
        wndclass.cbSize = sizeof(wndclass);
        wndclass.style = CS_DBLCLKS | CS_DROPSHADOW;
        wndclass.lpfnWndProc = fnWndProc;
        // We will populate GWLP_USERDATA with the application instance pointer.
        wndclass.cbWndExtra = sizeof(this);
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wndclass.lpszClassName = createInfo.name.c_str();

        RegisterClassEx(&wndclass);

        RECT workAreaRect = {};
        SystemParametersInfo(SPI_GETWORKAREA, 0, &workAreaRect, 0);

        auto workAreaWidth = math_utils::width(workAreaRect);
        auto workAreaHeight = math_utils::height(workAreaRect);

        RECT wndrect = {};
        if (createInfo.showMaximized)
        {
            wndrect = { 0, 0, workAreaWidth, workAreaHeight };
        }
        else if (createInfo.win32WindowRect.has_value())
        {
            auto srcrc = platform_utils::scaledByDpi(createInfo.win32WindowRect.value());

            if (createInfo.showCentered)
            {
                wndrect = math_utils::centered(
                    { 0, 0, workAreaWidth, workAreaHeight },
                    math_utils::size(srcrc));
            }
            else wndrect = createInfo.win32WindowRect.value();
        }
        else // Display in the default rectangle area (800 x 600).
        {
            wndrect = math_utils::centered(
                { 0, 0, workAreaWidth, workAreaHeight },
                platform_utils::scaledByDpi(SIZE{ 800, 600 }));
        }

        DWORD dwStyle = WS_POPUP;
        // Prevent DWM from drawing the window again.
        DWORD dwExStyle =  WS_EX_NOREDIRECTIONBITMAP;

        auto dpi = (UINT)platform_utils::dpi();
        AdjustWindowRectExForDpi(&wndrect, dwStyle, FALSE, dwExStyle, dpi);

        THROW_IF_NULL(m_win32Window = CreateWindowEx(
            dwExStyle,
            createInfo.name.c_str(),
            createInfo.name.c_str(),
            dwStyle,
            wndrect.left,
            wndrect.top,
            math_utils::width(wndrect),
            math_utils::height(wndrect),
            nullptr,
            nullptr,
            hInstance,
            nullptr));

        SetWindowLongPtr(m_win32Window, GWLP_USERDATA, (LONG_PTR)this);
    }

    void Application::initMainRenderer()
    {
        auto dpi = platform_utils::dpi();

        Renderer::CreateInfo info = { .dpi = dpi };
        m_renderer = std::make_unique<Renderer>(m_win32Window, info);

        auto device = m_renderer->d3d12Device();
        m_uiCmdLayer = std::make_shared<Renderer::CommandLayer>(device);

        m_uiCmdLayer->setPriority(g_uiCmdLayerPriority);
        m_renderer->cmdLayers.insert(m_uiCmdLayer);

        m_uiCmdLayer->drawTarget.emplace<Renderer::CommandLayer::D2D1Target>();

        m_renderer->skipUpdating = true;
        m_renderer->timer()->stop();
        // We will restart the timer when playing animation.

        m_renderer->d2d1DeviceContext()->SetDpi(dpi, dpi);
        m_renderer->d2d1DeviceContext()->SetUnitMode(D2D1_UNIT_MODE_DIPS);
    }

    void Application::initMiscComponents()
    {
        m_systemThemeStyle.querySystemSettingsFromRegistry();

        appearance::initialize();

        bool light = (m_systemThemeStyle.mode == ThemeStyle::Mode::Light);
        m_currThemeName = light ? L"Light" : L"Dark";

        bitmap_utils::initialize();
        resource_utils::initialize();

        m_cursor = makeUIObject<Cursor>();

        m_cursor->setVisible(false);
        m_cursor->registerDrawObjects();
        // The cursor does not need to receive any UI event.
    }

    int Application::run(FuncParam<void(Application* app)> onLaunch)
    {
        if (onLaunch) onLaunch(this);

        m_renderer->renderNextFrame();

        if (createInfo.showFullscreen)
        {
            m_renderer->window().enterFullscreenMode();
        }
        else ShowWindow(m_win32Window, SW_SHOW);

        UpdateWindow(m_win32Window);

        MSG msg;
        while (true)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            // Use "else-if" instead of "if" here to empty the Win32 message
            // queue between each frame since there are usually dozens of UI
            // messages queued up during a single frame.
            else if (m_animationCount > 0)
            {
                m_renderer->renderNextFrame();
            }
        }
        return (int)msg.wParam;
    }

    void Application::exit()
    {
        SendMessage(m_win32Window, WM_DESTROY, 0, 0);
    }

    LRESULT CALLBACK Application::fnWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto app = (Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        
        if (app != nullptr && app->m_isHandlingSensitiveUIEvent)
        {
            if (message == WM_MOUSEMOVE ||
                message == WM_MOUSELEAVE ||
                message == WM_MOUSEWHEEL ||
                message == WM_KEYDOWN ||
                message == WM_SYSKEYDOWN ||
                message == WM_KEYUP ||
                message == WM_SYSKEYUP ||
                message == WM_LBUTTONDOWN ||
                message == WM_LBUTTONUP ||
                message == WM_LBUTTONDBLCLK ||
                message == WM_RBUTTONDOWN ||
                message == WM_RBUTTONUP ||
                message == WM_RBUTTONDBLCLK ||
                message == WM_MBUTTONDOWN ||
                message == WM_MBUTTONUP ||
                message == WM_MBUTTONDBLCLK)
            {
                PostMessage(hwnd, message, wParam, lParam);
                return 0;
            }
        }
        switch (message)
        {
        case WM_SIZE:
        {
            if (app != nullptr)
            {
                SIZE clntSize = { LOWORD(lParam), HIWORD(lParam) };
                if (clntSize.cx > 0 && clntSize.cy > 0)
                {
                    app->m_renderer->onWindowResize();

                    if (app->win32WindowSettings.callback.f_onClientAreaSize)
                    {
                        app->win32WindowSettings.callback.f_onClientAreaSize(clntSize);
                    }
                    app->m_renderer->renderNextFrame();
                }
            }
            return 0;
        }
        case WM_PAINT:
        {
            if (app != nullptr)
            {
                if (app->m_animationCount != 0)
                {
                    // Prevent the system from sending WM_PAINT repeatedly.
                    ValidateRect(hwnd, nullptr);
                }
                else app->m_renderer->renderNextFrame();
            }
            return 0;
        }
        case WM_ERASEBKGND:
        {
            return TRUE; // Prevent GDI from clearing background.
        }
        case WM_SETTINGCHANGE:
        {
            if (app != nullptr)
            {
                if (lstrcmp(LPCTSTR(lParam), L"ImmersiveColorSet") == 0)
                {
                    app->m_systemThemeStyle.querySystemSettingsFromRegistry();
                
                    if (app->f_onSystemThemeStyleChange) app->f_onSystemThemeStyleChange();
                }
            }
            return 0;
        }
        case WM_ACTIVATEAPP:
        {
            if (app != nullptr)
            {
                if (wParam && IsIconic(app->m_win32Window))
                {
                    if (app->win32WindowSettings.callback.f_onRestoreFromMinimized)
                    {
                        app->win32WindowSettings.callback.f_onRestoreFromMinimized();
                    }
                }
                if (!wParam && app->m_renderer->window().fullscreen())
                {
                    // Make sure the fullscreen-window can be switched by Alt+Tab.
                    ShowWindow(app->m_win32Window, SW_MINIMIZE);
                }
            }
            return 0;
        }
        case WM_SETCURSOR:
        {
            if (app != nullptr && LOWORD(lParam) == HTCLIENT)
            {
                if (app->m_cursor->useSystemIcons)
                {
                    app->m_cursor->setSystemIcon();
                }
                else SetCursor(nullptr); // Take over cursor drawing from GDI.
                return 0;
            }
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        case WM_NCHITTEST:
        {
            if (app != nullptr)
            {
                POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                ScreenToClient(hwnd, &pt);

                return app->handleWin32NCHITTESTMessage(platform_utils::restoredByDpi(pt));
            }
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        case WM_GETMINMAXINFO:
        {
            // We must config the min-max-info for the popup-window manually,
            // otherwise it occupies the whole screen when showing maximized.

            auto pMinMaxInfo = (MINMAXINFO*)lParam;

            RECT workAreaRect = {};
            SystemParametersInfo(SPI_GETWORKAREA, 0, &workAreaRect, 0);

            pMinMaxInfo->ptMaxSize.x = math_utils::width(workAreaRect);
            pMinMaxInfo->ptMaxSize.y = math_utils::height(workAreaRect);

            pMinMaxInfo->ptMaxPosition.x = workAreaRect.left;
            pMinMaxInfo->ptMaxPosition.y = workAreaRect.top;

            if (app != nullptr && app->win32WindowSettings.geometry.minTrackSize.has_value())
            {
                auto& minSize = app->win32WindowSettings.geometry.minTrackSize.value();
                pMinMaxInfo->ptMinTrackSize = platform_utils::scaledByDpi(minSize);
            }
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if (app == nullptr) return 0;
            app->m_isHandlingSensitiveUIEvent = true;

            auto rawCursorPoint =
            platform_utils::restoredByDpi(POINT
            {
                GET_X_LPARAM(lParam),
                GET_Y_LPARAM(lParam)
            });
            D2D1_POINT_2F cursorPoint =
            {
                (float)rawCursorPoint.x,
                (float)rawCursorPoint.y
            };
            if (!app->isTriggerDraggingWin32Window)
            {
                app->m_cursor->move(cursorPoint.x, cursorPoint.y);
            }
            app->m_cursor->setIcon(Cursor::Arrow);

            MouseMoveEvent e = {};
            e.cursorPoint = cursorPoint;

            e.buttonState.leftPressed = wParam & MK_LBUTTON;
            e.buttonState.middlePressed = wParam & MK_MBUTTON;
            e.buttonState.rightPressed = wParam & MK_RBUTTON;

            e.keyState.ALT = wParam & MK_ALT;
            e.keyState.CTRL = wParam & MK_CONTROL;
            e.keyState.SHIFT = wParam & MK_SHIFT;

            e.lastCursorPoint = app->m_lastCursorPoint;
            app->m_lastCursorPoint = e.cursorPoint;

            if (!app->m_currFocusedUIObject.expired() &&
                 app->m_currFocusedUIObject.lock()->forceGlobalExclusiveFocusing)
            {
                app->m_currFocusedUIObject.lock()->onMouseMove(e);
            }
            else // Deliver mouse-move event normally.
            {
                UIObjectTempSet currHitUIObjects = {};
                for (auto& uiobj : app->m_uiObjects)
                {
                    if (uiobj->appEventReactability.hitTest && uiobj->isHit(cursorPoint))
                    {
                        currHitUIObjects.insert(uiobj);
                    }
                }
                if (app->forceSingleMouseEnterLeaveEvent)
                {
                    WeakPtr<Panel> enterCandidate = {}, leaveCandidate = {};
                    if (!currHitUIObjects.empty())
                    {
                        enterCandidate = *currHitUIObjects.begin();
                    }
                    if (!app->m_hitUIObjects.empty())
                    {
                        leaveCandidate = *app->m_hitUIObjects.begin();
                    }
                    if (!cpp_lang_utils::isMostDerivedEqual(enterCandidate.lock(), leaveCandidate.lock()))
                    {
                        if (!enterCandidate.expired())
                        {
                            auto candidate = enterCandidate.lock();
                            if (candidate->appEventReactability.mouse.enter)
                            {
                                candidate->onMouseEnter(e);
                            }
                        }
                        if (!leaveCandidate.expired())
                        {
                            auto candidate = leaveCandidate.lock();
                            if (candidate->appEventReactability.mouse.leave)
                            {
                                candidate->onMouseLeave(e);
                            }
                        }
                    }
                }
                else // trigger multiple mouse-enter-leave events
                {
                    ISortable<Panel>::foreach(currHitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                    {
                        // Moved in just now, trigger mouse-enter event.
                        if (app->m_hitUIObjects.find(uiobj) == app->m_hitUIObjects.end())
                        {
                            if (uiobj->appEventReactability.mouse.enter)
                            {
                                uiobj->onMouseEnter(e);
                            }
                            return uiobj->appEventTransparency.mouse.enter;
                        }
                        return true;
                    });
                    ISortable<Panel>::foreach(app->m_hitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                    {
                        // Moved out just now, trigger mouse-leave event.
                        if (currHitUIObjects.find(uiobj) == currHitUIObjects.end())
                        {
                            if (uiobj->appEventReactability.mouse.leave)
                            {
                                uiobj->onMouseLeave(e);
                            }
                            return uiobj->appEventTransparency.mouse.leave;
                        }
                        return true;
                    });
                }
                app->m_hitUIObjects = std::move(currHitUIObjects);

                ISortable<Panel>::foreach(app->m_hitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.move)
                    {
                        uiobj->onMouseMove(e);
                    }
                    return uiobj->appEventTransparency.mouse.move;
                });
                app->updateDiffPinnedUIObjects();

                ISortable<Panel>::foreach(app->m_diffPinnedUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.move)
                    {
                        uiobj->onMouseMove(e);
                    }
                    return uiobj->appEventTransparency.mouse.move;
                });
            }
            // Register mouse-leave event for the Win32 window.
            TRACKMOUSEEVENT tme = {};
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hwnd;

            TrackMouseEvent(&tme);

            // The cursor will be hidden if moves out of the Win32 window,
            // so we need to show it explicitly in every mouse-move event.
            app->m_cursor->setVisible(true);
            if (app->m_cursor->useSystemIcons)
            {
                app->m_cursor->setSystemIcon();
            }
            InvalidateRect(hwnd, nullptr, FALSE);

            app->m_isHandlingSensitiveUIEvent = false;
            return 0;
        }
        case WM_MOUSELEAVE:
        {
            if (app == nullptr) return 0;
            app->m_isHandlingSensitiveUIEvent = true;

            POINT screenCursorPoint = {};
            GetCursorPos(&screenCursorPoint);
            ScreenToClient(hwnd, &screenCursorPoint);

            screenCursorPoint = platform_utils::restoredByDpi(screenCursorPoint);

            MouseMoveEvent e = {};
            e.cursorPoint =
            {
                (float)screenCursorPoint.x,
                (float)screenCursorPoint.y
            };
            e.lastCursorPoint = app->m_lastCursorPoint;
            app->m_lastCursorPoint = e.cursorPoint;

            if (!app->m_currFocusedUIObject.expired() &&
                 app->m_currFocusedUIObject.lock()->forceGlobalExclusiveFocusing)
            {
                app->m_currFocusedUIObject.lock()->onMouseLeave(e);
            }
            else // Deliver mouse-leave event normally.
            {
                ISortable<Panel>::foreach(app->m_hitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.leave)
                    {
                        uiobj->onMouseLeave(e);
                    }
                    return uiobj->appEventTransparency.mouse.leave;
                });
                app->m_hitUIObjects.clear();
            }
            app->m_cursor->setVisible(false);

            InvalidateRect(hwnd, nullptr, FALSE);

            app->m_isHandlingSensitiveUIEvent = false;
            return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        {
            if (app == nullptr) return 0;
            app->m_isHandlingSensitiveUIEvent = true;

            if (message == WM_LBUTTONDOWN ||
                message == WM_RBUTTONDOWN ||
                message == WM_MBUTTONDOWN)
            {
                SetCapture(hwnd);
            }
            else ReleaseCapture();

            auto rawCursorPoint =
            platform_utils::restoredByDpi(POINT
            {
                GET_X_LPARAM(lParam),
                GET_Y_LPARAM(lParam)
            });
            MouseButtonEvent e = {};
            e.cursorPoint = { (float)rawCursorPoint.x, (float)rawCursorPoint.y };
            e.state.flag = MouseButtonEvent::State::FLAG_MAP.at(message);

            if (!app->m_currFocusedUIObject.expired() &&
                 app->m_currFocusedUIObject.lock()->forceGlobalExclusiveFocusing)
            {
                app->m_currFocusedUIObject.lock()->onMouseButton(e);
            }
            else // Deliver mouse-button event normally.
            {
                ISortable<Panel>::foreach(app->m_hitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.button)
                    {
                        if (uiobj->appEventReactability.focus.get &&
                           (e.state.leftDown() || e.state.leftDblclk()))
                        {
                            e.focused = uiobj;
                        }
                        uiobj->onMouseButton(e);
                    }
                    return uiobj->appEventTransparency.mouse.button;
                });
                ISortable<Panel>::foreach(app->m_diffPinnedUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.button)
                    {
                        uiobj->onMouseButton(e);
                    }
                    return uiobj->appEventTransparency.mouse.button;
                });
                if (app->skipHandleNextFocusChangeEvent)
                {
                    app->skipHandleNextFocusChangeEvent = false;
                }
                else if (e.state.leftDown() || e.state.leftDblclk())
                {
                    app->focusUIObject(e.focused.lock());
                }
                app->handleImmediateMouseMoveEventCallback();
            }
            InvalidateRect(hwnd, nullptr, FALSE);

            app->m_isHandlingSensitiveUIEvent = false;
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            if (app == nullptr) return 0;
            app->m_isHandlingSensitiveUIEvent = true;

            POINT screenCursorPoint =
            {
                GET_X_LPARAM(lParam),
                GET_Y_LPARAM(lParam)
            };
            ScreenToClient(hwnd, &screenCursorPoint);

            screenCursorPoint = platform_utils::restoredByDpi(screenCursorPoint);

            MouseWheelEvent e = {};
            e.cursorPoint =
            {
                (float)screenCursorPoint.x,
                (float)screenCursorPoint.y
            };
            auto lowParam = LOWORD(wParam);

            e.buttonState.leftPressed = lowParam & MK_LBUTTON;
            e.buttonState.middlePressed = lowParam & MK_MBUTTON;
            e.buttonState.rightPressed = lowParam & MK_RBUTTON;

            e.keyState.CTRL = lowParam & MK_CONTROL;
            e.keyState.SHIFT = lowParam & MK_SHIFT;

            // The wheel distance can be negative.
            e.deltaCount = GET_Y_LPARAM(wParam) / WHEEL_DELTA;

            if (!app->m_currFocusedUIObject.expired() &&
                 app->m_currFocusedUIObject.lock()->forceGlobalExclusiveFocusing)
            {
                app->m_currFocusedUIObject.lock()->onMouseWheel(e);
            }
            else // Deliver mouse-wheel event normally.
            {
                ISortable<Panel>::foreach(app->m_hitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.wheel)
                    {
                        uiobj->onMouseWheel(e);
                    }
                    return uiobj->appEventTransparency.mouse.wheel;
                });
                ISortable<Panel>::foreach(app->m_diffPinnedUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.mouse.wheel)
                    {
                        uiobj->onMouseWheel(e);
                    }
                    return uiobj->appEventTransparency.mouse.wheel;
                });
                app->handleImmediateMouseMoveEventCallback();
            }
            InvalidateRect(hwnd, nullptr, FALSE);

            app->m_isHandlingSensitiveUIEvent = false;
            return 0;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            if (app == nullptr) return 0;
            app->m_isHandlingSensitiveUIEvent = true;

            KeyboardEvent e = {};
            e.vkey = wParam;

            if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
            {
                e.state.flag = KeyboardEvent::State::Flag::Pressed;
            }
            else e.state.flag = KeyboardEvent::State::Flag::Released;

            if (!app->m_currFocusedUIObject.expired())
            {
                app->m_currFocusedUIObject.lock()->onKeyboard(e);
            }
            else // Deliver keyboard event normally.
            {
                ISortable<Panel>::foreach(app->m_hitUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.keyboard)
                    {
                        uiobj->onKeyboard(e);
                    }
                    return uiobj->appEventTransparency.keyboard;
                });
                ISortable<Panel>::foreach(app->m_diffPinnedUIObjects, [&](ShrdPtrParam<Panel> uiobj)
                {
                    if (uiobj->appEventReactability.keyboard)
                    {
                        uiobj->onKeyboard(e);
                    }
                    return uiobj->appEventTransparency.keyboard;
                });
                app->handleImmediateMouseMoveEventCallback();
            }
            InvalidateRect(hwnd, nullptr, FALSE);

            app->m_isHandlingSensitiveUIEvent = false;
            return 0;
        }
        case WM_CHAR:
        {
            if (app != nullptr)
            {
                app->broadcastInputStringEvent({ (WCHAR*)&wParam, 1 });
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            return 0;
        }
        case WM_IME_STARTCOMPOSITION:
        {
            if (app && !app->m_focusedTextInputObject.expired())
            {
                auto form = app->m_focusedTextInputObject.lock()->getCompositionForm();
                if (form.has_value())
                {
                    HIMC himc = ImmGetContext(hwnd);
                    if (himc)
                    {
                        auto& fval = form.value();
                        if (fval.dwStyle == CFS_RECT)
                        {
                            fval.rcArea = platform_utils::scaledByDpi(fval.rcArea);
                        }
                        else if (fval.dwStyle == CFS_POINT)
                        {
                            fval.ptCurrentPos = platform_utils::scaledByDpi(fval.ptCurrentPos);
                        }
                        ImmSetCompositionWindow(himc, &fval);
                    }
                    ImmReleaseContext(hwnd, himc);
                }
            }
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        case WM_IME_COMPOSITION:
        {
            if (lParam & GCS_RESULTSTR)
            {
                HIMC himc = ImmGetContext(hwnd);
                if (himc)
                {
                    auto nSize = ImmGetCompositionString(himc, GCS_RESULTSTR, nullptr, 0);
                    if (nSize > 0)
                    {
                        // The null-terminated needs an extra character space.
                        auto hLocal = LocalAlloc(LPTR, nSize + sizeof(WCHAR));
                        if (hLocal)
                        {
                            ImmGetCompositionString(himc, GCS_RESULTSTR, hLocal, nSize);

                            if (app != nullptr)
                            {
                                app->broadcastInputStringEvent((WCHAR*)hLocal);
                                InvalidateRect(hwnd, nullptr, FALSE);
                            }
                            LocalFree(hLocal);
                        }
                    }
                }
                ImmReleaseContext(hwnd, himc);

                // Prevent from receiving WM_CHAR with the result string since it has been handled above.
                lParam &= ~(GCS_RESULTCLAUSE | GCS_RESULTREADCLAUSE | GCS_RESULTREADSTR | GCS_RESULTSTR);
            }
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        case (UINT)CustomWin32Message::UpdateRootDiffPinnedUIObjects:
        {
            if (app != nullptr)
            {
                app->updateDiffPinnedUIObjects();
            }
            return 0;
        }
        case (UINT)CustomWin32Message::UpdateMiscDiffPinnedUIObjects:
        {
            if (app != nullptr)
            {
                for (auto& uiobj : app->m_diffPinnedUpdateCandidates)
                {
                    if (!uiobj.expired())
                    {
                        uiobj.lock()->updateDiffPinnedUIObjects();
                    }
                }
                app->m_diffPinnedUpdateCandidates.clear();
            }
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        default: return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    LRESULT Application::handleWin32NCHITTESTMessage(const POINT& pt)
    {
        if (IsMaximized(m_win32Window))
        {
            return HTCLIENT;
        }
        if (win32WindowSettings.sizingFrame.f_NCHITTEST)
        {
            return win32WindowSettings.sizingFrame.f_NCHITTEST(pt);
        }
        return defWin32NCHITTESTMessageHandler(pt);
    }

    HWND Application::win32Window() const
    {
        return m_win32Window;
    }

    LRESULT Application::defWin32NCHITTESTMessageHandler(const POINT& pt)
    {
        if (win32WindowSettings.sizingFrame.frameWidth.has_value())
        {
            auto frmWidth = win32WindowSettings.sizingFrame.frameWidth.value();

            RECT clntRect = {};
            GetClientRect(m_win32Window, &clntRect);
            auto rawClntSize = math_utils::size(clntRect);
            auto clntSize = platform_utils::restoredByDpi(rawClntSize);

            if (pt.x <= frmWidth && pt.y <= frmWidth)
            {
                return HTTOPLEFT;
            }
            if (pt.x <= frmWidth && pt.y >= clntSize.cy - frmWidth)
            {
                return HTBOTTOMLEFT;
            }
            if (pt.x >= clntSize.cx - frmWidth && pt.y <= frmWidth)
            {
                return HTTOPRIGHT;
            }
            if (pt.x >= clntSize.cx - frmWidth && pt.y >= clntSize.cy - frmWidth)
            {
                return HTBOTTOMRIGHT;
            }
            if (pt.x <= frmWidth)
            {
                return HTLEFT;
            }
            if (pt.x >= clntSize.cx - frmWidth)
            {
                return HTRIGHT;
            }
            if (pt.y <= frmWidth)
            {
                return HTTOP;
            }
            if (pt.y >= clntSize.cy - frmWidth)
            {
                return HTBOTTOM;
            }
        }
        return HTCLIENT;
    }

    Renderer* Application::dxRenderer() const
    {
        return m_renderer.get();
    }

    ComPtr<ID2D1Bitmap1> Application::screenshot() const
    {
        m_renderer->beginGpuCommand();

        auto texture = m_renderer->sceneBuffer();
        auto staging = graph_utils::capture(texture, m_renderer->cmdList());

        m_renderer->endGpuCommand();

        auto unmap = cpp_lang_utils::finally([&]() { staging->Unmap(0, nullptr); });

        BYTE* mapped = nullptr;
        THROW_IF_FAILED(staging->Map(0, nullptr, (void**)&mapped));

        auto pixSize = m_renderer->d2d1RenderTarget()->GetPixelSize();
        return bitmap_utils::loadBitmap(pixSize.width, pixSize.height, mapped);
    }

    int Application::animationCount() const
    {
        return m_animationCount;
    }

    void Application::increaseAnimationCount()
    {
        ++m_animationCount;

        m_renderer->skipUpdating = false;
        m_renderer->timer()->resume();
    }

    void Application::decreaseAnimationCount()
    {
        m_animationCount = std::max(--m_animationCount, 0);

        if (m_animationCount == 0)
        {
            m_renderer->skipUpdating = true;
            m_renderer->timer()->start();
            m_renderer->timer()->stop();
        }
    }

    const Application::TopmostPriority& Application::topmostPriority() const
    {
        return m_topmostPriority;
    }

    void Application::moveRootObjectTopmost(Panel* uiobj)
    {
        uiobj->setD2d1ObjectPriority(++m_topmostPriority.d2d1Object);
        uiobj->setUIObjectPriority(--m_topmostPriority.uiObject);
    }

    const SharedPtr<Renderer::CommandLayer>& Application::uiCmdLayer() const
    {
        return m_uiCmdLayer;
    }

    const Application::UIObjectSet& Application::uiObjects() const
    {
        return m_uiObjects;
    }

    void Application::addUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (!uiobj) return;
        m_uiObjects.insert(uiobj);

        m_topmostPriority.uiObject = std::min
        (
            m_topmostPriority.uiObject,
            uiobj->ISortable<Panel>::m_priority
        );
    }

    void Application::removeUIObject(ShrdPtrParam<Panel> uiobj)
    {
        m_uiObjects.erase(uiobj);
    }

    void Application::pinUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (uiobj == nullptr) return;
        m_pinnedUIObjects.insert(uiobj);

        updateDiffPinnedUIObjectsLater();
    }

    void Application::unpinUIObject(ShrdPtrParam<Panel> uiobj)
    {
        m_pinnedUIObjects.erase(uiobj);

        updateDiffPinnedUIObjectsLater();
    }

    void Application::clearAddedUIObjects()
    {
        m_uiObjects.clear();
    }

    void Application::clearPinnedUIObjects()
    {
        m_pinnedUIObjects.clear();

        updateDiffPinnedUIObjectsLater();
    }

    void Application::updateDiffPinnedUIObjects()
    {
        m_diffPinnedUIObjects.clear();

        std::set_difference(
            m_pinnedUIObjects.begin(), m_pinnedUIObjects.end(),
            m_hitUIObjects.begin(), m_hitUIObjects.end(),
            std::inserter(m_diffPinnedUIObjects, m_diffPinnedUIObjects.begin()),
            ISortable<Panel>::WeakAscending()); // Can not deduce automatically.
    }

    void Application::updateDiffPinnedUIObjectsLater()
    {
        postCustomWin32Message(CustomWin32Message::UpdateRootDiffPinnedUIObjects);
    }

    WeakPtr<Panel> Application::currFocusedUIObject() const
    {
        return m_currFocusedUIObject;
    }

    void Application::focusUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (!cpp_lang_utils::isMostDerivedEqual(m_currFocusedUIObject.lock(), uiobj))
        {
            if (!m_currFocusedUIObject.expired())
            {
                auto fcobj = m_currFocusedUIObject.lock();
                if (!fcobj->appEventReactability.focus.lose)
                {
                    return;
                }
                m_currFocusedUIObject.reset();
                m_focusedTextInputObject.reset();

                fcobj->onLoseFocus();
            }
            if (uiobj && uiobj->appEventReactability.focus.get)
            {
                m_currFocusedUIObject = uiobj;
                m_focusedTextInputObject = std::dynamic_pointer_cast<TextInputObject>(uiobj);

                uiobj->onGetFocus();
            }
        }
    }

    Cursor* Application::cursor() const
    {
        return m_cursor.get();
    }

    const D2D1_POINT_2F& Application::lastCursorPoint() const
    {
        return m_lastCursorPoint;
    }

    const Wstring& Application::currThemeName() const
    {
        return m_currThemeName;
    }

    void Application::changeTheme(WstrParam themeName)
    {
        appearance::g_colorGroup.generateTonedColors();

        m_currThemeName = themeName;

        for (auto& uiobj : m_uiObjects)
        {
            uiobj->onChangeTheme(themeName);
        }
    }

    void Application::ThemeStyle::querySystemSettingsFromRegistry()
    {
        querySystemModeSetting();
        querySystemColorSetting();
    }

    void Application::ThemeStyle::querySystemModeSetting()
    {
        DWORD value = FALSE, valueSize = sizeof(value);

        if (SUCCEEDED(RegGetValue(HKEY_CURRENT_USER,
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
            L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &value, &valueSize)))
        {
            mode = value ? Mode::Light : Mode::Dark;
        }
    }

    void Application::ThemeStyle::querySystemColorSetting()
    {
        DWORD value = FALSE, valueSize = sizeof(value);

        if (SUCCEEDED(RegGetValue(HKEY_CURRENT_USER,
            L"Software\\Microsoft\\Windows\\DWM", L"AccentColor",
            RRF_RT_REG_DWORD, nullptr, &value, &valueSize)))
        {
            BYTE r = value % 256;
            BYTE g = (value >> 8) % 256;
            BYTE b = (value >> 16) % 256;
            BYTE a = (value >> 24) % 256;

            color = D2D1::ColorF((r << 16) + (g << 8) + b, a / 255.0f);
        }
    }

    const Application::ThemeStyle& Application::systemThemeStyle() const
    {
        return m_systemThemeStyle;
    }

    const Wstring& Application::currLangLocaleName() const
    {
        return m_currLangLocaleName;
    }

    void Application::changeLangLocale(WstrParam langLocaleName)
    {
        m_currLangLocaleName = langLocaleName;

        for (auto& uiobj : m_uiObjects)
        {
            uiobj->onChangeLangLocale(langLocaleName);
        }
    }

    void Application::broadcastInputStringEvent(WstrParam content)
    {
        if (!m_focusedTextInputObject.expired())
        {
            m_focusedTextInputObject.lock()->OnInputString(content);
        }
    }

    void Application::handleImmediateMouseMoveEventCallback()
    {
        if (sendNextImmediateMouseMoveEvent)
        {
            sendNextImmediateMouseMoveEvent = false;

            POINT screenCursorPoint = {};
            GetCursorPos(&screenCursorPoint);
            ScreenToClient(m_win32Window, &screenCursorPoint);

            SendMessage(m_win32Window, WM_MOUSEMOVE, 0,
                MAKELPARAM(screenCursorPoint.x, screenCursorPoint.y));
        }
    }

    void Application::postCustomWin32Message(CustomWin32Message message)
    {
        PostMessage(m_win32Window, (UINT)message, 0, 0);
    }

    void Application::pushDiffPinnedUpdateCandidate(ShrdPtrParam<Panel> uiobj)
    {
        m_diffPinnedUpdateCandidates.insert(uiobj);
    }
}
