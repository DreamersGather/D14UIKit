#include "Common/Precompile.h"

#include "Common.h"

#ifdef _D14_UIKIT_PYBIND_DEBUG
#include "__Test__.h"
#endif
#include "Application.h"
#include "BasicType.h"
#include "Button.h"
#include "ClickablePanel.h"
#include "Cursor.h"
#include "DraggablePanel.h"
#include "ElevatedButton.h"
#include "Event.h"
#include "FilledButton.h"
#include "FlatButton.h"
#include "Font.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "OutlinedButton.h"
#include "Panel.h"
#include "ResizablePanel.h"
#include "ToggleButton.h"
#include "VirtualKeyCode.h"
#include "Window.h"

namespace d14uikit
{
    PYBIND11_MODULE(D14UIKit, m)
    {
#ifdef _D14_UIKIT_PYBIND_DEBUG
        Py_InitTest(m);
#endif
        // These init funcs are called when the module loaded.
        // It is worth noting that the invocation order here
        // is significant: we must first init the base classes
        // to avoid pybind11 incomplete type reference error.

        Py_InitBasicType(m);
        Py_InitVirtualKeyCode(m);

        Py_InitApplication(m);
        Py_InitEvent(m);
        Py_InitFont(m);
        Py_InitImage(m);
        Py_InitPanel(m);

        Py_InitCursor(m);
        Py_InitLabel(m);
        Py_InitClickablePanel(m);
        Py_InitButton(m);
        Py_InitFlatButton(m);
        Py_InitOutlinedButton(m);
        Py_InitFilledButton(m);
        Py_InitElevatedButton(m);
        Py_InitToggleButton(m);
        Py_InitDraggablePanel(m);
        Py_InitResizablePanel(m);
        Py_InitWindow(m);
        Py_InitMainWindow(m);
    }
}
