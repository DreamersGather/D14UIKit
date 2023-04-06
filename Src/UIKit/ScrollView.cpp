#include "Common/Precompile.h"

#include "UIKit/ScrollView.h"

#include "Common/CppLangUtils/PointerEquality.h"
#include "Common/MathUtils/2D.h"

#include "Renderer/Renderer.h"

#include "UIKit/Application.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine::renderer;

namespace d14engine::uikit
{
    ScrollView::ScrollView(
        ShrdPtrParam<Panel> content,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        ResizablePanel(rect, resource_utils::g_solidColorBrush),
        contentMask(math_utils::roundu(size())),
        m_content(content)
    {
        m_takeOverChildrenDrawing = (m_content != nullptr);

        setResizable(false);
    }

    void ScrollView::onInitializeFinish()
    {
        ResizablePanel::onInitializeFinish();

        addUIObject(m_content);

        if (m_content) m_content->move(0, 0);
    }

    void ScrollView::onStartThumbScrolling(const D2D1_POINT_2F& offset)
    {
        onStartThumbScrollingHelper(offset);

        if (f_onStartThumbScrolling) f_onStartThumbScrolling(this, offset);
    }

    void ScrollView::onEndThumbScrolling(const D2D1_POINT_2F& offset)
    {
        onEndThumbScrollingHelper(offset);

        if (f_onEndThumbScrolling) f_onEndThumbScrolling(this, offset);
    }

    void ScrollView::onViewportOffsetChange(const D2D1_POINT_2F& offset)
    {
        onViewportOffsetChangeHelper(offset);

        if (f_onViewportOffsetChange) f_onViewportOffsetChange(this, offset);
    }

    void ScrollView::onStartThumbScrollingHelper(const D2D1_POINT_2F& offset)
    {
        forceGlobalExclusiveFocusing = true;
    }

    void ScrollView::onEndThumbScrollingHelper(const D2D1_POINT_2F& offset)
    {
        forceGlobalExclusiveFocusing = false;
    }

    void ScrollView::onViewportOffsetChangeHelper(const D2D1_POINT_2F& offset)
    {
        if (m_content) m_content->move(-offset.x, -offset.y);
    }

    D2D1_SIZE_F ScrollView::getSelfSize() const
    {
        if (customSizeGetter.self) return customSizeGetter.self(this);
        else return size();
    }

    D2D1_SIZE_F ScrollView::getContentSize() const
    {
        if (customSizeGetter.content) return customSizeGetter.content(this);
        else if (m_content) return m_content->size();
        else return { 0.0f, 0.0f };
    }

    void ScrollView::setContent(ShrdPtrParam<Panel> content)
    {
        if (!cpp_lang_utils::isMostDerivedEqual(content, m_content))
        {
            removeUIObject(m_content);

            m_content = content;
            addUIObject(m_content);

            if (m_content) m_content->move(0.0f, 0.0f);

            m_takeOverChildrenDrawing = (m_content != nullptr);

            m_viewportOffset = { 0.0f, 0.0f };
        }
    }

    D2D1_POINT_2F ScrollView::validateViewportOffset(const D2D1_POINT_2F& in)
    {
        D2D1_POINT_2F out = { 0.0f, 0.0f };

        auto selfSize = getSelfSize();
        auto contentSize = getContentSize();

        if (contentSize.width > selfSize.width)
        {
            out.x = std::clamp(in.x, 0.0f, contentSize.width - selfSize.width);
        }
        else out.x = 0.0f;

        if (contentSize.height > selfSize.height)
        {
            out.y = std::clamp(in.y, 0.0f, contentSize.height - selfSize.height);
        }
        else out.y = 0.0f; return out;
    }

    const D2D1_POINT_2F& ScrollView::viewportOffset() const
    {
        return m_viewportOffset;
    }

    void ScrollView::setViewportOffset(const D2D1_POINT_2F& absolute)
    {
        auto validOffset = validateViewportOffset(absolute);

        if (validOffset.x != m_viewportOffset.x ||
            validOffset.y != m_viewportOffset.y)
        {
            m_viewportOffset = validOffset;
            onViewportOffsetChange(validOffset);
        }
    }

    D2D1_POINT_2F ScrollView::viewportOffsetPercentage() const
    {
        auto selfSize = getSelfSize();
        auto contentSize = getContentSize();
        return
        {
            m_viewportOffset.x / (contentSize.width - selfSize.width),
            m_viewportOffset.y / (contentSize.height - selfSize.height)
        };
    }

    void ScrollView::setViewportOffsetPercentage(const D2D1_POINT_2F& relative)
    {
        auto selfSize = getSelfSize();
        auto contentSize = getContentSize();
        setViewportOffset
        ({
            relative.x * (contentSize.width - selfSize.width),
            relative.y * (contentSize.height - selfSize.height)
        });
    }

    void ScrollView::setViewportOffsetDirect(const D2D1_POINT_2F& offset)
    {
        m_viewportOffset = validateViewportOffset(offset);
    }

    bool ScrollView::isControllingHorzBar() const
    {
        return m_isHorzBarHover || m_isHorzBarDown;
    }

    bool ScrollView::isControllingVertBar() const
    {
        return m_isVertBarHover || m_isVertBarDown;
    }

    bool ScrollView::isControllingScrollBars() const
    {
        return isControllingHorzBar() || isControllingVertBar();
    }

    ScrollView::ScrollBarState ScrollView::getHorzBarState(bool isHover, bool isDown) const
    {
        if (m_isHorzBarDown) return ScrollBarState::Down;
        else if (m_isHorzBarHover) return ScrollBarState::Hover;
        else return ScrollBarState::Idle;
    }

    ScrollView::ScrollBarState ScrollView::getVertBarState(bool isHover, bool isDown) const
    {
        if (m_isVertBarDown) return ScrollBarState::Down;
        else if (m_isVertBarHover) return ScrollBarState::Hover;
        else return ScrollBarState::Idle;
    }

    D2D1_RECT_F ScrollView::horzBarSelfcoordRect(ScrollBarState state) const
    {
        auto& setting = getAppearance();
        auto& geoSetting = setting.scrollBar[(size_t)state].geometry;

        auto selfSize = getSelfSize();
        auto contentSize = getContentSize();

        if (selfSize.width > 0.0f && contentSize.width > selfSize.width)
        {
            float horzStart = m_viewportOffset.x / contentSize.width;
            float horzEnd = horzStart + selfSize.width / contentSize.width;
            return
            {
                horzStart * selfSize.width,
                height() - (geoSetting.offset + geoSetting.width),
                horzEnd   * selfSize.width,
                height() -  geoSetting.offset
            };
        }
        else return math_utils::zeroRectF();
    }

    D2D1_RECT_F ScrollView::vertBarSelfcoordRect(ScrollBarState state) const
    {
        auto& setting = getAppearance();
        auto& geoSetting = getAppearance().scrollBar[(size_t)state].geometry;

        auto selfSize = getSelfSize();
        auto contentSize = getContentSize();

        if (selfSize.height > 0.0f && contentSize.height > selfSize.height)
        {
            float vertStart = m_viewportOffset.y / contentSize.height;
            float vertEnd = vertStart + selfSize.height / contentSize.height;
            return
            {
                width()  - (geoSetting.offset + geoSetting.width),
                vertStart * selfSize.height,
                width()  -  geoSetting.offset,
                vertEnd   * selfSize.height
            };
        }
        else return math_utils::zeroRectF();
    }

    void ScrollView::onRendererDrawD2d1LayerHelper(Renderer* rndr)
    {
        if (m_content && m_content->isD2d1ObjectVisible())
        {
            m_content->onRendererDrawD2d1Layer(rndr);

            // Set opaque background to support rendering ClearType text.
            contentMask.color = getAppearance().background.color;
            contentMask.color.a = getAppearance().background.opacity;

            auto maskDrawTrans = D2D1::Matrix3x2F::Translation
            (
                -m_absoluteRect.left, -m_absoluteRect.top
            );
            contentMask.beginMaskDraw(rndr->d2d1DeviceContext(), maskDrawTrans);
            {
                m_content->onRendererDrawD2d1Object(rndr);
            }
            contentMask.endMaskDraw(rndr->d2d1DeviceContext());
        }
    }

    void ScrollView::onRendererDrawD2d1ObjectHelper(Renderer* rndr)
    {
        // Background
        resource_utils::g_solidColorBrush->SetColor(getAppearance().background.color);
        resource_utils::g_solidColorBrush->SetOpacity(getAppearance().background.opacity);

        ResizablePanel::drawBackground(rndr);

        // Content
        if (m_content && m_content->isD2d1ObjectVisible())
        {
            rndr->d2d1DeviceContext()->DrawBitmap(
                contentMask.bitmap.Get(), math_utils::roundf(m_absoluteRect));
        }
        // Outline
        resource_utils::g_solidColorBrush->SetColor(getAppearance().stroke.color);
        resource_utils::g_solidColorBrush->SetOpacity(getAppearance().stroke.opacity);

        float strokeWidth = getAppearance().stroke.width;

        auto frame = math_utils::inner(m_absoluteRect, strokeWidth);
        D2D1_ROUNDED_RECT outlineRect = { frame, roundRadiusX, roundRadiusY };

        rndr->d2d1DeviceContext()->DrawRoundedRectangle(
            outlineRect, resource_utils::g_solidColorBrush.Get(), strokeWidth);
    }

    void ScrollView::drawD2d1ObjectPosterior(renderer::Renderer* rndr)
    {
        // Horz Bar
        if (isHorzBarEnabled)
        {
            auto horzState = getHorzBarState(m_isHorzBarHover, m_isHorzBarDown);
            auto& horzSetting = getAppearance().scrollBar[(size_t)horzState];

            resource_utils::g_solidColorBrush->SetColor(horzSetting.background.color);
            resource_utils::g_solidColorBrush->SetOpacity(horzSetting.background.opacity);

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
            {
                selfCoordToAbsolute(horzBarSelfcoordRect(horzState)),
                horzSetting.geometry.roundRadius,
                horzSetting.geometry.roundRadius
            },
            resource_utils::g_solidColorBrush.Get());
        }
        // Vert Bar
        if (isVertBarEnabled)
        {
            auto vertState = getVertBarState(m_isVertBarHover, m_isVertBarDown);
            auto& vertSetting = getAppearance().scrollBar[(size_t)vertState];

            resource_utils::g_solidColorBrush->SetColor(vertSetting.background.color);
            resource_utils::g_solidColorBrush->SetOpacity(vertSetting.background.opacity);

            rndr->d2d1DeviceContext()->FillRoundedRectangle(
            {
                selfCoordToAbsolute(vertBarSelfcoordRect(vertState)),
                vertSetting.geometry.roundRadius,
                vertSetting.geometry.roundRadius
            },
            resource_utils::g_solidColorBrush.Get());
        }
        ResizablePanel::drawD2d1ObjectPosterior(rndr);
    }

    bool ScrollView::destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj)
    {
        if (cpp_lang_utils::isMostDerivedEqual(m_content, uiobj)) m_content.reset();

        return ResizablePanel::destroyUIObjectHelper(uiobj);
    }

    void ScrollView::onSizeHelper(SizeEvent& e)
    {
        ResizablePanel::onSizeHelper(e);

        contentMask.loadMaskBitmap(math_utils::roundu(e.size));

        // The viewport offset may be out of range after resizing.
        setViewportOffsetDirect(m_viewportOffset);
        onViewportOffsetChange(m_viewportOffset);
    }

    void ScrollView::onChangeThemeHelper(WstrParam themeName)
    {
        ResizablePanel::onChangeThemeHelper(themeName);

        getAppearance().changeTheme(themeName);
    }

    void ScrollView::onMouseMoveHelper(MouseMoveEvent& e)
    {
        ResizablePanel::onMouseMoveHelper(e);

        auto p = absoluteToSelfCoord(e.cursorPoint);

        auto selfSize = getSelfSize();

        // Perform viewport motion.
        if (selfSize.width > 0.0f && selfSize.height > 0.0f)
        {
            auto contentSize = getContentSize();

            float horzRatio = contentSize.width / selfSize.width;
            float vertRatio = contentSize.height / selfSize.height;

            if (m_isHorzBarDown)
            {
                m_skipDeliverNextMouseMoveEventToChildren = true;

                setViewportOffset(math_utils::offset(m_originalViewportOffset,
                    { std::round((p.x - m_horzBarHoldOffset) * horzRatio), 0.0f }));
            }
            if (m_isVertBarDown)
            {
                m_skipDeliverNextMouseMoveEventToChildren = true;

                setViewportOffset(math_utils::offset(m_originalViewportOffset,
                    { 0.0f, std::round((p.y - m_vertBarHoldOffset) * vertRatio) }));
            }
        }
        // Update scroll bar state.
        if (isHorzBarEnabled)
        {
            auto hstate = m_isHorzBarHover ? ScrollBarState::Hover : ScrollBarState::Idle;
            auto hrect = math_utils::overrideBottom(horzBarSelfcoordRect(hstate), FLT_MAX);

            m_isHorzBarHover = math_utils::isOverlapped(p, hrect);
        }
        if (isVertBarEnabled)
        {
            auto vstate = m_isVertBarHover ? ScrollBarState::Hover : ScrollBarState::Idle;
            auto vrect = math_utils::overrideRight(vertBarSelfcoordRect(vstate), FLT_MAX);

            m_isVertBarHover = math_utils::isOverlapped(p, vrect);
        }
        m_skipUpdateChildrenHitStatesInMouseMoveEvent = isControllingScrollBars();
    }

    void ScrollView::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        ResizablePanel::onMouseLeaveHelper(e);

        if (!forceGlobalExclusiveFocusing)
        {
            m_isHorzBarHover = m_isHorzBarDown =
            m_isVertBarHover = m_isVertBarDown = false;
        }
    }

    void ScrollView::onMouseButtonHelper(MouseButtonEvent& e)
    {
        ResizablePanel::onMouseButtonHelper(e);

        auto p = absoluteToSelfCoord(e.cursorPoint);

        if (e.state.leftDown() || e.state.leftDblclk())
        {
            if (isHorzBarEnabled)
            {
                m_isHorzBarDown = m_isHorzBarHover;
            }
            if (isVertBarEnabled)
            {
                m_isVertBarDown = m_isVertBarHover;
            }
            if (isControllingScrollBars())
            {
                onStartThumbScrolling(m_viewportOffset);
            }
            m_horzBarHoldOffset = p.x;
            m_vertBarHoldOffset = p.y;

            m_originalViewportOffset = m_viewportOffset;
        }
        else if (e.state.leftUp())
        {
            if (isControllingScrollBars())
            {
                onEndThumbScrolling(m_viewportOffset);
            }
            m_isHorzBarDown = m_isVertBarDown = false;
            m_horzBarHoldOffset = m_vertBarHoldOffset = 0.0f;
            m_originalViewportOffset = { 0.0f, 0.0f };
        }
    }

    void ScrollView::onMouseWheelHelper(MouseWheelEvent& e)
    {
        ResizablePanel::onMouseWheelHelper(e);

        D2D1_POINT_2F nextOffset = m_viewportOffset;

        if (e.keyState.CTRL)
        {
            nextOffset.x -= e.deltaCount * deltaPixelsPerScroll.horz;
        }
        else nextOffset.y -= e.deltaCount * deltaPixelsPerScroll.vert;

        setViewportOffset(nextOffset);

        Application::g_app->sendNextImmediateMouseMoveEvent = true;
    }
}
