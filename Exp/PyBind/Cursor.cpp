#include "Common/Precompile.h"

#include "Cursor.h"
#include "Panel.h"

#include "Inc/Cursor.h"

namespace d14uikit
{
    void Py_InitCursor(py::module_& m)
    {
        py::class_<Cursor, Panel, PyPanel<Cursor>> i(m, "Cursor");

        i.def(py::init<>());

        py::enum_<Cursor::StaticIconIndex>(i, "StaticIconIndex")
            .value("Alternate", Cursor::Alternate)
            .value("Arrow",     Cursor::Arrow)
            .value("BackDiag",  Cursor::BackDiag)
            .value("Beam",      Cursor::Beam)
            .value("Hand",      Cursor::Hand)
            .value("Help",      Cursor::Help)
            .value("HorzSize",  Cursor::HorzSize)
            .value("MainDiag",  Cursor::MainDiag)
            .value("Move",      Cursor::Move)
            .value("Pen",       Cursor::Pen)
            .value("Person",    Cursor::Person)
            .value("Pin",       Cursor::Pin)
            .value("Select",    Cursor::Select)
            .value("Stop",      Cursor::Stop)
            .value("VertSize",  Cursor::VertSize)
            .export_values();

        py::enum_<Cursor::DynamicIconIndex>(i, "DynamicIconIndex")
            .value("Busy",      Cursor::Busy)
            .value("Working",   Cursor::Working)
            .export_values();

        i.def("setIcon", py::overload_cast<
              Cursor::StaticIconIndex>(&Cursor::setIcon),
              "index"_a);

        i.def("setIcon", py::overload_cast<
              Cursor::DynamicIconIndex>(&Cursor::setIcon),
              "index"_a);
    }
}
