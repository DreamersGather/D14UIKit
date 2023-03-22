#include "Common/Precompile.h"

#include "Event.h"

#include "Inc/Event.h"

namespace d14uikit
{
    void Py_InitEvent(py::module_& m)
    {
        // Event
        {
            py::class_<Event> i(m, "Event");

            i.def(py::init<>());

            i.def_static(
                "isKeyDown",
                &Event::isKeyDown,
                "vkey"_a);

            i.def_static(
                "isKeyToggled",
                &Event::isKeyToggled,
                "vkey"_a);

            i.def_static(
                "lbutton",
                &Event::lbutton);

            i.def_static(
                "rbutton",
                &Event::rbutton);

            i.def_static(
                "mbutton",
                &Event::mbutton);

            i.def_static(
                "alt",
                &Event::alt);

            i.def_static(
                "lalt",
                &Event::lalt);

            i.def_static(
                "ralt",
                &Event::ralt);

            i.def_static(
                "ctrl",
                &Event::ctrl);

            i.def_static(
                "lctrl",
                &Event::lctrl);

            i.def_static(
                "rctrl",
                &Event::rctrl);

            i.def_static(
                "shift",
                &Event::shift);

            i.def_static(
                "lshift",
                &Event::lshift);

            i.def_static(
                "rshift",
                &Event::rshift);

            i.def_static(
                "capslock",
                &Event::capslock);
        }
        // SizeEvent
        {
            py::class_<SizeEvent, Event> i(m, "SizeEvent");

            i.def(py::init<>());

            i.def_property_readonly(
                "size",
                &SizeEvent::size);
        }
        // MoveEvent
        {
            py::class_<MoveEvent, Event> i(m, "MoveEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "position",
                &MoveEvent::position);
        }
        // MouseEvent
        {
            py::class_<MouseEvent, Event> i(m, "MouseEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "cursorPoint",
                &MouseEvent::cursorPoint);
        }
        // KeyboardEvent
        {
            py::class_<KeyboardEvent, Event> i(m, "KeyboardEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "vkey",
                &KeyboardEvent::vkey);

            i.def_property_readonly(
                "pressed",
                &KeyboardEvent::pressed);

            i.def_property_readonly(
                "released",
                &KeyboardEvent::released);
        }
        // MouseMoveEvent
        {
            py::class_<MouseMoveEvent, MouseEvent> i(m, "MouseMoveEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "lastCursorPoint",
                &MouseMoveEvent::lastCursorPoint);
        }
        // MouseWheelEvent
        {
            py::class_<MouseWheelEvent, MouseEvent> i(m, "MouseWheelEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "deltaCount",
                &MouseWheelEvent::deltaCount);
        }
        // MouseButtonEvent
        {
            py::class_<MouseButtonEvent, MouseEvent> i(m, "MouseButtonEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "leftDown",
                &MouseButtonEvent::leftDown);

            i.def_property_readonly(
                "leftUp",
                &MouseButtonEvent::leftUp);

            i.def_property_readonly(
                "leftDblclk",
                &MouseButtonEvent::leftDblclk);

            i.def_property_readonly(
                "rightDown",
                &MouseButtonEvent::rightDown);

            i.def_property_readonly(
                "rightUp",
                &MouseButtonEvent::rightUp);

            i.def_property_readonly(
                "rightDblclk",
                &MouseButtonEvent::rightDblclk);

            i.def_property_readonly(
                "middleDown",
                &MouseButtonEvent::middleDown);

            i.def_property_readonly(
                "middleUp",
                &MouseButtonEvent::middleUp);

            i.def_property_readonly(
                "middleDblclk",
                &MouseButtonEvent::middleDblclk);
        }
        // MouseButtonClickEvent
        {
            py::class_<MouseButtonClickEvent, MouseEvent> i(m, "MouseButtonClickEvent");
        
            i.def(py::init<>());

            i.def_property_readonly(
                "left",
                &MouseButtonClickEvent::left);

            i.def_property_readonly(
                "right",
                &MouseButtonClickEvent::right);

            i.def_property_readonly(
                "middle",
                &MouseButtonClickEvent::middle);
        }
    }
}
