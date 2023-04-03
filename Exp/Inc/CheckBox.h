#pragma once

#include "ClickablePanel.h"

namespace d14uikit
{
    class DllExport CheckBox : public ClickablePanel
    {
        _D14_UIKIT_PIMPL_DEF

        explicit CheckBox(bool tripleState = false);

        enum class State
        {
            Unchecked,
            Intermediate,
            Checked
        };
        constexpr static auto Unchecked = State::Unchecked;
        constexpr static auto Intermediate = State::Intermediate;
        constexpr static auto Checked = State::Checked;

        State state() const;
        void setState(State state);

        bool tripleState() const;
        void setTripleState(bool value);

        struct Callback
        {
            std::function<void(CheckBox*, State)> onStateChange = {};
        };
        Callback& callback() const;

    protected:
        void initialize();
        explicit CheckBox(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        virtual void onStateChange(State state);
    };
}
