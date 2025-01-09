#include "Common/Precompile.h"

#include "Event.h"

#include "Common.h"

#include "Common/MathUtils/Basic.h"

using namespace d14engine;

namespace d14uikit
{
    Event::Event()
        :
        Event(std::make_shared<uikit::Event>()) { }

    _D14_UIKIT_CTOR(Event)
        :
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
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

    SizeEvent::SizeEvent()
        :
        SizeEvent(std::make_shared<uikit::SizeEvent>()) { }

    _D14_UIKIT_CTOR(SizeEvent)
        :
        Event(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    Size SizeEvent::size() const
    {
        return convert(pimpl->uiobj->size);
    }

    MoveEvent::MoveEvent()
        :
        MoveEvent(std::make_shared<uikit::MoveEvent>()) { }

    _D14_UIKIT_CTOR(MoveEvent)
        :
        Event(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    Point MoveEvent::position() const
    {
        return convert(pimpl->uiobj->position);
    }

    MouseEvent::MouseEvent()
        :
        MouseEvent(std::make_shared<uikit::MouseEvent>()) { }

    _D14_UIKIT_CTOR(MouseEvent)
        :
        Event(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    Point MouseEvent::cursorPoint() const
    {
        return convert(pimpl->uiobj->cursorPoint);
    }

    MouseMoveEvent::MouseMoveEvent()
        :
        MouseMoveEvent(std::make_shared<uikit::MouseMoveEvent>()) { }

    _D14_UIKIT_CTOR(MouseMoveEvent)
        :
        MouseEvent(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    Point MouseMoveEvent::lastCursorPoint() const
    {
        return convert(pimpl->uiobj->lastCursorPoint);
    }

    MouseButtonEvent::MouseButtonEvent()
        :
        MouseButtonEvent(std::make_shared<uikit::MouseButtonEvent>()) { }

    _D14_UIKIT_CTOR(MouseButtonEvent)
        :
        MouseEvent(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
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

    MouseWheelEvent::MouseWheelEvent()
        :
        MouseWheelEvent(std::make_shared<uikit::MouseWheelEvent>()) { }

    _D14_UIKIT_CTOR(MouseWheelEvent)
        :
        MouseEvent(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

    int MouseWheelEvent::deltaCount() const
    {
        return pimpl->uiobj->deltaCount;
    }

    KeyboardEvent::KeyboardEvent()
        :
        KeyboardEvent(std::make_shared<uikit::KeyboardEvent>()) { }

    _D14_UIKIT_CTOR(KeyboardEvent)
        :
        Event(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
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

    MouseButtonClickEvent::MouseButtonClickEvent()
        :
        MouseButtonClickEvent(std::make_shared<uikit::MouseButtonClickEvent>()) { }

    _D14_UIKIT_CTOR(MouseButtonClickEvent)
        :
        MouseEvent(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
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
}
