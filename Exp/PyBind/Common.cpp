#include "Common/Precompile.h"

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "Common.h"

#ifdef _D14_UIKIT_EXP_PY_BIND_DEBUG
#include "__Test__.h"
#endif
#include "Application.h"
#include "BasicType.h"
#include "ClickablePanel.h"
#include "DraggablePanel.h"
#include "Event.h"
#include "MainWindow.h"
#include "Panel.h"
#include "ResizablePanel.h"
#include "VirtualKeyCode.h"
#include "Window.h"

namespace d14uikit
{
    PYBIND11_MODULE(D14UIKit, m)
    {
#ifdef _D14_UIKIT_EXP_PY_BIND_DEBUG
        Py_InitTest(m);
#endif
        // These init funcs are called when the module loaded.
        // It is worth noting that the invocation order here
        // is significant: we must first init the base classes
        // to avoid pybind11 incomplete type reference error.
        Py_InitApplication(m);
        Py_InitBasicType(m);
        Py_InitEvent(m);
        Py_InitPanel(m);
        Py_InitClickablePanel(m);
        Py_InitDraggablePanel(m);
        Py_InitResizablePanel(m);
        Py_InitVirtualKeyCode(m);
        Py_InitWindow(m);
        Py_InitMainWindow(m);
    }
}

#endif
