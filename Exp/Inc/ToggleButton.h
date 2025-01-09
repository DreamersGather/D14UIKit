#pragma once

#include "FilledButton.h"
#include "StatefulObject.h"

_D14_UIKIT_FWDEF(ToggleButton)

namespace d14uikit
{
    class DllExport ToggleButton : public FilledButton
    {
        _D14_UIKIT_PIMPL(ToggleButton)

        explicit ToggleButton(const std::wstring& text = L"ToggleButton");

        enum class State
        {
            Activated,
            Deactivated
        };
        constexpr static auto Activated = State::Activated;
        constexpr static auto Deactivated = State::Deactivated;

        _D14_UIKIT_STATEFUL_OBJECT_DECL(ToggleButton)
    };
}
