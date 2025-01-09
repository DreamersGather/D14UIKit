#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(ResizablePanel)

namespace d14uikit
{
    class DllExport ResizablePanel : virtual public Panel
    {
        _D14_UIKIT_PIMPL(ResizablePanel)

        ResizablePanel();

        enum class Border
        {
            All, Left, Right, Top, Bottom
        };
#define SET_BORDER(Name) constexpr static auto Name = Border::Name;

        SET_BORDER(All)
        SET_BORDER(Left)
        SET_BORDER(Right)
        SET_BORDER(Top)
        SET_BORDER(Bottom)

#undef SET_BORDER

        bool resizable(Border border = All) const;
        void setResizable(bool value, Border border = All);

        bool dynamicSizing() const;
        void setDynamicSizing(bool value);

        struct Callback
        {
            std::function<void(ResizablePanel*)> onEndResizing = {};

            std::function<void(ResizablePanel*)> onStartResizing = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onEndResizing();

        virtual void onStartResizing();
    };
}
