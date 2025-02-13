#include "Common/Precompile.h"

#include "UIKit/ResizablePanel.h"

#include "Common/DirectXError.h"
#include "Common/MathUtils/2D.h"

#include "UIKit/Application.h"
#include "UIKit/Cursor.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    ResizablePanel::ResizablePanel(
        const D2D1_RECT_F& rect,
        ComPtrParam<ID2D1Brush> brush,
        ComPtrParam<ID2D1Bitmap1> bitmap)
        :
        Panel(rect, brush, bitmap)
    {
        appEventReactability.focus.get = true;
    }

    void ResizablePanel::onInitializeFinish()
    {
        Panel::onInitializeFinish();

        staticSizingGuideFrame.loadStrokeStyle();
    }

    void ResizablePanel::StaticSizingGuideFrame::loadStrokeStyle()
    {
        THROW_IF_NULL(Application::g_app);

        auto factory = Application::g_app->dx12Renderer()->d2d1Factory();

        auto properties = D2D1::StrokeStyleProperties
        (
            /* startCap   */ D2D1_CAP_STYLE_ROUND,
            /* endCap     */ D2D1_CAP_STYLE_ROUND,
            /* dashCap    */ D2D1_CAP_STYLE_ROUND,
            /* lineJoin   */ D2D1_LINE_JOIN_MITER,
            /* miterLimit */ 10.0f,
            /* dashStyle  */ D2D1_DASH_STYLE_DASH,
            /* dashOffset */ 0.0f
        );
        THROW_IF_FAILED(factory->CreateStrokeStyle(properties, nullptr, 0, &strokeStyle));
    }

    D2D1_RECT_F ResizablePanel::sizingFrameExtendedRect(const D2D1_RECT_F& flatRect) const
    {
        auto& frameExt = getAppearance().sizingFrame.extension;
        return
        {
            flatRect.left   - (isLeftResizable   ? frameExt.left   : 0.0f),
            flatRect.top    - (isTopResizable    ? frameExt.top    : 0.0f),
            flatRect.right  + (isRightResizable  ? frameExt.right  : 0.0f),
            flatRect.bottom + (isBottomResizable ? frameExt.bottom : 0.0f)
        };
    }

    void ResizablePanel::onStartResizing()
    {
        onStartResizingHelper();

        if (f_onStartResizing) f_onStartResizing(this);
    }

    void ResizablePanel::onEndResizing()
    {
        onEndResizingHelper();

        if (f_onEndResizing) f_onEndResizing(this);
    }

    void ResizablePanel::onStartResizingHelper()
    {
        forceGlobalExclusiveFocusing = true;
    }

    void ResizablePanel::onEndResizingHelper()
    {
        forceGlobalExclusiveFocusing = false;
    }

    void ResizablePanel::setResizable(bool value)
    {
        isLeftResizable = isTopResizable = isRightResizable = isBottomResizable = value;
    }

    bool ResizablePanel::isSizing() const
    {
        return m_isLeftSizing || m_isTopSizing || m_isRightSizing || m_isBottomSizing;
    }

    bool ResizablePanel::isLeftSizingOnly() const
    {
        return m_isLeftSizing && !m_isTopSizing && !m_isRightSizing && !m_isBottomSizing;
    }

    bool ResizablePanel::isRightSizingOnly() const
    {
        return m_isRightSizing && !m_isLeftSizing && !m_isTopSizing && !m_isBottomSizing;
    }

    bool ResizablePanel::isTopSizingOnly() const
    {
        return m_isTopSizing && !m_isLeftSizing && !m_isRightSizing && !m_isBottomSizing;
    }

    bool ResizablePanel::isBottomSizingOnly() const
    {
        return m_isBottomSizing && !m_isLeftSizing && !m_isTopSizing && !m_isRightSizing;
    }

    bool ResizablePanel::isLeftTopSizing() const
    {
        return m_isLeftSizing && m_isTopSizing && !m_isRightSizing && !m_isBottomSizing;
    }

    bool ResizablePanel::isLeftBottomSizing() const
    {
        return m_isLeftSizing && m_isBottomSizing && !m_isTopSizing && !m_isRightSizing;
    }

    bool ResizablePanel::isRightTopSizing() const
    {
        return m_isRightSizing && m_isTopSizing && !m_isLeftSizing && !m_isBottomSizing;
    }

    bool ResizablePanel::isRightBottomSizing() const
    {
        return m_isRightSizing && m_isBottomSizing && !m_isLeftSizing && !m_isTopSizing;
    }

    bool ResizablePanel::isHitLeftTopSizingCorner(const D2D1_POINT_2F& p) const
    {
        auto& offset = getAppearance().sizingFrame.cornerOffset;
        return (p.x < m_absoluteRect.left && p.y < m_absoluteRect.top + offset.left) ||
               (p.x < m_absoluteRect.left + offset.top && p.y < m_absoluteRect.top);
    }

    bool ResizablePanel::isHitLeftBottomSizingCorner(const D2D1_POINT_2F& p) const
    {
        auto& offset = getAppearance().sizingFrame.cornerOffset;
        return (p.x < m_absoluteRect.left && p.y > m_absoluteRect.bottom - offset.left) ||
               (p.x < m_absoluteRect.left + offset.bottom && p.y > m_absoluteRect.bottom);
    }

    bool ResizablePanel::isHitRightTopSizingCorner(const D2D1_POINT_2F& p) const
    {
        auto& offset = getAppearance().sizingFrame.cornerOffset;
        return (p.x > m_absoluteRect.right && p.y < m_absoluteRect.top + offset.right) ||
               (p.x > m_absoluteRect.right - offset.top && p.y < m_absoluteRect.top);
    }

    bool ResizablePanel::isHitRightBottomSizingCorner(const D2D1_POINT_2F& p) const
    {
        auto& offset = getAppearance().sizingFrame.cornerOffset;
        return (p.x > m_absoluteRect.right && p.y > m_absoluteRect.bottom - offset.right) ||
               (p.x > m_absoluteRect.right - offset.bottom && p.y > m_absoluteRect.bottom);
    }

    void ResizablePanel::drawD2d1ObjectPosterior(Renderer* rndr)
    {
        // Static Sizing Frame
        if (!enableDynamicSizing && isSizing())
        {
            auto& frameSetting = getAppearance().staticSizingGuideFrame;

            resource_utils::g_solidColorBrush->SetColor(frameSetting.background.color);
            resource_utils::g_solidColorBrush->SetOpacity(frameSetting.background.opacity);

            auto frame = math_utils::inner(m_sizingRect, frameSetting.strokeWidth);
            D2D1_ROUNDED_RECT outlineRect = { relativeToAbsolute(frame), roundRadiusX, roundRadiusY };

            rndr->d2d1DeviceContext()->DrawRoundedRectangle(
                outlineRect, resource_utils::g_solidColorBrush.Get(),
                frameSetting.strokeWidth, staticSizingGuideFrame.strokeStyle.Get());
        }
    }

    bool ResizablePanel::isHitHelper(const Event::Point& p) const
    {
        return math_utils::isOverlapped(p, sizingFrameExtendedRect(m_absoluteRect));
    }

    void ResizablePanel::onChangeThemeHelper(WstrParam themeName)
    {
        Panel::onChangeThemeHelper(themeName);

        onChangeThemeWrapper(themeName);
    }

    void ResizablePanel::onChangeThemeWrapper(WstrParam themeName)
    {
        getAppearance().changeTheme(themeName);
    }

    void ResizablePanel::onMouseMoveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseMoveHelper(e);

        onMouseMoveWrapper(e);
    }

    void ResizablePanel::onMouseMoveWrapper(MouseMoveEvent& e)
    {
        THROW_IF_NULL(Application::g_app);

        auto& p = e.cursorPoint;

        float minWidth = minimalWidth();
        float minHeight = minimalHeight();

        float maxWidth = maximalWidth();
        float maxHeight = maximalHeight();

        auto relative = math_utils::offset(absoluteToRelative(p), math_utils::minus(m_sizingOffset));

        // Left

        if (isLeftResizable && isLeftSizingOnly())
        {
            float afterWidth = m_rect.right - relative.x;

            if (afterWidth < minWidth)
            {
                afterWidth = minWidth;
                relative.x = m_rect.right - minWidth;
            }
            else if (afterWidth > maxWidth)
            {
                afterWidth = maxWidth;
                relative.x = m_rect.right - maxWidth;
            }
            m_sizingRect = math_utils::rect(relative.x, m_rect.top, afterWidth, height());

            Application::g_app->cursor()->setIcon(Cursor::HorzSize);
        }

        // Right

        else if (isRightResizable && isRightSizingOnly())
        {
            float afterWidth = relative.x - m_rect.left;

            if (afterWidth < minWidth)
            {
                afterWidth = minWidth;
            }
            else if (afterWidth > maxWidth)
            {
                afterWidth = maxWidth;
            }
            m_sizingRect = math_utils::rect(m_rect.left, m_rect.top, afterWidth, height());

            Application::g_app->cursor()->setIcon(Cursor::HorzSize);
        }

        // Top

        else if (isTopResizable && isTopSizingOnly())
        {
            float afterHeight = m_rect.bottom - relative.y;

            if (afterHeight < minHeight)
            {
                afterHeight = minHeight;
                relative.y = m_rect.bottom - minHeight;
            }
            else if (afterHeight > maxHeight)
            {
                afterHeight = maxHeight;
                relative.y = m_rect.bottom - maxHeight;
            }
            m_sizingRect = math_utils::rect(m_rect.left, relative.y, width(), afterHeight);

            Application::g_app->cursor()->setIcon(Cursor::VertSize);
        }

        // Bottom

        else if (isBottomResizable && isBottomSizingOnly())
        {
            float afterHeight = relative.y - m_rect.top;

            if (afterHeight < minHeight)
            {
                afterHeight = minHeight;
            }
            else if (afterHeight > maxHeight)
            {
                afterHeight = maxHeight;
            }
            m_sizingRect = math_utils::rect(m_rect.left, m_rect.top, width(), afterHeight);

            Application::g_app->cursor()->setIcon(Cursor::VertSize);
        }

        // Left Top

        else if (isLeftResizable && isTopResizable && isLeftTopSizing())
        {
            float afterWidth = m_rect.right - relative.x;
            float afterHeight = m_rect.bottom - relative.y;

            if (afterWidth < minWidth)
            {
                afterWidth = minWidth;
                relative.x = m_rect.right - minWidth;
            }
            else if (afterWidth > maxWidth)
            {
                afterWidth = maxWidth;
                relative.x = m_rect.right - maxWidth;
            }
            if (afterHeight < minHeight)
            {
                afterHeight = minHeight;
                relative.y = m_rect.bottom - minHeight;
            }
            else if (afterHeight > maxHeight)
            {
                afterHeight = maxHeight;
                relative.y = m_rect.bottom - maxHeight;
            }
            m_sizingRect = math_utils::rect(relative.x, relative.y, afterWidth, afterHeight);

            Application::g_app->cursor()->setIcon(Cursor::MainDiag);
        }

        // Left Bottom

        else if (isLeftResizable && isBottomResizable && isLeftBottomSizing())
        {
            float afterWidth = m_rect.right - relative.x;
            float afterHeight = relative.y - m_rect.top;

            if (afterWidth < minWidth)
            {
                afterWidth = minWidth;
                relative.x = m_rect.right - minWidth;
            }
            else if (afterWidth > maxWidth)
            {
                afterWidth = maxWidth;
                relative.x = m_rect.right - maxWidth;
            }
            if (afterHeight < minHeight)
            {
                afterHeight = minHeight;
            }
            else if (afterHeight > maxHeight)
            {
                afterHeight = maxHeight;
            }
            m_sizingRect = math_utils::rect(relative.x, m_rect.top, afterWidth, afterHeight);

            Application::g_app->cursor()->setIcon(Cursor::BackDiag);
        }

        // Right Top

        else if (isRightResizable && isTopResizable && isRightTopSizing())
        {
            float afterWidth = relative.x - m_rect.left;
            float afterHeight = m_rect.bottom - relative.y;

            if (afterWidth < minWidth)
            {
                afterWidth = minWidth;
            }
            else if (afterWidth > maxWidth)
            {
                afterWidth = maxWidth;
            }
            if (afterHeight < minHeight)
            {
                afterHeight = minHeight;
                relative.y = m_rect.bottom - minHeight;
            }
            else if (afterHeight > maxHeight)
            {
                afterHeight = maxHeight;
                relative.y = m_rect.bottom - maxHeight;
            }
            m_sizingRect = math_utils::rect(m_rect.left, relative.y, afterWidth, afterHeight);

            Application::g_app->cursor()->setIcon(Cursor::BackDiag);
        }

        // Right Bottom

        else if (isRightResizable && isBottomResizable && isRightBottomSizing())
        {
            float afterWidth = relative.x - m_rect.left;
            float afterHeight = relative.y - m_rect.top;

            if (afterWidth < minWidth)
            {
                afterWidth = minWidth;
            }
            else if (afterWidth > maxWidth)
            {
                afterWidth = maxWidth;
            }
            if (afterHeight < minHeight)
            {
                afterHeight = minHeight;
            }
            else if (afterHeight > maxHeight)
            {
                afterHeight = maxHeight;
            }
            m_sizingRect = math_utils::rect(m_rect.left, m_rect.top, afterWidth, afterHeight);

            Application::g_app->cursor()->setIcon(Cursor::MainDiag);
        }

        // No Sizing

        else // Update miscellaneous fields.
        {
            m_isLeftHover = m_isTopHover = m_isRightHover = m_isBottomHover = false;

            if (isLeftResizable && isTopResizable && isHitLeftTopSizingCorner(p))
            {
                m_isLeftHover = m_isTopHover = true;

                Application::g_app->cursor()->setIcon(Cursor::MainDiag);
            }
            else if (isLeftResizable && isBottomResizable && isHitLeftBottomSizingCorner(p))
            {
                m_isLeftHover = m_isBottomHover = true;

                Application::g_app->cursor()->setIcon(Cursor::BackDiag);
            }
            else if (isRightResizable && isTopResizable && isHitRightTopSizingCorner(p))
            {
                m_isRightHover = m_isTopHover = true;

                Application::g_app->cursor()->setIcon(Cursor::BackDiag);
            }
            else if (isRightResizable && isBottomResizable && isHitRightBottomSizingCorner(p))
            {
                m_isRightHover = m_isBottomHover = true;

                Application::g_app->cursor()->setIcon(Cursor::MainDiag);
            }
            else if (isLeftResizable && p.x < m_absoluteRect.left)
            {
                m_isLeftHover = true;

                Application::g_app->cursor()->setIcon(Cursor::HorzSize);
            }
            else if (isTopResizable && p.y < m_absoluteRect.top)
            {
                m_isTopHover = true;

                Application::g_app->cursor()->setIcon(Cursor::VertSize);
            }
            else if (isRightResizable && p.x > m_absoluteRect.right)
            {
                m_isRightHover = true;

                Application::g_app->cursor()->setIcon(Cursor::HorzSize);
            }
            else if (isBottomResizable && p.y > m_absoluteRect.bottom)
            {
                m_isBottomHover = true;

                Application::g_app->cursor()->setIcon(Cursor::VertSize);
            }
        }
        if (isSizing())
        {
            m_skipDeliverNextMouseMoveEventToChildren = true;
            if (enableDynamicSizing) transform(m_sizingRect);
        }
    }

    void ResizablePanel::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseLeaveHelper(e);

        onMouseLeaveWrapper(e);
    }

    void ResizablePanel::onMouseLeaveWrapper(MouseMoveEvent& e)
    {
        if (!forceGlobalExclusiveFocusing)
        {
            m_isLeftHover = m_isLeftSizing = m_isTopHover = m_isTopSizing =
            m_isRightHover = m_isRightSizing = m_isBottomHover = m_isBottomSizing = false;
        }
    }

    void ResizablePanel::onMouseButtonHelper(MouseButtonEvent& e)
    {
        Panel::onMouseButtonHelper(e);

        onMouseButtonWrapper(e);
    }

    void ResizablePanel::onMouseButtonWrapper(MouseButtonEvent& e)
    {
        auto& p = e.cursorPoint;

        if (e.state.leftDown() || e.state.leftDblclk())
        {
            if (isLeftResizable)   m_isLeftSizing   = m_isLeftHover  ;
            if (isTopResizable)    m_isTopSizing    = m_isTopHover   ;
            if (isRightResizable)  m_isRightSizing  = m_isRightHover ;
            if (isBottomResizable) m_isBottomSizing = m_isBottomHover;

            if (isSizing())
            {
                m_sizingRect = m_rect;

                m_sizingOffset = absoluteToSelfCoord(p);

                if (m_isRightSizing) m_sizingOffset.x -= width();
                if (m_isBottomSizing) m_sizingOffset.y -= height();

                onStartResizing();
            }
        }
        else if (e.state.leftUp())
        {
            if (isSizing())
            {
                if (!enableDynamicSizing)
                {
                    transform(m_sizingRect);
                }
                m_isLeftSizing = m_isTopSizing = m_isRightSizing = m_isBottomSizing = false;
                
                onEndResizing();
            }
        }
    }
}
