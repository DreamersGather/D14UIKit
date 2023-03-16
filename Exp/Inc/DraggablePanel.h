#pragma once

#include "Common.h"

#include "Panel.h"

namespace d14uikit
{
    class DllExport DraggablePanel : virtual public Panel
    {
        _D14_UIKIT_PIMPL_DEF

        DraggablePanel();

        bool draggable() const;
        void setDraggable(bool value);

        struct Callback
        {
            std::function<void(DraggablePanel*)> onEndDragging = {};

            std::function<void(DraggablePanel*)> onStartDragging = {};
        };
        Callback& callback() const;

    protected:
        void initialize();
        explicit DraggablePanel(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        virtual void onEndDragging();

        virtual void onStartDragging();
    };
}
