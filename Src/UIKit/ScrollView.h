#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/ScrollView.h"
#include "UIKit/MaskStyle.h"
#include "UIKit/ResizablePanel.h"
#include "UIKit/StrokeStyle.h"

namespace d14engine::uikit
{
    struct ScrollView : appearance::ScrollView, ResizablePanel
    {
        ScrollView(ShrdPtrParam<Panel> content = nullptr, const D2D1_RECT_F& rect = {});

        void onInitializeFinish() override;

        MaskStyle contentMask = {};

        _D14_SET_APPEARANCE_GETTER(ScrollView)

    public:
        void onStartThumbScrolling(const D2D1_POINT_2F& offset);

        Function<void(ScrollView*, const D2D1_POINT_2F&)> f_onStartThumbScrolling = {};

        void onEndThumbScrolling(const D2D1_POINT_2F& offset);

        Function<void(ScrollView*, const D2D1_POINT_2F&)> f_onEndThumbScrolling = {};

        void onViewportOffsetChange(const D2D1_POINT_2F& offset);

        Function<void(ScrollView*, const D2D1_POINT_2F&)> f_onViewportOffsetChange = {};

    protected:
        virtual void onStartThumbScrollingHelper(const D2D1_POINT_2F& offset);
        virtual void onEndThumbScrollingHelper(const D2D1_POINT_2F& offset);

        virtual void onViewportOffsetChangeHelper(const D2D1_POINT_2F& offset);

    public:
        struct CustomSizeGetter
        {
            using Func = Function<D2D1_SIZE_F(const ScrollView*)>;

            Func self = {};
            Func content = {};
        }
        customSizeGetter = {};

        struct DeltaPixelPerScroll
        {
            float horz = 30.0f;
            float vert = 30.0f;
        }
        deltaPixelsPerScroll = {};

    protected:
        D2D1_SIZE_F getSelfSize() const;
        D2D1_SIZE_F getContentSize() const;

    protected:
        // NO WeakPtr here as we provide a content-param in the ctor.
        SharedPtr<Panel> m_content = {};

    public:
        template<typename T = Panel>
        WeakPtr<T> content() const
        {
            if constexpr (std::is_same_v<T, Panel>) return m_content;
            else return std::dynamic_pointer_cast<T>(m_content);
        }
        void setContent(ShrdPtrParam<Panel> content);

    protected:
        D2D1_POINT_2F m_viewportOffset = { 0.0f, 0.0f };
        D2D1_POINT_2F m_originalViewportOffset = { 0.0f, 0.0f };

    protected:
        virtual D2D1_POINT_2F validateViewportOffset(const D2D1_POINT_2F& in);

    public:
        const D2D1_POINT_2F& viewportOffset() const;
        void setViewportOffset(const D2D1_POINT_2F& absolute);

        D2D1_POINT_2F viewportOffsetPercentage() const;
        void setViewportOffsetPercentage(const D2D1_POINT_2F& relative);

        void setViewportOffsetDirect(const D2D1_POINT_2F& offset);

    public:
        bool isHorzBarEnabled = true, isVertBarEnabled = true;

    protected:
        bool m_isHorzBarHover = false, m_isHorzBarDown = false;
        bool m_isVertBarHover = false, m_isVertBarDown = false;

    public:
        bool isControllingHorzBar() const;
        bool isControllingVertBar() const;
        bool isControllingScrollBars() const;

    protected:
        ScrollBarState getHorzBarState(bool isHover, bool isDown) const;
        ScrollBarState getVertBarState(bool isHover, bool isDown) const;

        D2D1_RECT_F horzBarSelfcoordRect(ScrollBarState state) const;
        D2D1_RECT_F vertBarSelfcoordRect(ScrollBarState state) const;

        float m_horzBarHoldOffset = 0.0f, m_vertBarHoldOffset = 0.0f;
        
    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

        void drawD2d1ObjectPosterior(renderer::Renderer* rndr) override;

        // Panel
        bool destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj) override;

        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;

        void onMouseWheelHelper(MouseWheelEvent& e) override;
    };
}
