#include "Common/Precompile.h"

#include "UIKit/Window.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Application.h"
#include "UIKit/IconLabel.h"
#include "UIKit/ResourceUtils.h"
#include "UIKit/TabCaption.h"
#include "UIKit/TabGroup.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Window::Window(
        ShrdPtrParam<IconLabel> caption,
        const D2D1_RECT_F& rect,
        float captionPanelHeight,
        float decorativeBarHeight)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        DraggablePanel(rect, resource_utils::g_solidColorBrush),
        ResizablePanel(rect, resource_utils::g_solidColorBrush),
        mask(math_utils::roundu(size())),
        shadow(math_utils::roundu(size())),
        m_caption(caption),
        m_captionPanelHeight(captionPanelHeight),
        m_decorativeBarHeight(decorativeBarHeight)
    {
        m_takeOverChildrenDrawing = true;

        transform(math_utils::adaptMaxSize(rect, minimalSize()));
    }

    Window::Window(
        WstrParam title,
        const D2D1_RECT_F& rect,
        float captionPanelHeight,
        float decorativeBarHeight)
        :
        Window(
            IconLabel::labelExpandedLayout(title),
            rect,
            captionPanelHeight,
            decorativeBarHeight) { }

    void Window::onInitializeFinish()
    {
        ResizablePanel::onInitializeFinish();

        if (!m_caption)
        {
            m_caption = IconLabel::labelExpandedLayout(L"Untitled");
        }
        addUIObject(m_caption);

        m_caption->transform(captionIconLabelSelfcoordRect());

        loadDecorativeBarBrush();
    }

    void Window::loadDecorativeBarBrush()
    {
        auto context = Application::g_app->dxRenderer()->d2d1DeviceContext();

        D2D1_GRADIENT_STOP stop[] =
        {
            { 0.0f, getAppearance().decorativeBar.gradientColor._0_0 },
            { 0.5f, getAppearance().decorativeBar.gradientColor._0_5 },
            { 1.0f, getAppearance().decorativeBar.gradientColor._1_0 }
        };
        ComPtr<ID2D1GradientStopCollection> coll;

        THROW_IF_FAILED(context->CreateGradientStopCollection(stop, _countof(stop), &coll));
        THROW_IF_FAILED(context->CreateLinearGradientBrush({}, coll.Get(), &decorativeBarBrush));
    }

    float Window::minimalWidth() const
    {
        return nonClientAreaMinimalWidth();
    }

    float Window::minimalHeight() const
    {
        return nonClientAreaHeight();
    }

    void Window::onMinimize()
    {
        onMinimizeHelper();

        if (f_onMinimize) f_onMinimize(this);
    }

    void Window::onMaximize()
    {
        onMaximizeHelper();

        if (f_onMaximize) f_onMaximize(this);
    }

    void Window::onRestore()
    {
        onRestoreHelper();

        if (f_onRestore) f_onRestore(this);
    }

    void Window::onClose()
    {
        onCloseHelper();

        if (f_onClose) f_onClose(this);
    }

    void Window::onMinimizeHelper()
    {
        m_displayState = DisplayState::Minimized;
    }

    void Window::onMaximizeHelper()
    {
        m_displayState = DisplayState::Maximized;
    }

    void Window::onRestoreHelper()
    {
        m_displayState = DisplayState::Normal;
    }

    void Window::onCloseHelper()
    {
        // This method intentionally left blank.
    }

    const SharedPtr<IconLabel>& Window::caption() const
    {
        return m_caption;
    }

    void Window::setCaption(ShrdPtrParam<IconLabel> caption)
    {
        if (caption && !cpp_lang_utils::isMostDerivedEqual(caption, m_caption))
        {
            removeUIObject(m_caption);

            m_caption = caption;
            addUIObject(m_caption);

            m_caption->transform(captionIconLabelSelfcoordRect());
        }
    }

    void Window::setCenterUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (!cpp_lang_utils::isMostDerivedEqual(uiobj, m_centerUIObject))
        {
            removeUIObject(m_centerUIObject);

            m_centerUIObject = uiobj;
            addUIObject(m_centerUIObject);

            if (m_centerUIObject) m_centerUIObject->transform(clientAreaSelfcoordRect());
        }
    }

    D2D1_RECT_F Window::captionPanelAbsoluteRect() const
    {
        return
        {
            m_absoluteRect.left,
            m_absoluteRect.top,
            m_absoluteRect.right,
            m_absoluteRect.top + m_captionPanelHeight
        };
    }

    D2D1_RECT_F Window::decorativeBarAbsoluteRect() const
    {
        return
        {
            m_absoluteRect.left,
            m_absoluteRect.top + m_captionPanelHeight,
            m_absoluteRect.right,
            m_absoluteRect.top + nonClientAreaHeight()
        };
    }

    constexpr float Window::threeBrosLeftmostOffset()
    {
        return threeBrosRightmostOffset() + closeButtonWidth() + minMaxButtonWidth() * 2.0f;
    }

    D2D1_RECT_F Window::minimizeIconAbsoluteRect() const
    {
        return math_utils::moveVertex(minimizeButtonAbsoluteRect(), minimizeIconVertexOffset());
    }

    D2D1_RECT_F Window::minimizeButtonAbsoluteRect() const
    {
        return
        {
            m_absoluteRect.right - threeBrosLeftmostOffset(),
            m_absoluteRect.top,
            m_absoluteRect.right - threeBrosLeftmostOffset() + minMaxButtonWidth(),
            m_absoluteRect.top + threeBrosHeight()
        };
    }

    D2D1_RECT_F Window::restoreIconAbsoluteRect() const
    {
        return math_utils::moveVertex(maximizeButtonAbsoluteRect(), restoreIconVertexOffset());
    }

    D2D1_RECT_F Window::maximizeIconAbsoluteRect() const
    {
        return math_utils::moveVertex(maximizeButtonAbsoluteRect(), maximizeIconVertexOffset());
    }

    D2D1_RECT_F Window::maximizeButtonAbsoluteRect() const
    {
        return
        {
            m_absoluteRect.right - threeBrosLeftmostOffset() + minMaxButtonWidth(),
            m_absoluteRect.top,
            m_absoluteRect.right - threeBrosRightmostOffset() - closeButtonWidth(),
            m_absoluteRect.top + threeBrosHeight()
        };
    }

    D2D1_RECT_F Window::closeIconAbsoluteRect() const
    {
        return math_utils::moveVertex(closeButtonAbsoluteRect(), closeIconVertexOffset());
    }

    D2D1_RECT_F Window::closeButtonAbsoluteRect() const
    {
        return
        {
            m_absoluteRect.right - threeBrosRightmostOffset() - closeButtonWidth(),
            m_absoluteRect.top,
            m_absoluteRect.right - threeBrosRightmostOffset(),
            m_absoluteRect.top + threeBrosHeight()
        };
    }

    D2D1_RECT_F Window::captionIconLabelSelfcoordRect() const
    {
        return
        {
            threeBrosLeftmostOffset(),
            0.0f,
            std::max(width() - threeBrosLeftmostOffset(), threeBrosLeftmostOffset()),
            m_captionPanelHeight
        };
    }

    float Window::captionPanelHeight() const
    {
        return m_captionPanelHeight;
    }

    void Window::setCaptionPanelHeight(float value)
    {
        m_captionPanelHeight = value;

        m_caption->transform(captionIconLabelSelfcoordRect());
        if (m_centerUIObject) m_centerUIObject->transform(clientAreaSelfcoordRect());
    }

    float Window::decorativeBarHeight() const
    {
        return m_decorativeBarHeight;
    }

    void Window::setDecorativeBarHeight(float value)
    {
        m_decorativeBarHeight = value;

        m_caption->transform(captionIconLabelSelfcoordRect());
        if (m_centerUIObject) m_centerUIObject->transform(clientAreaSelfcoordRect());
    }

    float Window::clientAreaHeight() const
    {
        return height() - nonClientAreaHeight();
    }

    D2D1_RECT_F Window::clientAreaSelfcoordRect() const
    {
        return { 0.0f, nonClientAreaHeight(), width(), height()};
    }

    float Window::nonClientAreaHeight() const
    {
        return m_captionPanelHeight + m_decorativeBarHeight;
    }

    D2D1_RECT_F Window::nonClientAreaSelfcoordRect() const
    {
        return { 0.0f, 0.0f, width(), nonClientAreaHeight() };
    }

    D2D1_RECT_F Window::nonClientAreaMinimalSelfcoordRect() const
    {
        return { 0.0f, 0.0f, nonClientAreaMinimalWidth(), nonClientAreaHeight() };
    }

    Window::DisplayState Window::currDisplayState() const
    {
        return m_displayState;
    }

    bool Window::isDisplayNormal() const
    {
        return m_displayState == DisplayState::Normal;
    }

    bool Window::isDisplayMinimized() const
    {
        return m_displayState == DisplayState::Minimized;
    }

    bool Window::isDisplayMaximized() const
    {
        return m_displayState == DisplayState::Maximized;
    }

    Window::ThreeBrosState Window::getMinMaxBroState(bool isHover, bool isDown) const
    {
        if (isDown) return ThreeBrosState::Down;
        else if (isHover) return ThreeBrosState::Hover;
        else return ThreeBrosState::Idle;
    }

    Window::ThreeBrosState Window::getCloseXBroState(bool isHover, bool isDown) const
    {
        if (isDown) return ThreeBrosState::CloseDown;
        else if (isHover) return ThreeBrosState::CloseHover;
        else return ThreeBrosState::CloseIdle;
    }

    void Window::set3BrothersIconBrushState(ThreeBrosState state)
    {
        auto& setting = getAppearance().threeBrothers[(size_t)state];

        resource_utils::g_solidColorBrush->SetColor(setting.foreground.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.foreground.opacity);
    }

    void Window::set3BrothersButtonBrushState(ThreeBrosState state)
    {
        auto& setting = getAppearance().threeBrothers[(size_t)state];

        resource_utils::g_solidColorBrush->SetColor(setting.background.color);
        resource_utils::g_solidColorBrush->SetOpacity(setting.background.opacity);
    }

    void Window::registerTabGroup(WeakPtrParam<TabGroup> tg)
    {
        m_registeredTabGroups.insert(tg);
    }

    void Window::unregisterTabGroup(WeakPtrParam<TabGroup> tg)
    {
        m_registeredTabGroups.erase(tg);
    }

    void Window::handleMouseMoveForRegisteredTabGroups(MouseMoveEvent& e)
    {
        auto& p = e.cursorPoint;

        associatedTabGroup.reset();
        if (!m_isDragging) return;

        for (auto tgItor = m_registeredTabGroups.begin(); tgItor != m_registeredTabGroups.end(); )
        {
            if (!tgItor->expired())
            {
                auto tabGroup = tgItor->lock();
                if (math_utils::isInside(p, tabGroup->cardBarExtendedCardBarAbsoluteRect()))
                {
                    associatedTabGroup = tabGroup;
                    tabGroup->associatedWindow = std::dynamic_pointer_cast<Window>(shared_from_this());
                }
                else if (cpp_lang_utils::isMostDerivedEqual(tabGroup->associatedWindow.lock(), shared_from_this()))
                {
                    tabGroup->associatedWindow.reset();
                }
                tgItor++;
            }
            else tgItor = m_registeredTabGroups.erase(tgItor);
        }
    }

    void Window::handleMouseButtonForRegisteredTabGroups(MouseButtonEvent& e)
    {
        if (e.state.leftUp())
        {
            if (m_centerUIObject && !associatedTabGroup.expired())
            {
                auto tabGroup = associatedTabGroup.lock();

                // Retain this to avoid abrupt destruction after destory(),
                // otherwise the following demoting operation may collapse.
                auto temporaryLocked = shared_from_this();

                if (destroy())
                {
                    auto caption = makeUIObject<TabCaption>(m_caption);
                    caption->promotable = true;

                    tabGroup->insertTab({ caption, m_centerUIObject });
                    tabGroup->selectTab(0);

                    if (f_onTriggerTabDemoting) f_onTriggerTabDemoting(this, tabGroup.get());
                }
            }
        }
    }

    void Window::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        Panel::drawChildrenLayers(rndr);

        if (associatedTabGroup.expired())
        {
            // Shape of Shadow
            shadow.beginShadowDraw(rndr->d2d1DeviceContext());
            {
                rndr->d2d1DeviceContext()->Clear(D2D1::ColorF{ 0x000000, 1.0f });
            }
            shadow.endShadowDraw(rndr->d2d1DeviceContext());
        }
        // Content on Mask
        auto maskDrawTrans = D2D1::Matrix3x2F::Translation
        (
            -m_absoluteRect.left, -m_absoluteRect.top
        );
        mask.beginMaskDraw(rndr->d2d1DeviceContext(), maskDrawTrans);
        {
            // Background
            {
                resource_utils::g_solidColorBrush->SetColor(getAppearance().background.color);
                resource_utils::g_solidColorBrush->SetOpacity(getAppearance().background.opacity);

                ResizablePanel::drawBackground(rndr);
            }
            // Non-client Area
            {
                // Caption Panel
                auto& bkgn = getAppearance().captionPanel.background;

                resource_utils::g_solidColorBrush->SetColor(bkgn.color);
                resource_utils::g_solidColorBrush->SetOpacity(bkgn.opacity);

                auto cpr = captionPanelAbsoluteRect();

                rndr->d2d1DeviceContext()->FillRectangle(cpr, resource_utils::g_solidColorBrush.Get());

                // The caption icon-label is drawn as a child.

                // Decorative Bar
                auto dbr = decorativeBarAbsoluteRect();

                decorativeBarBrush->SetStartPoint({ dbr.left, dbr.top });
                decorativeBarBrush->SetEndPoint({ dbr.right, dbr.top });

                rndr->d2d1DeviceContext()->FillRectangle(dbr, decorativeBarBrush.Get());
            }
            // 3 Brothers
            {
                // Minimize Button
                if (isMinimizeEnabled)
                {
                    auto state = getMinMaxBroState(m_isMinimizeHover, m_isMinimizeDown);

                    set3BrothersButtonBrushState(state);

                    rndr->d2d1DeviceContext()->FillRectangle(
                        minimizeButtonAbsoluteRect(),
                        resource_utils::g_solidColorBrush.Get());

                    set3BrothersIconBrushState(state);

                    rndr->d2d1DeviceContext()->FillRectangle(
                        minimizeIconAbsoluteRect(),
                        resource_utils::g_solidColorBrush.Get());
                }
                // Maximize/Restore Button
                if (isMaximizeEnabled)
                {
                    auto state = getMinMaxBroState(m_isMaximizeHover, m_isMaximizeDown);

                    set3BrothersButtonBrushState(state);

                    rndr->d2d1DeviceContext()->FillRectangle(
                        maximizeButtonAbsoluteRect(),
                        resource_utils::g_solidColorBrush.Get());

                    set3BrothersIconBrushState(state);

                    // Maximize Button
                    if (isDisplayNormal())
                    {
                        rndr->d2d1DeviceContext()->DrawRectangle(
                            maximizeIconAbsoluteRect(),
                            resource_utils::g_solidColorBrush.Get(),
                            maximizeIconStrokeWidth());
                    }
                    else // Restore Button
                    {
                        // Body
                        auto rect = restoreIconAbsoluteRect();

                        rndr->d2d1DeviceContext()->DrawRectangle(
                            rect,
                            resource_utils::g_solidColorBrush.Get(),
                            restoreIconStrokeWidth());

                        // Ornament
                        D2D1_POINT_2F point00 =
                        {
                            rect.left,
                            rect.top - restoreOrnamentOffset()
                        };
                        D2D1_POINT_2F point10 =
                        {
                            rect.right + restoreOrnamentOffset(),
                            rect.bottom
                        };
                        D2D1_POINT_2F crossPoint = { point10.x, point00.y };

                        auto point01 = math_utils::increaseX(
                            crossPoint, -restoreIconStrokeWidth() * 0.5f);

                        // Top Dash
                        rndr->d2d1DeviceContext()->DrawLine(
                            point00, point01,
                            resource_utils::g_solidColorBrush.Get(),
                            restoreIconStrokeWidth());

                        auto point11 = math_utils::increaseY(
                            crossPoint, -restoreIconStrokeWidth() * 0.5f);

                        // Right Dash
                        rndr->d2d1DeviceContext()->DrawLine(
                            point10, point11,
                            resource_utils::g_solidColorBrush.Get(),
                            restoreIconStrokeWidth());
                    }
                }
                // Close Button
                if (isCloseEnabled)
                {
                    auto state = getCloseXBroState(m_isCloseHover, m_isCloseDown);

                    set3BrothersButtonBrushState(state);

                    rndr->d2d1DeviceContext()->FillRectangle(
                        closeButtonAbsoluteRect(),
                        resource_utils::g_solidColorBrush.Get());

                    set3BrothersIconBrushState(state);

                    auto iconRect = closeIconAbsoluteRect();

                    // Main Diagonal
                    rndr->d2d1DeviceContext()->DrawLine(
                        { iconRect.left, iconRect.top },
                        { iconRect.right, iconRect.bottom },
                        resource_utils::g_solidColorBrush.Get(),
                        closeIconStrokeWidth());

                    // Back Diagonal
                    rndr->d2d1DeviceContext()->DrawLine(
                        { iconRect.right, iconRect.top },
                        { iconRect.left, iconRect.bottom },
                        resource_utils::g_solidColorBrush.Get(),
                        closeIconStrokeWidth());
                }
            }
            // Children
            Panel::drawChildrenObjects(rndr);
        }
        mask.endMaskDraw(rndr->d2d1DeviceContext());
    }

    void Window::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Shadow
        if (associatedTabGroup.expired())
        {
            auto& shadowSetting = getAppearance().shadow;

            shadow.color = shadowSetting.color;
            shadow.standardDeviation = shadowSetting.standardDeviation;

            shadow.configShadowEffectInput(resource_utils::g_shadowEffect.Get());

            rndr->d2d1DeviceContext()->DrawImage(
                resource_utils::g_shadowEffect.Get(),
                math_utils::roundf(absolutePosition()));
        }
        // Content
        {
            float maskOpacity = associatedTabGroup.expired() ?
                mask.opacity : getAppearance().maskOpacityWhenDragAboveTabGroup;

            rndr->d2d1DeviceContext()->DrawBitmap(
                mask.bitmap.Get(), math_utils::roundf(m_absoluteRect), maskOpacity);
        }
    }

    bool Window::destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        if (cpp_lang_utils::isMostDerivedEqual(uiobj, m_caption)) return false;

        return Panel::destroyUIObjectHelper(uiobj);
    }

    void Window::onSizeHelper(SizeEvent& e)
    {
        ResizablePanel::onSizeHelper(e);

        auto bitmapSize = math_utils::roundu(e.size);

        mask.loadMaskBitmap(bitmapSize);
        shadow.loadShadowBitmap(bitmapSize);

        m_caption->transform(captionIconLabelSelfcoordRect());
        if (m_centerUIObject) m_centerUIObject->transform(clientAreaSelfcoordRect());
    }

    void Window::onChangeThemeHelper(WstrParam themeName)
    {
        ResizablePanel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);

        loadDecorativeBarBrush();
    }

    void Window::onMouseMoveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseMoveHelper(e);

        auto& p = e.cursorPoint;
        
        if (!m_isPerformSpecialOperation)
        {
            if (isMinimizeEnabled)
            {
                if (!math_utils::isOverlappedExcludingRight(p, minimizeButtonAbsoluteRect()))
                {
                    m_isMinimizeHover = false;
                    m_isMinimizeDown = false;
                }
                else m_isMinimizeHover = true;
            }
            if (isMaximizeEnabled)
            {
                if (!math_utils::isOverlapped(p, maximizeButtonAbsoluteRect()))
                {
                    m_isMaximizeHover = false;
                    m_isMaximizeDown = false;
                }
                else m_isMaximizeHover = true;
            }
            if (isCloseEnabled)
            {
                if (!math_utils::isOverlappedExcludingLeft(p, closeButtonAbsoluteRect()))
                {
                    m_isCloseHover = false;
                    m_isCloseDown = false;
                }
                else m_isCloseHover = true;
            }
        }
        else m_isMinimizeHover = m_isMaximizeHover = m_isCloseHover = false;

        DraggablePanel::onMouseMoveWrapper(e);
        ResizablePanel::onMouseMoveWrapper(e);

        handleMouseMoveForRegisteredTabGroups(e);
    }

    void Window::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseLeaveHelper(e);
        ResizablePanel::onMouseLeaveWrapper(e);

        m_isMinimizeHover = m_isMinimizeDown = false;
        m_isMaximizeHover = m_isMaximizeDown = false;
        m_isCloseHover = m_isCloseDown = false;
    }

    void Window::onMouseButtonHelper(MouseButtonEvent& e)
    {
        Panel::onMouseButtonHelper(e);

        if (respondSetForegroundEvent)
        {
            moveTopmost();
        }
        auto& p = e.cursorPoint;

        if (e.state.leftDown() || e.state.leftDblclk())
        {
            if (!m_isPerformSpecialOperation)
            {
                if (isMinimizeEnabled) m_isMinimizeDown = m_isMinimizeHover;
                if (isMaximizeEnabled) m_isMaximizeDown = m_isMaximizeHover;
                if (isCloseEnabled) m_isCloseDown = m_isCloseHover;
            }
            else m_isMinimizeDown = m_isMaximizeDown = m_isCloseDown = false;
        }
        else if (e.state.leftUp())
        {
            if (m_isMinimizeDown)
            {
                onMinimize();
            }
            else if (m_isMaximizeDown)
            {
                if (isDisplayNormal())
                {
                    onMaximize();
                }
                else onRestore();
            }
            else if (m_isCloseDown)
            {
                onClose();
            }
            m_isMinimizeDown = m_isMaximizeDown = m_isCloseDown = false;
        }
        DraggablePanel::onMouseButtonWrapper(e);
        ResizablePanel::onMouseButtonWrapper(e);

        handleMouseButtonForRegisteredTabGroups(e);
    }

    bool Window::isTriggerDraggingHelper(const Event::Point& p)
    {
        return math_utils::isInside(p, captionPanelAbsoluteRect()) &&
            !m_isMinimizeHover && !m_isMaximizeHover && !m_isCloseHover;
    }

    void Window::onStartDraggingHelper()
    {
        DraggablePanel::onStartDraggingHelper();

        m_isPerformSpecialOperation = true;
    }

    void Window::onEndDraggingHelper()
    {
        DraggablePanel::onEndDraggingHelper();

        m_isPerformSpecialOperation = isSizing();
    }

    void Window::onStartResizingHelper()
    {
        ResizablePanel::onStartResizingHelper();

        m_isPerformSpecialOperation = true;
    }

    void Window::onEndResizingHelper()
    {
        ResizablePanel::onEndResizingHelper();

        m_isPerformSpecialOperation = m_isDragging;
    }
}
