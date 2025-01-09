#include "Common/Precompile.h"

#include "TabGroup.h"

#include "Callback.h"

#include "Inc/TabCaption.h"

namespace d14uikit
{
    void Py_InitTabGroup(py::module_& m)
    {
        py::class_<TabGroup, ResizablePanel, PyTabGroup<>> i(m, "TabGroup");

        i.def(py::init());

        i.def_property("cardSize", &TabGroup::cardSize, &TabGroup::setCardSize);

        // Tab
        {
            using Tab = TabGroup::Tab;
            py::class_<Tab> i2(i, "Tab");

            i2.def(py::init());
            i2.def(py::init<TabCaption*, Panel*>(), "caption"_a, "content"_a);

            i2.def_readwrite("caption", &Tab::caption);
            i2.def_readwrite("content", &Tab::content);
        }

        i.def("insertTab", &TabGroup::insertTab, "tab"_a, "index"_a = 0);
        i.def("appendTab", &TabGroup::appendTab, "tab"_a);

        i.def("removeTab", &TabGroup::removeTab, "index"_a, "count"_a = 1);
        i.def("clearAllTabs", &TabGroup::clearAllTabs);

        i.def_property_readonly("tabCount", &TabGroup::tabCount);

        i.def_property_readonly("currSelected", &TabGroup::currSelected);
        i.def("setCurrSelected", &TabGroup::setCurrSelected, "index"_a);

        i.def_property_readonly("currSelectedTitle", &TabGroup::currSelectedTitle);

        _D14_CALLBACK_PROPERTY(TabGroup, onSelectedChange);

        i.def("onSelectedChange", &ExTabGroup::onSelectedChange, "title"_a);
    }
}
