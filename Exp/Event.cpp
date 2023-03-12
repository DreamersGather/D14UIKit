#include "Common/Precompile.h"

#include "Event.h"

#include "Common/MathUtils/Basic.h"

using namespace d14engine;

namespace d14uikit
{
    Event::Event() : Event(Passkey{})
    {
        pimpl->uiobj = std::make_shared<uikit::Event>();

        initialize();
    }

    bool Event::isKeyDown(VirtualKeyCode vkey)
    {
        return uikit::Event::isKeyDown(vkey);
    }

    bool Event::isKeyToggled(VirtualKeyCode vkey)
    {
        return uikit::Event::isKeyToggled(vkey);
    }

    bool Event::lbutton() { return isKeyDown(vk_lbutton); }

    bool Event::rbutton() { return isKeyDown(vk_rbutton); }

    bool Event::mbutton() { return isKeyDown(vk_mbutton); }

    bool Event::alt() { return isKeyDown(vk_menu); }

    bool Event::lalt() { return isKeyDown(vk_lmenu); }

    bool Event::ralt() { return isKeyDown(vk_rmenu); }

    bool Event::ctrl() { return isKeyDown(vk_control); }

    bool Event::lctrl() { return isKeyDown(vk_lcontrol); }

    bool Event::rctrl() { return isKeyDown(vk_rcontrol); }

    bool Event::shift() { return isKeyDown(vk_shift); }

    bool Event::lshift() { return isKeyDown(vk_lshift); }

    bool Event::rshift() { return isKeyDown(vk_rshift); }

    bool Event::capslock() { return isKeyToggled(vk_capital); }

    Event::Event(Passkey) : pimpl(std::make_shared<Impl>()) { }

    void Event::initialize() { }

    SizeEvent::SizeEvent() : SizeEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        SizeEvent::pimpl->uiobj =
        std::make_shared<uikit::SizeEvent>();

        Event::initialize();
        SizeEvent::initialize();
    }

    Size SizeEvent::size() const
    {
        auto& sz = pimpl->uiobj->size;
        return { math_utils::round(sz.width), math_utils::round(sz.height) };
    }

    SizeEvent::SizeEvent(Passkey)
        :
        Event(Event::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void SizeEvent::initialize() { }

    MoveEvent::MoveEvent() : MoveEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        MoveEvent::pimpl->uiobj =
        std::make_shared<uikit::MoveEvent>();

        Event::initialize();
        MoveEvent::initialize();
    }

    Point MoveEvent::position() const
    {
        auto& pt = pimpl->uiobj->position;
        return { math_utils::round(pt.x), math_utils::round(pt.y) };
    }

    MoveEvent::MoveEvent(Passkey)
        :
        Event(Event::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MoveEvent::initialize() { }

    MouseEvent::MouseEvent() : MouseEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        MouseEvent::pimpl->uiobj =
        std::make_shared<uikit::MouseEvent>();

        Event::initialize();
        MouseEvent::initialize();
    }

    Point MouseEvent::cursorPoint() const
    {
        auto& pt = pimpl->uiobj->cursorPoint;
        return { math_utils::round(pt.x), math_utils::round(pt.y) };
    }

    MouseEvent::MouseEvent(Passkey)
        :
        Event(Event::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MouseEvent::initialize() { }

    MouseMoveEvent::MouseMoveEvent() : MouseMoveEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        MouseEvent::pimpl->uiobj =
        MouseMoveEvent::pimpl->uiobj =
        std::make_shared<uikit::MouseMoveEvent>();

        Event::initialize();
        MouseEvent::initialize();
        MouseMoveEvent::initialize();
    }

    Point MouseMoveEvent::lastCursorPoint() const
    {
        auto& pt = pimpl->uiobj->lastCursorPoint;
        return { math_utils::round(pt.x), math_utils::round(pt.y) };
    }

    MouseMoveEvent::MouseMoveEvent(Passkey)
        :
        MouseEvent(MouseEvent::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MouseMoveEvent::initialize() { }

    MouseButtonEvent::MouseButtonEvent() : MouseButtonEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        MouseEvent::pimpl->uiobj =
        MouseButtonEvent::pimpl->uiobj =
        std::make_shared<uikit::MouseButtonEvent>();

        Event::initialize();
        MouseEvent::initialize();
        MouseButtonEvent::initialize();
    }

    bool MouseButtonEvent::leftDown() const
    {
        return pimpl->uiobj->state.leftDown();
    }

    bool MouseButtonEvent::leftUp() const
    {
        return pimpl->uiobj->state.leftUp();
    }

    bool MouseButtonEvent::leftDblclk() const
    {
        return pimpl->uiobj->state.leftDblclk();
    }

    bool MouseButtonEvent::rightDown() const
    {
        return pimpl->uiobj->state.rightDown();
    }

    bool MouseButtonEvent::rightUp() const
    {
        return pimpl->uiobj->state.rightUp();
    }

    bool MouseButtonEvent::rightDblclk() const
    {
        return pimpl->uiobj->state.rightDblclk();
    }

    bool MouseButtonEvent::middleDown() const
    {
        return pimpl->uiobj->state.middleDown();
    }

    bool MouseButtonEvent::middleUp() const
    {
        return pimpl->uiobj->state.middleUp();
    }

    bool MouseButtonEvent::middleDblclk() const
    {
        return pimpl->uiobj->state.middleDblclk();
    }

    MouseButtonEvent::MouseButtonEvent(Passkey)
        :
        MouseEvent(MouseEvent::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MouseButtonEvent::initialize() { }

    MouseWheelEvent::MouseWheelEvent() : MouseWheelEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        MouseEvent::pimpl->uiobj =
        MouseWheelEvent::pimpl->uiobj =
        std::make_shared<uikit::MouseWheelEvent>();

        Event::initialize();
        MouseEvent::initialize();
        MouseWheelEvent::initialize();
    }

    int MouseWheelEvent::deltaCount() const
    {
        return pimpl->uiobj->deltaCount;
    }

    MouseWheelEvent::MouseWheelEvent(Passkey)
        :
        MouseEvent(MouseEvent::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MouseWheelEvent::initialize() { }

    KeyboardEvent::KeyboardEvent() : KeyboardEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        KeyboardEvent::pimpl->uiobj =
        std::make_shared<uikit::KeyboardEvent>();

        Event::initialize();
        KeyboardEvent::initialize();
    }

    VirtualKeyCode KeyboardEvent::vkey() const
    {
        return (VirtualKeyCode)pimpl->uiobj->vkey;
    }

    bool KeyboardEvent::pressed() const
    {
        return pimpl->uiobj->state.pressed();
    }

    bool KeyboardEvent::released() const
    {
        return pimpl->uiobj->state.released();
    }

    KeyboardEvent::KeyboardEvent(Passkey)
        :
        Event(Event::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void KeyboardEvent::initialize() { }

    MouseButtonClickEvent::MouseButtonClickEvent() : MouseButtonClickEvent(Passkey{})
    {
        Event::pimpl->uiobj =
        MouseButtonClickEvent::pimpl->uiobj =
        std::make_shared<uikit::ClickablePanel::Event>();

        Event::initialize();
        MouseButtonClickEvent::initialize();
    }

    bool MouseButtonClickEvent::left() const
    {
        return pimpl->uiobj->left();
    }

    bool MouseButtonClickEvent::right() const
    {
        return pimpl->uiobj->right();
    }

    bool MouseButtonClickEvent::middle() const
    {
        return pimpl->uiobj->middle();
    }

    MouseButtonClickEvent::MouseButtonClickEvent(Passkey)
        :
        Event(Event::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void MouseButtonClickEvent::initialize() { }
}
