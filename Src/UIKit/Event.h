#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    struct Panel;

    struct Event
    {
        using Win32Message = UINT;

        using Size = D2D1_SIZE_F;
        using Point = D2D1_POINT_2F;
    };

    struct SizeEvent : Event
    {
        Size size = {};
    };

    struct MoveEvent : Event
    {
        Point position = {};
    };

    struct MouseEvent : Event
    {
        using CursorPoint = Point;

        CursorPoint cursorPoint = {};
    };

    struct MouseMoveEvent : MouseEvent
    {
        struct ButtonState
        {
            bool leftPressed = false;
            bool rightPressed = false;
            bool middlePressed = false;
        }
        buttonState = {};

        struct KeyState
        {
            bool ALT = false;
            bool CTRL = false;
            bool SHIFT = false;
        }
        keyState = {};

        CursorPoint lastCursorPoint = {};
    };

    struct MouseButtonEvent : MouseEvent
    {
        struct State
        {
            enum class Flag
            {
                Unknown,
                LeftDown,
                LeftUp,
                LeftDblclk,
                RightDown,
                RightUp,
                RightDblclk,
                MiddleDown,
                MiddleUp,
                MiddleDblclk
            }
            flag = Flag::Unknown;

            using FlagMap = std::unordered_map<Win32Message, Flag>;

            const static FlagMap FLAG_MAP;

            bool leftDown() const;
            bool leftUp() const;
            bool leftDblclk() const;
            bool rightDown() const;
            bool rightUp() const;
            bool rightDblclk() const;
            bool middleDown() const;
            bool middleUp() const;
            bool middleDblclk() const;
        }
        state = {};

        WeakPtr<Panel> focused = {};
    };

    struct MouseWheelEvent : MouseEvent
    {
        struct ButtonState
        {
            bool leftPressed = false;
            bool rightPressed = false;
            bool middlePressed = false;
        }
        buttonState = {};

        struct KeyState
        {
            bool CTRL = false;
            bool SHIFT = false;
        }
        keyState = {};

        int deltaCount = 0;
    };

    struct KeyboardEvent : Event
    {
        struct State
        {
            enum class Flag
            {
                Unknown,
                Pressed,
                Released,
            }
            flag = Flag::Unknown;

            bool pressed() const;
            bool released() const;
        }
        state = {};

        WPARAM vkey = VK_NONAME;

        static bool isKeyDown(int vkey);
        static bool isKeyToggled(int vkey);

        // Return whether the key is down.

        static bool LALT();
        static bool RALT();
        static bool ALT();

        static bool LCTRL();
        static bool RCTRL();
        static bool CTRL();

        static bool LSHIFT();
        static bool RSHIFT();
        static bool SHIFT();

        // Return whether the key is toggled.

        static bool CAPSLOCK();
    };
}
