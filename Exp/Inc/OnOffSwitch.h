#pragma once

#include "ClickablePanel.h"

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

        State state() const;
        void setState(State state);

        struct Callback
        {
            std::function<void(OnOffSwitch*, State)> onStateChange = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onStateChange(State state);
    };
}
