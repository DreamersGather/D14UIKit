#pragma once

#include "Panel.h"

_D14_UIKIT_FWDEF(ScenePanel)

namespace d14uikit
{
    class RenderObject;

    class DllExport ScenePanel : public Panel
    {
        _D14_UIKIT_PIMPL(ScenePanel)

        ScenePanel();

        void addObject(RenderObject* obj);
        void removeObject(RenderObject* obj);
    };
}
