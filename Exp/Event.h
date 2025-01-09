#pragma once

#include "Common/Precompile.h"

#include "Inc/Event.h"

#include "UIKit/ClickablePanel.h"

namespace d14engine::uikit
{
    // Define a dummy subclass to avoid the forward declaration of inner class.
    struct MouseButtonClickEvent : public ClickablePanel::Event
    {
        using ClickablePanel::Event::Event;

        MouseButtonClickEvent(const ClickablePanel::Event& rhs)
            :
            ClickablePanel::Event(rhs) {}

        MouseButtonClickEvent& operator=(const ClickablePanel::Event& rhs)
        {
            (ClickablePanel::Event)(*this) = rhs; return *this;
        }
    };
}
namespace d14uikit
{
#define DECLARE_PIMPL(Class_Name1, Class_Name2) \
    struct Class_Name1::Impl \
    { \
        std::shared_ptr<d14engine::uikit::Class_Name2> uiobj = {}; \
    };
#define DECLARE_DUMMY_PIMPL(Class_Name) DECLARE_PIMPL(Class_Name, Class_Name)

    DECLARE_DUMMY_PIMPL(Event)
    DECLARE_DUMMY_PIMPL(SizeEvent)
    DECLARE_DUMMY_PIMPL(MoveEvent)
    DECLARE_DUMMY_PIMPL(MouseEvent)
    DECLARE_DUMMY_PIMPL(KeyboardEvent)
    DECLARE_DUMMY_PIMPL(MouseMoveEvent)
    DECLARE_DUMMY_PIMPL(MouseWheelEvent)
    DECLARE_DUMMY_PIMPL(MouseButtonEvent)
    DECLARE_DUMMY_PIMPL(MouseButtonClickEvent)


#undef DECLARE_PIMPL
#undef DECLARE_DUMMY_PIMPL
}
