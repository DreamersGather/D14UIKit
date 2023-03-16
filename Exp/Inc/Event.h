#pragma once

#include "Common.h"

#include "BasicType.h"
#include "VirtualKeyCode.h"

namespace d14uikit
{
    class DllExport Event : public _D14_UIKIT_NONCOPYABLE
    {
        _D14_UIKIT_PIMPL_DEF

        Event();

        static bool isKeyDown(VirtualKeyCode vkey);
        static bool isKeyToggled(VirtualKeyCode vkey);

        // Return whether the key is down.

        static bool lbutton();
        static bool rbutton();
        static bool mbutton();

        static bool alt();
        static bool lalt();
        static bool ralt();

        static bool ctrl();
        static bool lctrl();
        static bool rctrl();

        static bool shift();
        static bool lshift();
        static bool rshift();

        // Return whether the key is toggled.

        static bool capslock();

    protected:
        void initialize();
        explicit Event(Passkey);
    };

    class DllExport SizeEvent : public Event
    {
        _D14_UIKIT_PIMPL_DEF

        SizeEvent();

        Size size() const;

    protected:
        void initialize();
        explicit SizeEvent(Passkey);
    };

    class DllExport MoveEvent : public Event
    {
        _D14_UIKIT_PIMPL_DEF

        MoveEvent();

        Point position() const;

    protected:
        void initialize();
        explicit MoveEvent(Passkey);
    };

    class DllExport MouseEvent : public Event
    {
        _D14_UIKIT_PIMPL_DEF

        MouseEvent();

        Point cursorPoint() const;

    protected:
        void initialize();
        explicit MouseEvent(Passkey);
    };

    class DllExport MouseMoveEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL_DEF

        MouseMoveEvent();

        Point lastCursorPoint() const;

    protected:
        void initialize();
        explicit MouseMoveEvent(Passkey);
    };

    class DllExport MouseButtonEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL_DEF

        MouseButtonEvent();

        bool leftDown() const;
        bool leftUp() const;
        bool leftDblclk() const;
        bool rightDown() const;
        bool rightUp() const;
        bool rightDblclk() const;
        bool middleDown() const;
        bool middleUp() const;
        bool middleDblclk() const;

    protected:
        void initialize();
        explicit MouseButtonEvent(Passkey);
    };

    class DllExport MouseWheelEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL_DEF

        MouseWheelEvent();

        int deltaCount() const;

    protected:
        void initialize();
        explicit MouseWheelEvent(Passkey);
    };

    class DllExport KeyboardEvent : public Event
    {
        _D14_UIKIT_PIMPL_DEF

        KeyboardEvent();

        VirtualKeyCode vkey() const;

        bool pressed() const;
        bool released() const;

    protected:
        void initialize();
        explicit KeyboardEvent(Passkey);
    };

    class DllExport MouseButtonClickEvent : public Event
    {
        _D14_UIKIT_PIMPL_DEF

        MouseButtonClickEvent();

        bool left() const;
        bool right() const;
        bool middle() const;

    protected:
        void initialize();
        explicit MouseButtonClickEvent(Passkey);
    };
}
