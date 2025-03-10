﻿#include "Common/Precompile.h"

#include "Window.h"

#include "Callback.h"

#include "Inc/IconLabel.h"

namespace d14uikit
{
    void Py_InitWindow(py::module_& m)
    {
        py::class_<Window, DraggablePanel, ResizablePanel, PyWindow<>> i(
            m,
            "Window",
            py::multiple_inheritance());

        i.def(
            py::init<const std::wstring&>(),
            "title"_a = "Untitled");

        i.def_property(
            "captionHeight",
            &Window::captionHeight,
            &Window::setCaptionHeight);

        i.def_property(
            "decoBarHeight",
            &Window::decoBarHeight,
            &Window::setDecoBarHeight);

        i.def_property_readonly(
            "caption",
            &Window::caption);

        i.def_property(
            "title",
            &Window::title,
            &Window::setTitle);

        i.def_property_readonly(
            "contentHeight",
            &Window::contentHeight);

        i.def_property_readonly(
            "nonContentHeight",
            &Window::nonContentHeight);

        i.def_property(
            "content",
            &Window::content,
            &Window::setContent);

        i.def_property(
            "minimizeButton",
            &Window::minimizeButton,
            &Window::setMinimizeButton);

        i.def_property(
            "maximizeButton",
            &Window::maximizeButton,
            &Window::setMaximizeButton);

        i.def_property(
            "closeButton",
            &Window::closeButton,
            &Window::setCloseButton);

        _D14_CALLBACK_PROPERTY(Window, onClose);
        _D14_CALLBACK_PROPERTY(Window, onRestore);
        _D14_CALLBACK_PROPERTY(Window, onMaximize);
        _D14_CALLBACK_PROPERTY(Window, onMinimize);

        i.def("onClose", &ExWindow::onClose);
        i.def("onRestore", &ExWindow::onRestore);
        i.def("onMaximize", &ExWindow::onMaximize);
        i.def("onMinimize", &ExWindow::onMinimize);
    }
}
