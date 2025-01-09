#pragma once

#include "Common.h"

#include "BasicType.h"
#include "VirtualKeyCode.h"

_D14_UIKIT_FWDEF(Event)
_D14_UIKIT_FWDEF(SizeEvent)
_D14_UIKIT_FWDEF(MoveEvent)
_D14_UIKIT_FWDEF(MouseEvent)
_D14_UIKIT_FWDEF(MouseMoveEvent)
_D14_UIKIT_FWDEF(MouseButtonEvent)
_D14_UIKIT_FWDEF(MouseWheelEvent)
_D14_UIKIT_FWDEF(KeyboardEvent)
_D14_UIKIT_FWDEF(MouseButtonClickEvent)

namespace d14uikit
{
    class DllExport Event : public NonCopyable
    {
        _D14_UIKIT_PIMPL(Event)

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
    };

    class DllExport SizeEvent : public Event
    {
        _D14_UIKIT_PIMPL(SizeEvent)

        SizeEvent();

        Size size() const;
    };

    class DllExport MoveEvent : public Event
    {
        _D14_UIKIT_PIMPL(MoveEvent)

        MoveEvent();

        Point position() const;
    };

    class DllExport MouseEvent : public Event
    {
        _D14_UIKIT_PIMPL(MouseEvent)

        MouseEvent();

        Point cursorPoint() const;
    };

    class DllExport MouseMoveEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL(MouseMoveEvent)

        MouseMoveEvent();

        Point lastCursorPoint() const;
    };

    class DllExport MouseButtonEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL(MouseButtonEvent)

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
    };

    class DllExport MouseWheelEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL(MouseWheelEvent)

        MouseWheelEvent();

        int deltaCount() const;
    };

    class DllExport KeyboardEvent : public Event
    {
        _D14_UIKIT_PIMPL(KeyboardEvent)

        KeyboardEvent();

        VirtualKeyCode vkey() const;

        bool pressed() const;
        bool released() const;
    };

    class DllExport MouseButtonClickEvent : public MouseEvent
    {
        _D14_UIKIT_PIMPL(MouseButtonClickEvent)

        MouseButtonClickEvent();

        bool left() const;
        bool right() const;
        bool middle() const;
    };
}
