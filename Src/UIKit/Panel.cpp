#include "Common/Precompile.h"

#include "UIKit/Panel.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/MathUtils/2D.h"
#include "Common/MathUtils/Basic.h"

#include "UIKit/Application.h"
#include "UIKit/BitmapObject.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    Panel::Panel(
        const D2D1_RECT_F& rect,
        ComPtrParam<ID2D1Brush> brush,
        ComPtrParam<ID2D1Bitmap1> bitmap)
        :
        m_rect(rect),
        brush(brush),
        bitmap(bitmap)
    {
        // Since we do not know anything about the priority context of the
        // new UI object, it is a good idea to move to the bottom at first.

        ISortable<IDrawObject2D>::m_priority = INT_MIN;
        ISortable<Panel>::m_priority = INT_MAX;

        updateAbsoluteRect();
    }

    void Panel::onInitializeFinish()
    {
        onChangeThemeHelper(Application::g_app->currThemeName());
        onChangeLangLocaleHelper(Application::g_app->currLangLocaleName());
    }

    bool Panel::destroy()
    {
        bool isDestroyed = true;

        // After Step 1, the ref-count of this becomes 0 if it is destroyed
        // successfully, so we must retain a temporary ptr to make sure the
        // callback in Step 2 can be performed correctly.
        auto temporaryLocked = shared_from_this();

        // Step 1
        if (m_parent.expired())
        {
            auto app = Application::g_app;
            app->dxRenderer()->waitGpuCommand();

            unregisterDrawObjects();
            unregisterApplicationEvents();
        }
        else isDestroyed = m_parent.lock()->destroyUIObject(temporaryLocked);

        // Step 2
        if (isDestroyed &&
            f_onDestroy)
        {
            f_onDestroy(this);
        }
        return isDestroyed;
    }

    bool Panel::destroyUIObject(ShrdPtrParam<Panel> uiobj)
    {
        auto app = Application::g_app;
        app->dxRenderer()->waitGpuCommand();

        if (f_destroyUIObject)
        {
            return f_destroyUIObject(this, uiobj);
        }
        else return destroyUIObjectHelper(uiobj);
    }

    bool Panel::isD2d1ObjectVisible() const
    {
        return m_visible;
    }

    void Panel::setD2d1ObjectVisible(bool value)
    {
        m_visible = value;
    }

    void Panel::onRendererUpdateObject2D(Renderer* rndr)
    {
        if (f_onRendererUpdateObject2DBefore) f_onRendererUpdateObject2DBefore(this, rndr);

        if (!m_takeOverChildrenUpdating) updateChildrenObjects(rndr);

        onRendererUpdateObject2DHelper(rndr);

        if (f_onRendererUpdateObject2DAfter) f_onRendererUpdateObject2DAfter(this, rndr);
    }

    void Panel::onRendererDrawD2d1Layer(Renderer* rndr)
    {
        if (f_onRendererDrawD2d1LayerBefore) f_onRendererDrawD2d1LayerBefore(this, rndr);

        if (!m_takeOverChildrenDrawing) drawChildrenLayers(rndr);

        onRendererDrawD2d1LayerHelper(rndr);

        if (f_onRendererDrawD2d1LayerAfter) f_onRendererDrawD2d1LayerAfter(this, rndr);
    }

    void Panel::onRendererDrawD2d1Object(Renderer* rndr)
    {
        if (f_onRendererDrawD2d1ObjectBefore) f_onRendererDrawD2d1ObjectBefore(this, rndr);

        if (!skipDrawPrecedingObjects) drawD2d1ObjectPreceding(rndr);

        onRendererDrawD2d1ObjectHelper(rndr);

        if (!m_takeOverChildrenDrawing) drawChildrenObjects(rndr);

        if (!skipDrawPosteriorObjects) drawD2d1ObjectPosterior(rndr);

        if (f_onRendererDrawD2d1ObjectAfter) f_onRendererDrawD2d1ObjectAfter(this, rndr);
    }

    void Panel::onRendererUpdateObject2DHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    void Panel::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    void Panel::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        drawBackground(rndr);
    }

    bool Panel::isPlayAnimation() const
    {
        return m_isPlayAnimation;
    }

    void Panel::increaseAnimationCount()
    {
        if (!m_isPlayAnimation)
        {
            m_isPlayAnimation = true;
            Application::g_app->increaseAnimationCount();
        }
    }

    void Panel::decreaseAnimationCount()
    {
        if (m_isPlayAnimation)
        {
            m_isPlayAnimation = false;
            Application::g_app->decreaseAnimationCount();
        }
    }

    void Panel::ApplicationEventReactability::setFlag(bool value)
    {
        ApplicationEventGroup::setFlag(value);

        hitTest = value;

        // There is no need to change the reactability of focus as it
        // depends on the result of mouse.button (through leftDown()).
    }

    bool Panel::isHit(const Event::Point& p) const
    {
        if (f_isHit) return f_isHit(this, p);
        else return isHitHelper(p);
    }

    float Panel::minimalWidth() const
    {
        return minimalWidthHint.has_value() ? minimalWidthHint.value() : 0.0f;
    }

    float Panel::minimalHeight() const
    {
        return minimalHeightHint.has_value() ? minimalHeightHint.value() : 0.0f;
    }

    D2D1_SIZE_F Panel::minimalSize() const
    {
        return { minimalWidth(), minimalHeight() };
    }

    float Panel::maximalWidth() const
    {
        return maximalWidthHint.has_value() ? maximalWidthHint.value() : FLT_MAX;
    }

    float Panel::maximalHeight() const
    {
        return maximalHeightHint.has_value() ? maximalHeightHint.value() : FLT_MAX;
    }

    D2D1_SIZE_F Panel::maximalSize() const
    {
        return { maximalWidth(), maximalHeight() };
    }

    void Panel::onSize(SizeEvent& e)
    {
        onSizeHelper(e);

        if (f_onSize) f_onSize(this, e);
    }

    void Panel::onParentSize(SizeEvent& e)
    {
        onParentSizeHelper(e);

        if (f_onParentSize) f_onParentSize(this, e);
    }

    void Panel::onMove(MoveEvent& e)
    {
        onMoveHelper(e);

        if (f_onMove) f_onMove(this, e);
    }

    void Panel::onParentMove(MoveEvent& e)
    {
        onParentMoveHelper(e);

        if (f_onParentMove) f_onParentMove(this, e);
    }

    void Panel::onChangeTheme(WstrParam themeName)
    {
        onChangeThemeHelper(themeName);

        if (f_onChangeTheme) f_onChangeTheme(this, themeName);
    }

    void Panel::onChangeLangLocale(WstrParam langLocaleName)
    {
        onChangeLangLocaleHelper(langLocaleName);

        if (f_onChangeLangLocale) f_onChangeLangLocale(this, langLocaleName);
    }

    void Panel::onGetFocus()
    {
        onGetFocusHelper();

        if (f_onGetFocus) f_onGetFocus(this);
    }

    void Panel::onLoseFocus()
    {
        onLoseFocusHelper();

        if (f_onLoseFocus) f_onLoseFocus(this);
    }

    bool Panel::isFocused() const
    {
        return cpp_lang_utils::isMostDerivedEqual(
            Application::g_app->currFocusedUIObject().lock(), shared_from_this());
    }

    void Panel::onMouseEnter(MouseMoveEvent& e)
    {
        onMouseEnterHelper(e);

        if (f_onMouseEnter) f_onMouseEnter(this, e);
    }

    void Panel::onMouseMove(MouseMoveEvent& e)
    {
        onMouseMoveHelper(e);

        if (f_onMouseMove) f_onMouseMove(this, e);
    }

    void Panel::onMouseLeave(MouseMoveEvent& e)
    {
        onMouseLeaveHelper(e);

        if (f_onMouseLeave) f_onMouseLeave(this, e);
    }

    void Panel::onMouseButton(MouseButtonEvent& e)
    {
        onMouseButtonHelper(e);

        if (f_onMouseButton) f_onMouseButton(this, e);
    }

    void Panel::onMouseWheel(MouseWheelEvent& e)
    {
        onMouseWheelHelper(e);

        if (f_onMouseWheel) f_onMouseWheel(this, e);
    }

    void Panel::onKeyboard(KeyboardEvent& e)
    {
        onKeyboardHelper(e);

        if (f_onKeyboard) f_onKeyboard(this, e);
    }

    bool Panel::isHitHelper(const Event::Point& p) const
    {
        return math_utils::isOverlapped(p, m_absoluteRect);
    }

    bool Panel::destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        removeUIObject(uiobj);
        return true;
    }

    void Panel::onSizeHelper(SizeEvent& e)
    {
        for (auto& child : m_children)
        {
            child->onParentSize(e);
        }
    }

    void Panel::onParentSizeHelper(SizeEvent& e)
    {
        // This method intentionally left blank.
    }

    void Panel::onMoveHelper(MoveEvent& e)
    {
        MoveEvent me = {};
        // Always (0,0) in the self coordinate.
        me.position = { 0.0f, 0.0f };

        for (auto& child : m_children)
        {
            child->onParentMove(me);
        }
    }

    void Panel::onParentMoveHelper(MoveEvent& e)
    {
        updateAbsoluteRect();

        MoveEvent me = {};
        me.position = { m_rect.left, m_rect.top };

        onMove(me);
    }

    void Panel::onChangeThemeHelper(WstrParam themeName)
    {
        if (m_skipChangeChildrenThemes) return;

        for (auto& child : m_children)
        {
            child->onChangeTheme(themeName);
        }
    }

    void Panel::onChangeLangLocaleHelper(WstrParam langLocaleName)
    {
        if (m_skipChangeChildrenLangLocale) return;

        for (auto& child : m_children)
        {
            child->onChangeLangLocale(langLocaleName);
        }
    }

    void Panel::onGetFocusHelper()
    {
        appEventReactability.keyboard = false;
    }

    void Panel::onLoseFocusHelper()
    {
        appEventReactability.keyboard = true;
    }

    void Panel::onMouseEnterHelper(MouseMoveEvent& e)
    {
        // This method intentionally left blank.
    }

    void Panel::onMouseMoveHelper(MouseMoveEvent& e)
    {
        if (m_skipDeliverNextMouseMoveEventToChildren)
        {
            m_skipDeliverNextMouseMoveEventToChildren = false;
            return;
        }
        ChildObjectTempSet currHitChildren = {};

        if (!m_skipUpdateChildrenHitStatesInMouseMoveEvent)
        {
            for (auto& child : m_children)
            {
                if (child->appEventReactability.hitTest && child->isHit(e.cursorPoint))
                {
                    currHitChildren.insert(child);
                }
            }
        }
        if (forceSingleMouseEnterLeaveEvent)
        {
            WeakPtr<Panel> enterCandidate = {}, leaveCandidate = {};
            if (!currHitChildren.empty())
            {
                enterCandidate = *currHitChildren.begin();
            }
            if (!m_hitChildren.empty())
            {
                leaveCandidate = *m_hitChildren.begin();
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
            ISortable<Panel>::foreach(currHitChildren, [&](ShrdPtrParam<Panel> child)
            {
                // Moved in just now, trigger onMouseEnter event.
                if (m_hitChildren.find(child) == m_hitChildren.end())
                {
                    if (child->appEventReactability.mouse.enter)
                    {
                        child->onMouseEnter(e);
                    }
                    return child->appEventTransparency.mouse.enter;
                }
                return true;
            });
            ISortable<Panel>::foreach(m_hitChildren, [&](ShrdPtrParam<Panel> child)
            {
                // Moved out just now, trigger onMouseLeave event.
                if (currHitChildren.find(child) == currHitChildren.end())
                {
                    if (child->appEventReactability.mouse.leave)
                    {
                        child->onMouseLeave(e);
                    }
                    return child->appEventTransparency.mouse.leave;
                }
                return true;
            });
        }
        m_hitChildren = std::move(currHitChildren);

        ISortable<Panel>::foreach(m_hitChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.mouse.move)
            {
                child->onMouseMove(e);
            }
            return child->appEventTransparency.mouse.move;
        });
        updateDiffPinnedUIObjects();

        ISortable<Panel>::foreach(m_diffPinnedChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.mouse.move)
            {
                child->onMouseMove(e);
            }
            return child->appEventTransparency.mouse.move;
        });
    }

    void Panel::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        if (forceTriggerChildrenMouseLeaveEvents)
        {
            ISortable<Panel>::foreach(m_hitChildren, [&](ShrdPtrParam<Panel> child)
            {
                if (child->appEventReactability.mouse.leave)
                {
                    child->onMouseLeave(e);
                }
                return child->appEventTransparency.mouse.leave;
            });
            m_hitChildren.clear();
        }
        else onMouseMoveHelper(e); // use mouse-move to simulate mouse-leave
    }

    void Panel::onMouseButtonHelper(MouseButtonEvent& e)
    {
        ISortable<Panel>::foreach(m_hitChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.mouse.button)
            {
                if (child->appEventReactability.focus.get &&
                   (e.state.leftDown() || e.state.leftDblclk()))
                {
                    e.focused = child;
                }
                child->onMouseButton(e);
            }
            return child->appEventTransparency.mouse.button;
        });
        ISortable<Panel>::foreach(m_diffPinnedChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.mouse.button)
            {
                child->onMouseButton(e);
            }
            return child->appEventTransparency.mouse.button;
        });
    }

    void Panel::onMouseWheelHelper(MouseWheelEvent& e)
    {
        ISortable<Panel>::foreach(m_hitChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.mouse.wheel)
            {
                child->onMouseWheel(e);
            }
            return child->appEventTransparency.mouse.wheel;
        });
        ISortable<Panel>::foreach(m_diffPinnedChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.mouse.wheel)
            {
                child->onMouseWheel(e);
            }
            return child->appEventTransparency.mouse.wheel;
        });
    }

    void Panel::onKeyboardHelper(KeyboardEvent& e)
    {
        ISortable<Panel>::foreach(m_hitChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.keyboard)
            {
                child->onKeyboard(e);
            }
            return child->appEventTransparency.keyboard;
        });
        ISortable<Panel>::foreach(m_diffPinnedChildren, [&](ShrdPtrParam<Panel> child)
        {
            if (child->appEventReactability.keyboard)
            {
                child->onKeyboard(e);
            }
            return child->appEventTransparency.keyboard;
        });
    }

    int Panel::d2d1ObjectPriority() const
    {
        return ISortable<IDrawObject2D>::priority();
    }

    void Panel::setD2d1ObjectPriority(int value)
    {
        if (m_parent.expired())
        {
            auto& uiCmdLayer = Application::g_app->uiCmdLayer();
            if (std::holds_alternative<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget))
            {
                auto& drawobjs2d = std::get<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget);
                if (drawobjs2d.find(shared_from_this()) != drawobjs2d.end())
                {
                    unregisterDrawObjects();

                    ISortable<IDrawObject2D>::m_priority = value;
                    registerDrawObjects();

                    Application::g_app->m_topmostPriority.d2d1Object = std::max
                    (
                        Application::g_app->m_topmostPriority.d2d1Object, value
                    );
                }
            }
        }
        else // Managed by another object.
        {
            auto parentPtr = m_parent.lock();
            parentPtr->removeUIObject(shared_from_this());

            ISortable<IDrawObject2D>::m_priority = value;
            parentPtr->addUIObject(shared_from_this());

            parentPtr->m_topmostPriority.d2d1Object = std::max
            (
                parentPtr->m_topmostPriority.d2d1Object, value
            );
        }
        ISortable<IDrawObject2D>::m_priority = value;
    }

    int Panel::uiObjectPriority() const
    {
        return ISortable<Panel>::priority();
    }

    void Panel::setUIObjectPriority(int value)
    {
        if (m_parent.expired())
        {
            auto& uiobjs = Application::g_app->uiObjects();
            if (uiobjs.find(shared_from_this()) != uiobjs.end())
            {
                unregisterApplicationEvents();

                ISortable<Panel>::m_priority = value;
                registerApplicationEvents();

                Application::g_app->m_topmostPriority.uiObject = std::min
                (
                    Application::g_app->m_topmostPriority.uiObject, value
                );
            }
        }
        else // Managed by another object.
        {
            auto parentPtr = m_parent.lock();
            parentPtr->removeUIObject(shared_from_this());

            ISortable<Panel>::m_priority = value;
            parentPtr->addUIObject(shared_from_this());

            parentPtr->m_topmostPriority.uiObject = std::min
            (
                parentPtr->m_topmostPriority.uiObject, value
            );
        }
        ISortable<Panel>::m_priority = value;
    }

    const Panel::TopmostPriority& Panel::topmostPriority() const
    {
        return m_topmostPriority;
    }

    void Panel::moveTopmost()
    {
        if (m_parent.expired())
        {
            Application::g_app->moveRootObjectTopmost(this);
        }
        else // Managed by another object.
        {
            m_parent.lock()->moveChildObjectTopmost(this);
        }
    }

    void Panel::moveChildObjectTopmost(Panel* uiobj)
    {
        uiobj->setD2d1ObjectPriority(++m_topmostPriority.d2d1Object);
        uiobj->setUIObjectPriority(--m_topmostPriority.uiObject);
    }

    void Panel::moveAbovePeerObject(Panel* uiobj)
    {
        setD2d1ObjectPriority(uiobj->d2d1ObjectPriority() + 1);
        setUIObjectPriority(uiobj->uiObjectPriority() - 1);
    }

    void Panel::moveBelowPeerObject(Panel* uiobj)
    {
        setD2d1ObjectPriority(uiobj->d2d1ObjectPriority() - 1);
        setUIObjectPriority(uiobj->uiObjectPriority() + 1);
    }

    void Panel::updateChildrenObjects(Renderer* rndr)
    {
        for (auto& child : m_children)
        {
            if (child->isD2d1ObjectVisible())
            {
                child->onRendererUpdateObject2D(rndr);
            }
        }
    }

    void Panel::drawChildrenLayers(Renderer* rndr)
    {
        for (auto& child : m_children)
        {
            if (child->isD2d1ObjectVisible())
            {
                child->onRendererDrawD2d1Layer(rndr);
            }
        }
    }

    void Panel::drawBackground(Renderer* rndr)
    {
        if (brush)
        {
            rndr->d2d1DeviceContext()->FillRoundedRectangle(
                { m_absoluteRect, roundRadiusX, roundRadiusY }, brush.Get());
        }
        if (bitmap)
        {
            rndr->d2d1DeviceContext()->DrawBitmap( // round to fit pixel size
                bitmap.Get(), math_utils::roundf(m_absoluteRect),
                bitmapOpacity, BitmapObject::g_interpolationMode);
        }
    }

    void Panel::drawChildrenObjects(Renderer* rndr)
    {
        for (auto& obj2d : m_drawObjects2D)
        {
            if (obj2d->isD2d1ObjectVisible())
            {
                obj2d->onRendererDrawD2d1Object(rndr);
            }
        }
    }

    void Panel::drawD2d1ObjectPreceding(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    void Panel::drawD2d1ObjectPosterior(Renderer* rndr)
    {
        // This method intentionally left blank.
    }

    bool Panel::visible() const
    {
        return isD2d1ObjectVisible();
    }

    void Panel::setVisible(bool value)
    {
        setD2d1ObjectVisible(value);
    }

    bool Panel::enabled() const
    {
        return m_enabled;
    }

    void Panel::setEnabled(bool value)
    {
        m_enabled = value;

        appEventReactability.setFlag(value);
    }

    float Panel::width() const
    {
        return math_utils::width(m_rect);
    }

    float Panel::height() const
    {
        return math_utils::height(m_rect);
    }

    D2D1_SIZE_F Panel::size() const
    {
        return { width(), height() };
    }

    D2D1_POINT_2F Panel::position() const
    {
        return { m_rect.left, m_rect.top };
    }

    D2D_POINT_2F Panel::absolutePosition() const
    {
        return { m_absoluteRect.left, m_absoluteRect.top };
    }

    D2D1_RECT_F Panel::selfCoordRect() const
    {
        return { 0.0f, 0.0f, width(), height() };
    }

    const D2D1_RECT_F& Panel::relativeRect() const
    {
        return m_rect;
    }

    const D2D1_RECT_F& Panel::absoluteRect() const
    {
        return m_absoluteRect;
    }

    D2D1_POINT_2F Panel::selfCoordToRelative(const D2D1_POINT_2F& p) const
    {
        return { p.x + m_rect.left, p.y + m_rect.top };
    }

    D2D1_RECT_F Panel::selfCoordToRelative(const D2D1_RECT_F& rect) const
    {
        auto leftTop = selfCoordToRelative(D2D1_POINT_2F{ rect.left, rect.top });
        auto rightBottom = selfCoordToRelative(D2D1_POINT_2F{ rect.right, rect.bottom });

        return { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
    }

    D2D1_POINT_2F Panel::selfCoordToAbsolute(const D2D1_POINT_2F& p) const
    {
        return { p.x + m_absoluteRect.left, p.y + m_absoluteRect.top };
    }

    D2D1_RECT_F Panel::selfCoordToAbsolute(const D2D1_RECT_F& rect) const
    {
        auto leftTop = selfCoordToAbsolute(D2D1_POINT_2F{ rect.left, rect.top });
        auto rightBottom = selfCoordToAbsolute(D2D1_POINT_2F{ rect.right, rect.bottom });

        return { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
    }

    D2D1_POINT_2F Panel::relativeToSelfCoord(const D2D1_POINT_2F& p) const
    {
        return { p.x - m_rect.left, p.y - m_rect.top };
    }

    D2D1_RECT_F Panel::relativeToSelfCoord(const D2D1_RECT_F& rect) const
    {
        auto leftTop = relativeToSelfCoord(D2D1_POINT_2F{ rect.left, rect.top });
        auto rightBottom = relativeToSelfCoord(D2D1_POINT_2F{ rect.right, rect.bottom });

        return { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
    }

    D2D1_POINT_2F Panel::relativeToAbsolute(const D2D1_POINT_2F& p) const
    {
        return { p.x - m_rect.left + m_absoluteRect.left, p.y - m_rect.top + m_absoluteRect.top };
    }

    D2D1_RECT_F Panel::relativeToAbsolute(const D2D1_RECT_F& rect) const
    {
        auto leftTop = relativeToAbsolute(D2D1_POINT_2F{ rect.left, rect.top });
        auto rightBottom = relativeToAbsolute(D2D1_POINT_2F{ rect.right, rect.bottom });

        return { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
    }

    D2D1_POINT_2F Panel::absoluteToSelfCoord(const D2D1_POINT_2F& p) const
    {
        return { p.x - m_absoluteRect.left, p.y - m_absoluteRect.top };
    }

    D2D1_RECT_F Panel::absoluteToSelfCoord(const D2D1_RECT_F& rect) const
    {
        auto leftTop = absoluteToSelfCoord(D2D1_POINT_2F{ rect.left, rect.top });
        auto rightBottom = absoluteToSelfCoord(D2D1_POINT_2F{ rect.right, rect.bottom });

        return { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
    }

    D2D1_POINT_2F Panel::absoluteToRelative(const D2D1_POINT_2F& p) const
    {
        return { p.x + m_rect.left - m_absoluteRect.left, p.y + m_rect.top - m_absoluteRect.top };
    }

    D2D1_RECT_F Panel::absoluteToRelative(const D2D1_RECT_F& rect) const
    {
        auto leftTop = absoluteToRelative(D2D1_POINT_2F{ rect.left, rect.top });
        auto rightBottom = absoluteToRelative(D2D1_POINT_2F{ rect.right, rect.bottom });

        return { leftTop.x, leftTop.y, rightBottom.x, rightBottom.y };
    }

    void Panel::resize(const D2D1_SIZE_F& size)
    {
        m_rect.right = m_rect.left + std::clamp(size.width, minimalWidth(), maximalWidth());
        m_rect.bottom = m_rect.top + std::clamp(size.height, minimalHeight(), maximalHeight());
        updateAbsoluteRect();
    }

    void Panel::resize(float width, float height)
    {
        resize({ width, height });
    }

    void Panel::move(const D2D1_POINT_2F& point)
    {
        m_rect = math_utils::rect(point.x, point.y, width(), height());
        updateAbsoluteRect();
    }

    void Panel::move(float left, float top)
    {
        move({ left, top });
    }

    void Panel::transform(const D2D1_RECT_F& rect)
    {
        float rectWidth = math_utils::width(rect);
        float rectHeight = math_utils::height(rect);

        if (rectWidth >= minimalWidth() && rectWidth <= maximalWidth() &&
            rectHeight >= minimalHeight() && rectHeight <= maximalHeight())
        {
            m_rect = rect;
            updateAbsoluteRect();
        }
    }

    void Panel::transform(float left, float top, float width, float height)
    {
        transform(math_utils::rect(left, top, width, height));
    }

    void Panel::updateAbsoluteRect()
    {
        auto originalSize = math_utils::size(m_absoluteRect);
        auto originalPosition = absolutePosition();

        if (!m_parent.expired())
        {
            m_absoluteRect = math_utils::offset(m_rect, m_parent.lock()->absolutePosition());
        }
        else m_absoluteRect = m_rect;

        if (math_utils::round(originalSize.width) != math_utils::round(width()) ||
            math_utils::round(originalSize.height) != math_utils::round(height()))
        {
            SizeEvent e = {};
            e.size = { width(), height() };

            onSize(e);
        }
        if (math_utils::round(originalPosition.x) != math_utils::round(m_absoluteRect.left) ||
            math_utils::round(originalPosition.y) != math_utils::round(m_absoluteRect.top))
        {
            MoveEvent me = {};
            me.position = { m_rect.left, m_rect.top };

            onMove(me);
        }
    }

    void Panel::registerDrawObjects()
    {
        auto& uiCmdLayer = Application::g_app->uiCmdLayer();
        if (std::holds_alternative<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget))
        {
            auto& drawobjs2d = std::get<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget);
            drawobjs2d.insert(shared_from_this());
        }
        Application::g_app->m_topmostPriority.d2d1Object = std::max
        (
            Application::g_app->m_topmostPriority.d2d1Object,
            ISortable<IDrawObject2D>::m_priority
        );
    }

    void Panel::unregisterDrawObjects()
    {
        auto& uiCmdLayer = Application::g_app->uiCmdLayer();
        if (std::holds_alternative<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget))
        {
            auto& drawobjs2d = std::get<Renderer::CommandLayer::D2D1Target>(uiCmdLayer->drawTarget);
            drawobjs2d.erase(shared_from_this());
        }
    }

    void Panel::registerApplicationEvents()
    {
        Application::g_app->addUIObject(shared_from_this());

        // The priority has already been updated in addUIObject.
    }

    void Panel::unregisterApplicationEvents()
    {
        Application::g_app->removeUIObject(shared_from_this());
    }

    void Panel::pinApplicationEvents()
    {
        if (m_parent.expired())
        {
            Application::g_app->pinUIObject(shared_from_this());
        }
        else m_parent.lock()->pinUIObject(shared_from_this());
    }

    void Panel::unpinApplicationEvents()
    {
        if (m_parent.expired())
        {
            Application::g_app->unpinUIObject(shared_from_this());
        }
        else m_parent.lock()->unpinUIObject(shared_from_this());
    }

    const WeakPtr<Panel>& Panel::parent() const
    {
        return m_parent;
    }

    void Panel::setParent(ShrdPtrParam<Panel> uiobj)
    {
        if (!uiobj)
        {
            if (!m_parent.expired())
            {
                m_parent.lock()->removeUIObject(shared_from_this());
                m_parent.reset();
            }
            return;
        }
        if (!m_parent.expired())
        {
            auto pa = m_parent.lock();
            if (!cpp_lang_utils::isMostDerivedEqual(pa, uiobj))
            {
                // The ref-count of this may be 0 after removed from parent,
                // so we must retain a temporary ptr to avoid sudden release.
                auto temporaryLocked = shared_from_this();

                pa->removeUIObject(temporaryLocked);
                uiobj->addUIObject(temporaryLocked);
            }
        }
        else uiobj->addUIObject(shared_from_this());
    }

    const Panel::ChildObjectSet& Panel::children() const
    {
        return m_children;
    }

    void Panel::addUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (!uiobj) return;
        if (m_children.find(uiobj) != m_children.end()) return;

        if (!uiobj->m_parent.expired())
        {
            uiobj->m_parent.lock()->removeUIObject(uiobj);
        }
        uiobj->m_parent = shared_from_this();

        m_children.insert(uiobj);
        m_drawObjects2D.insert(uiobj);

        m_topmostPriority.uiObject = std::min
        (
            m_topmostPriority.uiObject,
            uiobj->ISortable<Panel>::m_priority
        );
        m_topmostPriority.d2d1Object = std::max
        (
            m_topmostPriority.d2d1Object,
            uiobj->ISortable<IDrawObject2D>::m_priority
        );
        // Changing parent should not trigger size/move-event.
        uiobj->m_rect = math_utils::offset(
            uiobj->m_absoluteRect,
            math_utils::minus(absolutePosition()));
    }

    void Panel::removeUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (!uiobj) return;
        if (m_children.find(uiobj) != m_children.end())
        {
            uiobj->m_parent.reset();
            uiobj->m_rect = uiobj->m_absoluteRect;
        }
        m_children.erase(uiobj);
        m_drawObjects2D.erase(uiobj);
    }

    void Panel::pinUIObject(ShrdPtrParam<Panel> uiobj)
    {
        if (!uiobj) return;
        m_pinnedChildren.insert(uiobj);

        updateDiffPinnedUIObjectsLater();
    }

    void Panel::unpinUIObject(ShrdPtrParam<Panel> uiobj)
    {
        m_pinnedChildren.erase(uiobj);

        updateDiffPinnedUIObjectsLater();
    }

    void Panel::clearAddedUIObjects()
    {
        for (auto& child : m_children)
        {
            child->m_parent.reset();
            child->m_rect = child->m_absoluteRect;
        }
        m_children.clear();
        m_drawObjects2D.clear();
    }

    void Panel::clearPinnedUIObjects()
    {
        m_pinnedChildren.clear();

        updateDiffPinnedUIObjectsLater();
    }

    void Panel::updateDiffPinnedUIObjects()
    {
        m_diffPinnedChildren.clear();

        std::set_difference(
            m_pinnedChildren.begin(), m_pinnedChildren.end(),
            m_hitChildren.begin(), m_hitChildren.end(),
            std::inserter(m_diffPinnedChildren, m_diffPinnedChildren.begin()),
            ISortable<Panel>::WeakAscending()); // Can't deduce automatically.
    }

    void Panel::updateDiffPinnedUIObjectsLater()
    {
        auto app = Application::g_app;
        using CustomMsg = Application::CustomWin32Message;

        app->pushDiffPinnedUpdateCandidate(shared_from_this());
        app->postCustomWin32Message(CustomMsg::UpdateMiscDiffPinnedUIObjects);
    }
}
