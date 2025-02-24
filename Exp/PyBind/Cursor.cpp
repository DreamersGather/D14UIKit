#include "Common/Precompile.h"

#include "Cursor.h"

#include "EnumBind.h"
#include "Panel.h"

#include "Inc/Cursor.h"

namespace d14uikit
{
    void Py_InitCursor(py::module_& m)
    {
        py::class_<Cursor, Panel, PyPanel<Cursor>> i(m, "Cursor");

        i.def(py::init());

        py::enum_<Cursor::StaticIconIndex>(i, "StaticIconIndex")
            _D14_UIKIT_PYBIND_ENUM(Cursor, Alternate)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Arrow)
            _D14_UIKIT_PYBIND_ENUM(Cursor, BackDiag)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Hand)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Help)
            _D14_UIKIT_PYBIND_ENUM(Cursor, HorzSize)
            _D14_UIKIT_PYBIND_ENUM(Cursor, MainDiag)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Move)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Person)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Pin)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Select)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Stop)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Text)
            _D14_UIKIT_PYBIND_ENUM(Cursor, VertSize)
            .export_values();

        py::enum_<Cursor::DynamicIconIndex>(i, "DynamicIconIndex")
            _D14_UIKIT_PYBIND_ENUM(Cursor, Busy)
            _D14_UIKIT_PYBIND_ENUM(Cursor, Working)
            .export_values();

        i.def("setIcon", py::overload_cast<
              Cursor::StaticIconIndex>(&Cursor::setIcon),
              "index"_a);

        i.def("setIcon", py::overload_cast<
              Cursor::DynamicIconIndex>(&Cursor::setIcon),
              "index"_a);
    }
}
