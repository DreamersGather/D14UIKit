#pragma once

#include "Panel.h"

namespace d14uikit
{
    class DllExport ResizablePanel : virtual public Panel
    {
        _D14_UIKIT_PIMPL_DEF

        ResizablePanel();

        bool resizable() const;
        void setResizable(bool value);

        bool dynamicSizing() const;
        void setDynamicSizing(bool value);

        struct Callback
        {
            std::function<void(ResizablePanel*)> onEndResizing = {};

            std::function<void(ResizablePanel*)> onStartResizing = {};
        };
        Callback& callback() const;

    protected:
        void initialize();
        explicit ResizablePanel(Passkey);

        std::unique_ptr<Callback> pcallback = {};

        virtual void onEndResizing();

        virtual void onStartResizing();
    };
}
