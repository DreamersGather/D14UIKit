#include "Common/Precompile.h"

#include "Event.h"

#include "Inc/Event.h"

#define DEF_PROPERTY_READONLY(Class_Name, Prop_Name) \
i.def_property_readonly(#Prop_Name, &Class_Name::Prop_Name)

#define DEF_PROPERTY_READONLY_STATIC(Class_Name, Prop_Name) \
i.def_property_readonly_static(#Prop_Name, [](py::object) { return Class_Name::Prop_Name(); })

namespace d14uikit
{
    void Py_InitEvent(py::module_& m)
    {
        // Event
        {
            py::class_<Event> i(m, "Event");

            i.def(py::init());

            i.def_static( "isKeyDown", &Event::isKeyDown, "vkey"_a);
            i.def_static("isKeyToggled", &Event::isKeyToggled, "vkey"_a);

            DEF_PROPERTY_READONLY_STATIC(Event, lbutton);
            DEF_PROPERTY_READONLY_STATIC(Event, rbutton);
            DEF_PROPERTY_READONLY_STATIC(Event, mbutton);

            DEF_PROPERTY_READONLY_STATIC(Event, alt);
            DEF_PROPERTY_READONLY_STATIC(Event, lalt);
            DEF_PROPERTY_READONLY_STATIC(Event, ralt);


            DEF_PROPERTY_READONLY_STATIC(Event, ctrl);
            DEF_PROPERTY_READONLY_STATIC(Event, lctrl);
            DEF_PROPERTY_READONLY_STATIC(Event, rctrl);

            DEF_PROPERTY_READONLY_STATIC(Event, shift);
            DEF_PROPERTY_READONLY_STATIC(Event, lshift);
            DEF_PROPERTY_READONLY_STATIC(Event, rshift);

            DEF_PROPERTY_READONLY_STATIC(Event, capslock);
        }
        // SizeEvent
        {
            py::class_<SizeEvent, Event> i(m, "SizeEvent");

            i.def(py::init());

            DEF_PROPERTY_READONLY(SizeEvent, size);
        }
        // MoveEvent
        {
            py::class_<MoveEvent, Event> i(m, "MoveEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(MoveEvent, position);
        }
        // MouseEvent
        {
            py::class_<MouseEvent, Event> i(m, "MouseEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(MouseEvent, cursorPoint);
        }
        // KeyboardEvent
        {
            py::class_<KeyboardEvent, Event> i(m, "KeyboardEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(KeyboardEvent, vkey);
            DEF_PROPERTY_READONLY(KeyboardEvent, pressed);
            DEF_PROPERTY_READONLY(KeyboardEvent, released);
        }
        // MouseMoveEvent
        {
            py::class_<MouseMoveEvent, MouseEvent> i(m, "MouseMoveEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(MouseMoveEvent, lastCursorPoint);
        }
        // MouseWheelEvent
        {
            py::class_<MouseWheelEvent, MouseEvent> i(m, "MouseWheelEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(MouseWheelEvent, deltaCount);
        }
        // MouseButtonEvent
        {
            py::class_<MouseButtonEvent, MouseEvent> i(m, "MouseButtonEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(MouseButtonEvent, leftDown);
            DEF_PROPERTY_READONLY(MouseButtonEvent, leftUp);
            DEF_PROPERTY_READONLY(MouseButtonEvent, leftDblclk);
            DEF_PROPERTY_READONLY(MouseButtonEvent, rightDown);
            DEF_PROPERTY_READONLY(MouseButtonEvent, rightUp);
            DEF_PROPERTY_READONLY(MouseButtonEvent, rightDblclk);
            DEF_PROPERTY_READONLY(MouseButtonEvent, middleDown);
            DEF_PROPERTY_READONLY(MouseButtonEvent, middleUp);
            DEF_PROPERTY_READONLY(MouseButtonEvent, middleDblclk);
        }
        // MouseButtonClickEvent
        {
            py::class_<MouseButtonClickEvent, MouseEvent> i(m, "MouseButtonClickEvent");
        
            i.def(py::init());

            DEF_PROPERTY_READONLY(MouseButtonClickEvent, left);
            DEF_PROPERTY_READONLY(MouseButtonClickEvent, right);
            DEF_PROPERTY_READONLY(MouseButtonClickEvent, middle);
        }
    }
}

#undef DEF_PROPERTY_READONLY
#undef DEF_PROPERTY_READONLY_STATIC
