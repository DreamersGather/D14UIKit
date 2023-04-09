#include "Common/Precompile.h"

#include "Common.h"

#ifdef _D14_UIKIT_PYBIND_DEBUG
#include "__Test__.h"
#endif
#include "Application.h"
#include "BasicType.h"
#include "Button.h"
#include "CheckBox.h"
#include "ClickablePanel.h"
#include "ComboBox.h"
#include "ComboBoxItem.h"
#include "ConstraintLayout.h"
#include "Cursor.h"
#include "DraggablePanel.h"
#include "ElevatedButton.h"
#include "Event.h"
#include "FilledButton.h"
#include "FlatButton.h"
#include "Font.h"
#include "GridLayout.h"
#include "HorzSlider.h"
#include "Image.h"
#include "Label.h"
#include "LabelArea.h"
#include "ListView.h"
#include "ListViewItem.h"
#include "MainWindow.h"
#include "MenuItem.h"
#include "MenuSeparator.h"
#include "OnOffSwitch.h"
#include "OutlinedButton.h"
#include "Panel.h"
#include "PopupMenu.h"
#include "RawTextBox.h"
#include "RawTextEditor.h"
#include "ResizablePanel.h"
#include "ScrollView.h"
#include "TabCaption.h"
#include "TabGroup.h"
#include "TextBox.h"
#include "TextEditor.h"
#include "ToggleButton.h"
#include "TreeView.h"
#include "TreeViewItem.h"
#include "VertSlider.h"
#include "ViewItem.h"
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

        // Calls init funcs according to the inheritance order.
        // One indent for one layer of derivation relationship.

        Py_InitPanel(m);
            Py_InitCursor(m);
            Py_InitClickablePanel(m);
                Py_InitButton(m);
                    Py_InitFlatButton(m);
                        Py_InitComboBox(m);
                        Py_InitFilledButton(m);
                            Py_InitElevatedButton(m);
                            Py_InitToggleButton(m);
                        Py_InitOutlinedButton(m);
                Py_InitCheckBox(m);
                Py_InitOnOffSwitch(m);
            Py_InitDraggablePanel(m);
            Py_InitHorzSlider(m);
            Py_InitLabel(m);
                Py_InitLabelArea(m);
                    Py_InitRawTextBox(m);
                    Py_InitRawTextEditor(m);
                    Py_InitTextBox(m);
                    Py_InitTextEditor(m);
            Py_InitResizablePanel(m);
                Py_InitConstraintLayout(m);
                Py_InitGridLayout(m);
                Py_InitScrollView(m);
                    Py_InitListView(m);
                    Py_InitPopupMenu(m);
                    Py_InitTreeView(m);
                Py_InitTabGroup(m);
                Py_InitWindow(m);
                    Py_InitMainWindow(m);
            Py_InitTabCaption(m);
            Py_InitVertSlider(m);
            Py_InitViewItem(m);
                Py_InitListViewItem(m);
                Py_InitMenuItem(m);
                    Py_InitComboBoxItem(m);
                    Py_InitMenuSeparator(m);
                Py_InitTreeViewItem(m);
    }
}
