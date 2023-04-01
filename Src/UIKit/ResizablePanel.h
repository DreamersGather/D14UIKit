#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/ResizablePanel.h"
#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct ResizablePanel : appearance::ResizablePanel, virtual Panel
    {
        ResizablePanel(
            const D2D1_RECT_F& rect = {},
            ComPtrParam<ID2D1Brush> brush = nullptr,
            ComPtrParam<ID2D1Bitmap1> bitmap = nullptr);

        void onInitializeFinish() override;

        struct StaticSizingGuideFrame
        {
            ComPtr<ID2D1StrokeStyle> strokeStyle = {};
        }
        staticSizingGuideFrame = {};

        void loadStaticSizingGuideFrameStrokeStyle();

        D2D1_RECT_F sizingFrameExtendedRect(const D2D1_RECT_F& flatRect) const;

        _D14_SET_APPEARANCE_GETTER(ResizablePanel)

    public:
        void onStartResizing();

        Function<void(ResizablePanel*)> f_onStartResizing = {};

        void onEndResizing();

        Function<void(ResizablePanel*)> f_onEndResizing = {};

    protected:
        virtual void onStartResizingHelper();
        virtual void onEndResizingHelper();

    public:
        bool isLeftResizable = true, isRightResizable = true;
        bool isTopResizable = true, isBottomResizable = true;

        void setResizable(bool value);

    protected:
        D2D1_RECT_F m_sizingRect = {};

        // Relative distance from the held point to the boundary of the panel.
        D2D1_POINT_2F m_sizingOffset = {};

    public:
        bool enableDynamicSizing = true;

    protected:
        bool m_isLeftHover = false, m_isRightHover = false;
        bool m_isTopHover = false, m_isBottomHover = false;

        bool m_isLeftSizing = false, m_isRightSizing = false;
        bool m_isTopSizing = false, m_isBottomSizing = false;

    public:
        bool isSizing() const;

        bool isLeftSizingOnly() const;
        bool isRightSizingOnly() const;
        bool isTopSizingOnly() const;
        bool isBottomSizingOnly() const;

        bool isLeftTopSizing() const;
        bool isLeftBottomSizing() const;
        bool isRightTopSizing() const;
        bool isRightBottomSizing() const;

    protected:
        // These methods give the correct results only when the point
        // hits (i.e. taking the sizing frame extension into account).

        bool isHitLeftTopSizingCorner(const D2D1_POINT_2F& p) const;
        bool isHitLeftBottomSizingCorner(const D2D1_POINT_2F& p) const;
        bool isHitRightTopSizingCorner(const D2D1_POINT_2F& p) const;
        bool isHitRightBottomSizingCorner(const D2D1_POINT_2F& p) const;

    protected:
        // IDrawObject2D
        void drawD2d1ObjectPosterior(renderer::Renderer* rndr) override;

        // Panel
        bool isHitHelper(const Event::Point& p) const override;

        void onChangeThemeHelper(WstrParam themeName) override;
        void onChangeThemeWrapper(WstrParam themeName);

        void onMouseMoveHelper(MouseMoveEvent& e) override;
        void onMouseMoveWrapper(MouseMoveEvent& e);

        void onMouseLeaveHelper(MouseMoveEvent& e) override;
        void onMouseLeaveWrapper(MouseMoveEvent& e);

        void onMouseButtonHelper(MouseButtonEvent& e) override;
        void onMouseButtonWrapper(MouseButtonEvent& e);
    };
}
