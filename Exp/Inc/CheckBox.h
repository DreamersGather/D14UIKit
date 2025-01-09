#pragma once

#include "ClickablePanel.h"
#include "StatefulObject.h"

_D14_UIKIT_FWDEF(CheckBox)

namespace d14uikit
{
    class DllExport CheckBox : public ClickablePanel
    {
        _D14_UIKIT_PIMPL(CheckBox)

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

        bool tripleState() const;
        void setTripleState(bool value);

        _D14_UIKIT_STATEFUL_OBJECT_DECL(CheckBox)
    };
}
