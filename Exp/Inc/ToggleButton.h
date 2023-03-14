#pragma once

#include "Common.h"

#include "FilledButton.h"

namespace d14uikit
{
    class DllExport ToggleButton : public FilledButton
    {
        _D14_UIKIT_PIMPL_DEF

        explicit ToggleButton(const std::wstring& text = L"Button");

        enum class State
        {
            Activated,
            Deactivated
        };
        constexpr static auto Activated = State::Activated;
        constexpr static auto Deactivated = State::Deactivated;

        State state() const;
        void setState(State state);

        struct Callback
        {
            std::function<void(ToggleButton*, State)> onStateChange = {};
        };
        Callback& callback() const;

    protected:
        void initialize();
        explicit ToggleButton(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        virtual void onStateChange(State state);
    };
}
