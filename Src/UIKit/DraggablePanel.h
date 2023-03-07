#pragma once

#include "Common/Precompile.h"

#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct DraggablePanel : virtual Panel
    {
        DraggablePanel(
            const D2D1_RECT_F& rect = {},
            ComPtrParam<ID2D1Brush> brush = nullptr,
            ComPtrParam<ID2D1Bitmap1> bitmap = nullptr);

    public:
        void onStartDragging();

        Function<void(DraggablePanel*)> f_onStartDragging = {};

        void onEndDragging();

        Function<void(DraggablePanel*)> f_onEndDragging = {};

        bool isTriggerDragging(const Event::Point& p);

        Function<bool(DraggablePanel*, const Event::Point&)> f_isTriggerDragging = {};

    protected:
        virtual void onStartDraggingHelper();
        virtual void onEndDraggingHelper();

        virtual bool isTriggerDraggingHelper(const Event::Point& p);

    public:
        bool isDraggable = true;

        enum class OperationTarget
        {
            LocalSelfWindow, GlobalWin32Window
        }
        operationTarget = OperationTarget::LocalSelfWindow;

    protected:
        bool m_isDragging = false;

        struct DraggingPoint
        {
            D2D1_POINT_2F localSelfWindow = {}; POINT globalWin32Window = {};
        }
        m_draggingPoint = {};

    protected:
        // Panel
        void onMouseMoveHelper(MouseMoveEvent& e) override;
        void onMouseMoveWrapper(MouseMoveEvent& e);

        void onMouseButtonHelper(MouseButtonEvent& e) override;
        void onMouseButtonWrapper(MouseButtonEvent& e);
    };
}
