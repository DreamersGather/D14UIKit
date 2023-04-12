#pragma once

#define D14_onChangeLangLocale(p, name, ...) \
Panel::callback().onChangeLangLocale = [__VA_ARGS__](Panel* p, const std::wstring& name)

#define D14_onChangeTheme(p, name, ...) \
Panel::callback().onChangeTheme = [__VA_ARGS__](Panel* p, const std::wstring& name)

#define D14_onClose(w, ...) \
Window::callback().onClose = [__VA_ARGS__](Window* w)

#define D14_onEndDragging(dgp, ...) \
DraggablePanel::callback().onEndDragging = [__VA_ARGS__](DraggablePanel* dgp)

#define D14_onEndResizing(rsp, ...) \
ResizablePanel::callback().onEndResizing = [__VA_ARGS__](ResizablePanel* rsp)

#define D14_onEndSliding(type, sldr, value, ...) \
type::callback().onEndSliding = [__VA_ARGS__](type* sldr, float value)

#define D14_onEndThumbScrolling(sv, offset, ...) \
ScrollView::callback().onEndThumbScrolling = [__VA_ARGS__](ScrollView* sv, const D2D1_POINT_2F& offset)

#define D14_onGetFocus(p, ...) \
Panel::callback().onGetFocus = [__VA_ARGS__](Panel* p)

#define D14_onKeyboard(p, e, ...) \
Panel::callback().onKeyboard = [__VA_ARGS__](Panel* p, KeyboardEvent* e)

#define D14_onLoseFocus(p, ...) \
Panel::callback().onLoseFocus = [__VA_ARGS__](Panel* p)

#define D14_onMaximize(w, ...) \
Window::callback().onMaximize = [__VA_ARGS__](Window* w)

#define D14_onMinimize(w, ...) \
Window::callback().onMinimize = [__VA_ARGS__](Window* w)

#define D14_onMouseButton(p, e, ...) \
Panel::callback().onMouseButton = [__VA_ARGS__](Panel* p, MouseButtonEvent* e)

#define D14_onMouseButtonPress(clkp, e, ...) \
ClickablePanel::callback().onMouseButtonPress = [__VA_ARGS__](ClickablePanel* clkp, MouseButtonClickEvent* e)

#define D14_onMouseButtonRelease(clkp, e, ...) \
ClickablePanel::callback().onMouseButtonRelease = [__VA_ARGS__](ClickablePanel* clkp, MouseButtonClickEvent* e)

#define D14_onMouseEnter(p, e, ...) \
Panel::callback().onMouseEnter = [__VA_ARGS__](Panel* p, MouseMoveEvent* e)

#define D14_onMouseLeave(p, e, ...) \
Panel::callback().onMouseLeave = [__VA_ARGS__](Panel* p, MouseMoveEvent* e)

#define D14_onMouseMove(p, e, ...) \
Panel::callback().onMouseMove = [__VA_ARGS__](Panel* p, MouseMoveEvent* e)

#define D14_onMouseWheel(p, e, ...) \
Panel::callback().onMouseWheel = [__VA_ARGS__](Panel* p, MouseWheelEvent* e)

#define D14_onMove(p, e, ...) \
Panel::callback().onMove = [__VA_ARGS__](Panel* p, MoveEvent* e)

#define D14_onRestore(w, ...) \
Window::callback().onRestore = [__VA_ARGS__](Window* w)

#define D14_onSelectedChange(type, obj, text, ...) \
type::callback().onSelectedChange = [__VA_ARGS__](type* obj, const std::wstring& text)

#define D14_onSize(p, e, ...) \
Panel::callback().onSize = [__VA_ARGS__](Panel* p, SizeEvent* e)

#define D14_onStartDragging(dgp, ...) \
DraggablePanel::callback().onStartDragging = [__VA_ARGS__](DraggablePanel* dgp)

#define D14_onStartResizing(rsp, ...) \
ResizablePanel::callback().onStartResizing = [__VA_ARGS__](ResizablePanel* rsp)

#define D14_onStartSliding(type, sldr, value, ...) \
type::callback().onStartSliding = [__VA_ARGS__](type* sldr, float value)

#define D14_onStartThumbScrolling(sv, offset, ...) \
ScrollView::callback().onStartThumbScrolling = [__VA_ARGS__](ScrollView* sv, const D2D1_POINT_2F& offset)

#define D14_onStateChange(type, obj, state, ...) \
type::callback().onStateChange = [__VA_ARGS__](type* obj, type::State state)

#define D14_onTextChange(type, obj, text, ...) \
type::callback().onTextChange = [__VA_ARGS__](type* obj, const std::wstring& text)

#define D14_onTriggerMenuItem(menu, text, ...) \
PopupMenu::callback().onTriggerMenuItem = [__VA_ARGS__](PopupMenu* menu, const std::wstring& text)

#define D14_onValueChange(type, obj, value, ...) \
type::callback().onValueChange = [__VA_ARGS__](type* obj, float value)

#define D14_onViewportOffsetChange(sv, offset, ...) \
ScrollView::callback().onViewportOffsetChange = [__VA_ARGS__](ScrollView* sv, const Point& offset)
