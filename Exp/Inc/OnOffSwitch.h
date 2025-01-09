#pragma once

#include "ClickablePanel.h"
#include "StatefulObject.h"

_D14_UIKIT_FWDEF(OnOffSwitch)

namespace d14uikit
{
    class DllExport OnOffSwitch : public ClickablePanel
    {
        _D14_UIKIT_PIMPL(OnOffSwitch)

        OnOffSwitch();

        enum class State
        {
            On, Off
        };
        constexpr static auto On = State::On;
        constexpr static auto Off = State::Off;

        _D14_UIKIT_STATEFUL_OBJECT_DECL(OnOffSwitch)
    };
}
