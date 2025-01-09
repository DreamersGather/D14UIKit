#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(ClickablePanel)

namespace d14uikit
{
    class DllExport ClickablePanel : virtual public Panel
    {
        _D14_UIKIT_PIMPL(ClickablePanel)

        ClickablePanel();

        bool doubleClick() const;
        void setDoubleClick(bool value);

        struct Callback
        {
            std::function<void(ClickablePanel*, MouseButtonClickEvent*)> onMouseButtonPress = {};

            std::function<void(ClickablePanel*, MouseButtonClickEvent*)> onMouseButtonRelease = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onMouseButtonPress(MouseButtonClickEvent* event);

        virtual void onMouseButtonRelease(MouseButtonClickEvent* event);
    };
}
