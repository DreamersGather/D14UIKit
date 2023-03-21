#pragma once

#include "Common/Precompile.h"

#include "UIKit/Panel.h"

namespace d14engine::uikit
{
    struct ClickablePanel : virtual Panel
    {
        using Panel::Panel;

        struct Event : MouseEvent
        {
            enum class Flag { Unknown, Left, Right, Middle } flag = Flag::Unknown;

            bool left() const { return flag == Flag::Left; }
            bool right() const { return flag == Flag::Right; }
            bool middle() const { return flag == Flag::Middle; }
        };

    public:
        void onMouseButtonPress(Event& e);

        Function<void(ClickablePanel*, Event&)> f_onMouseButtonPress = {};

        void onMouseButtonRelease(Event& e);

        Function<void(ClickablePanel*, Event&)> f_onMouseButtonRelease = {};

    protected:
        virtual void onMouseButtonPressHelper(Event& e);
        virtual void onMouseButtonReleaseHelper(Event& e);

    public:
        // Introduce this to help handle the specialized mouse-click callback.
        // 
        // Since the root Win32 window class of the application is created
        // with CS_DBLCLKS flag, UI objects can receive mouse-double-click
        // events in Panel::onMouseButton.
        // 
        // ClickablePanel::onMouseButtonPress is designed to only receive
        // mouse-down events (no mouse-double-click), so if enableDoubleClick
        // is true, ClickablePanel::onMouseButtonPress will not be triggered
        // when double clicking the mouse button, otherwise every native
        // mouse-double-click event will be translated into mouse-down before
        // delivering to ClickablePanel::onMouseButtonPress.

        bool enableDoubleClick = false;

    protected:
        // Since there is no capture mechanism in the UI event system, so the
        // following behavior can be triggered unexpectedly: A mouse button
        // is pressed and held outside the panel and then moved in, and if it
        // is released inside the panel at this point, a mouse-up event will
        // be generated, in which case UI object (e.g. button) may trigger a
        // callback unexpectedly if we simply put the code for mouse-release
        // into the mouse-up callback.
        // 
        // To solve this problem, we are determined to provide an callback to
        // check the real mouse-click event, which will be triggered when a
        // native mouse-up event is encountered and m_hasXxxPressed is true.

        bool m_hasLeftPressed = false, m_hasRightPressed = false, m_hasMiddlePressed = false;

    protected:
        // Panel
        void onMouseLeaveHelper(MouseMoveEvent& e) override;
        void onMouseLeaveWrapper(MouseMoveEvent& e);

        void onMouseButtonHelper(MouseButtonEvent& e) override;
        void onMouseButtonWrapper(MouseButtonEvent& e);

        // Introduce onXxxWrapper to avoid duplicated calls of the virtual
        // methods in the multiple inheritance.
        // 
        // For example, suppose we define a virtual method called "func()":
        // 
        // A { virtual func() { A's work... } }
        // B1 -> A { func() override { A::func(); B1's work... } }
        // B2 -> A { func() override { A::func(); B2's work... } }
        // C -> B1, B2 { func() override { ???::func(); C's work... } }
        // 
        // If we call B1::func() and B2::func() in C::func(), then A::func()
        // will be called twice, which is obviously not the expected behavior.
        // 
        // Introduce an intermediate method to easily solve this problem:
        // 
        // B1 -> A { ... funcWrapper() { B1's work... } }
        // B2 -> A { ... funcWrapper() { B2's work... } }
        // C -> B1, B2 { func() override { A::func(); B1::funcWrapper(); B2::funcWrapper(); C's work... } }

    public:
        void setEnabled(bool value) override;
        void setEnabledWrapper(bool value);
    };
}
