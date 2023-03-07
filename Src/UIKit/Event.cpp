#pragma once

#include "Common/Precompile.h"

#include "UIKit/Event.h"

namespace d14engine::uikit
{

#define MBE_STATE MouseButtonEvent::State

    const MBE_STATE::FlagMap MBE_STATE::FLAG_MAP =
    {
        { WM_LBUTTONDOWN,   MBE_STATE::Flag::LeftDown },
        { WM_LBUTTONUP,     MBE_STATE::Flag::LeftUp },
        { WM_LBUTTONDBLCLK, MBE_STATE::Flag::LeftDblclk },
        { WM_RBUTTONDOWN,   MBE_STATE::Flag::RightDown },
        { WM_RBUTTONUP,     MBE_STATE::Flag::RightUp },
        { WM_RBUTTONDBLCLK, MBE_STATE::Flag::RightDblclk },
        { WM_MBUTTONDOWN,   MBE_STATE::Flag::MiddleDown },
        { WM_MBUTTONUP,     MBE_STATE::Flag::MiddleUp },
        { WM_MBUTTONDBLCLK, MBE_STATE::Flag::MiddleDblclk }
    };

#undef MBE_STATE

    bool MouseButtonEvent::State::leftDown() const
    {
        return flag == Flag::LeftDown;
    }

    bool MouseButtonEvent::State::leftUp() const
    {
        return flag == Flag::LeftUp;
    }

    bool MouseButtonEvent::State::leftDblclk() const
    {
        return flag == Flag::LeftDblclk;
    }

    bool MouseButtonEvent::State::rightDown() const
    {
        return flag == Flag::RightDown;
    }

    bool MouseButtonEvent::State::rightUp() const
    {
        return flag == Flag::RightUp;
    }

    bool MouseButtonEvent::State::rightDblclk() const
    {
        return flag == Flag::RightDblclk;
    }

    bool MouseButtonEvent::State::middleDown() const
    {
        return flag == Flag::MiddleDown;
    }

    bool MouseButtonEvent::State::middleUp() const
    {
        return flag == Flag::MiddleUp;
    }

    bool MouseButtonEvent::State::middleDblclk() const
    {
        return flag == Flag::MiddleDblclk;
    }

    bool KeyboardEvent::State::pressed() const
    {
        return flag == Flag::Pressed;
    }

    bool KeyboardEvent::State::released() const
    {
        return flag == Flag::Released;
    }

    bool KeyboardEvent::isKeyDown(int vkey)
    {
        return GetKeyState(vkey) & 0x8000;
    }

    bool KeyboardEvent::isKeyToggled(int vkey)
    {
        return GetKeyState(vkey) & 0x0001;
    }

    bool KeyboardEvent::LALT()
    {
        return isKeyDown(VK_LMENU);
    }

    bool KeyboardEvent::RALT()
    {
        return isKeyDown(VK_RMENU);
    }

    bool KeyboardEvent::ALT()
    {
        return LALT() || RALT();
    }

    bool KeyboardEvent::LCTRL()
    {
        return isKeyDown(VK_LCONTROL);
    }

    bool KeyboardEvent::RCTRL()
    {
        return isKeyDown(VK_RCONTROL);
    }

    bool KeyboardEvent::CTRL()
    {
        return LCTRL() || RCTRL();
    }

    bool KeyboardEvent::LSHIFT()
    {
        return isKeyDown(VK_LSHIFT);
    }

    bool KeyboardEvent::RSHIFT()
    {
        return isKeyDown(VK_RSHIFT);
    }

    bool KeyboardEvent::SHIFT()
    {
        return LSHIFT() || RSHIFT();
    }

    bool KeyboardEvent::CAPSLOCK()
    {
        return isKeyToggled(VK_CAPITAL);
    }
}
