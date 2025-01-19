#pragma once

#include "Common.h"

_D14_UIKIT_FWDEF(RenderObject)

namespace d14uikit
{
    class DllExport RenderObject : public NonCopyable
    {
        _D14_UIKIT_PIMPL(RenderObject)

        RenderObject();

        int priority() const;
        void setPriority(int value);

        struct Callback
        {
            std::function<void(RenderObject*)> update = {};

            std::function<void(RenderObject*)> draw = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void update();

        virtual void draw();
    };
}
