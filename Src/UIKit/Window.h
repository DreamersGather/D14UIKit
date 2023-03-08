#pragma once

#include "Common/Precompile.h"

#include "UIKit/Appearances/Window.h"
#include "UIKit/DraggablePanel.h"
#include "UIKit/MaskStyle.h"
#include "UIKit/ResizablePanel.h"
#include "UIKit/ShadowStyle.h"

namespace d14engine::uikit
{
    struct IconLabel;
    struct TabGroup;

    struct Window : appearance::Window, DraggablePanel, ResizablePanel
    {
        Window(
            ShrdPtrParam<IconLabel> caption,
            const D2D1_RECT_F& rect = {},
            float captionPanelHeight = 32.0f,
            float decorativeBarHeight = 4.0f);

        Window(
            WstrParam title = L"Untitled",
            const D2D1_RECT_F& rect = {},
            float captionPanelHeight = 32.0f,
            float decorativeBarHeight = 4.0f);

        enum class DisplayState { Normal, Minimized, Maximized };

        void onInitializeFinish() override;

        MaskStyle mask = {};
        ShadowStyle shadow = {};

        ComPtr<ID2D1LinearGradientBrush> decorativeBarBrush = {};

        void loadDecorativeBarBrush();

        _D14_SET_APPEARANCE_GETTER(Window)

    public:
        void onMinimize();

        Function<void(Window*)> f_onMinimize = {};

        void onMaximize();

        Function<void(Window*)> f_onMaximize = {};

        void onRestore();

        Function<void(Window*)> f_onRestore = {};

        void onClose();

        Function<void(Window*)> f_onClose = {};

    protected:
        virtual void onMinimizeHelper();
        virtual void onMaximizeHelper();
        virtual void onRestoreHelper();
        virtual void onCloseHelper();

    protected:
        SharedPtr<IconLabel> m_caption = {};

        SharedPtr<Panel> m_centerUIObject = {};

    public:
        const SharedPtr<IconLabel>& caption() const;
        void setCaption(ShrdPtrParam<IconLabel> caption);

        template<typename T = Panel>
        WeakPtr<T> centerUIObject() const
        {
            if constexpr (std::is_same_v<T, Panel>) return m_centerUIObject;
            else return std::dynamic_pointer_cast<T>(m_centerUIObject);
        }
        void setCenterUIObject(ShrdPtrParam<Panel> uiobj);

    protected:
        float m_captionPanelHeight = {};
        float m_decorativeBarHeight = {};

        D2D1_RECT_F captionPanelAbsoluteRect() const;
        D2D1_RECT_F decorativeBarAbsoluteRect() const;

        constexpr static float minMaxButtonWidth() { return 32.0f; }
        constexpr static float closeButtonWidth() { return 40.0f; }

        constexpr static float threeBrosHeight() { return 24.0f; }

        constexpr static float threeBrosLeftmostOffset();
        constexpr static float threeBrosRightmostOffset() { return 20.0f; }

        constexpr static D2D1_RECT_F minimizeIconVertexOffset()
        {
            return { 10.0f, 14.0f, -10.0f, -8.0f };
        }
        constexpr static D2D1_RECT_F restoreIconVertexOffset()
        {
            return { 11.0f, 10.0f, -14.0f, -7.0f };
        }
        constexpr static D2D1_RECT_F maximizeIconVertexOffset()
        {
            return { 11.0f, 7.0f, -11.0f, -7.0f };
        }
        constexpr static D2D1_RECT_F closeIconVertexOffset()
        {
            return { 15.0f, 7.0f, -15.0f, -7.0f };
        }
        constexpr static float restoreIconStrokeWidth() { return 2.0f; }
        constexpr static float restoreOrnamentOffset() { return 3.0f; }
        constexpr static float maximizeIconStrokeWidth() { return 2.0f; }
        constexpr static float closeIconStrokeWidth() { return 2.0f; }

        D2D1_RECT_F minimizeIconAbsoluteRect() const;
        D2D1_RECT_F minimizeButtonAbsoluteRect() const;
        D2D1_RECT_F restoreIconAbsoluteRect() const;
        D2D1_RECT_F maximizeIconAbsoluteRect() const;
        D2D1_RECT_F maximizeButtonAbsoluteRect() const;
        D2D1_RECT_F closeIconAbsoluteRect() const;
        D2D1_RECT_F closeButtonAbsoluteRect() const;
        
        D2D1_RECT_F captionIconLabelSelfcoordRect() const;

    public:
        float captionPanelHeight() const;
        void setCaptionPanelHeight(float value);

        float decorativeBarHeight() const;
        void setDecorativeBarHeight(float value);

        float clientAreaHeight() const;
        D2D1_RECT_F clientAreaSelfcoordRect() const;

        float nonClientAreaHeight() const;
        D2D1_RECT_F nonClientAreaSelfcoordRect() const;
        D2D1_RECT_F nonClientAreaMinimalSelfcoordRect() const;

        constexpr static float nonClientAreaMinimalWidth() { return 144.0f; }
        constexpr static float nonClientAreaDefaultHeight() { return 40.0f; }

    protected:
        DisplayState m_displayState = DisplayState::Normal;

    public:
        DisplayState currDisplayState() const;

        bool isDisplayNormal() const;
        bool isDisplayMinimized() const;
        bool isDisplayMaximized() const;

    public:
        bool isMinimizeEnabled = true, isMaximizeEnabled = true, isCloseEnabled = true;

    protected:
        // Indicates whether a special operation is being performed on the
        // window (e.g. resizing, dragging).  When this is true, 3-bros
        // should not respond UI events even if they are enabled indeed.
        bool m_isPerformSpecialOperation = false;

        bool m_isMinimizeHover = false, m_isMinimizeDown = false;
        bool m_isMaximizeHover = false, m_isMaximizeDown = false;
        bool m_isCloseHover = false, m_isCloseDown = false;

        ThreeBrosState getMinMaxBroState(bool isHover, bool isDown) const;
        ThreeBrosState getCloseXBroState(bool isHover, bool isDown) const;

        void set3BrothersIconBrushState(ThreeBrosState state);
        void set3BrothersButtonBrushState(ThreeBrosState state);

    public:
        // Whether to move to the topmost when receiving mouse-button event.
        bool respondSetForegroundEvent = true;

    protected:
        using RegisteredTabGroupSet = std::set<WeakPtr<TabGroup>, std::owner_less<WeakPtr<TabGroup>>>;

        // Only the registered tab-groups can be associated with the window.
        RegisteredTabGroupSet m_registeredTabGroups = {};

    public:
        void registerTabGroup(WeakPtrParam<TabGroup> tg);
        void unregisterTabGroup(WeakPtrParam<TabGroup> tg);

    public:
        Function<void(Window*, TabGroup*)> f_onTriggerTabDemoting = {};

        // When a window is being dragged, all of the tab-groups that have
        // been registered for the window will be associated with it, and if
        // the window is then released above any associated tab-group, its
        // caption and content will be demoted and inserted as a new tab and
        // the original window will be destroyed later.

        WeakPtr<TabGroup> associatedTabGroup = {};

    protected:
        void handleMouseMoveForRegisteredTabGroups(MouseMoveEvent& e);
        void handleMouseButtonForRegisteredTabGroups(MouseButtonEvent& e);

    protected:
        // IDrawObject2D
        void onRendererDrawD2d1LayerHelper(renderer::Renderer* rndr) override;

        void onRendererDrawD2d1ObjectHelper(renderer::Renderer* rndr) override;

    public:
        // Panel
        float minimalWidth() const override;

        float minimalHeight() const override;

    protected:
        bool destroyUIObjectHelper(ShrdPtrParam<Panel> uiobj) override;

        void onSizeHelper(SizeEvent& e) override;

        void onChangeThemeHelper(WstrParam themeName) override;

        void onMouseMoveHelper(MouseMoveEvent& e) override;

        void onMouseLeaveHelper(MouseMoveEvent& e) override;

        void onMouseButtonHelper(MouseButtonEvent& e) override;

        // DraggablePanel
        bool isTriggerDraggingHelper(const Event::Point& p) override;

        void onStartDraggingHelper() override;

        void onEndDraggingHelper() override;

        // ResizablePanel
        void onStartResizingHelper() override;

        void onEndResizingHelper() override;
    };
}
