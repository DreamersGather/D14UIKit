#pragma once

#define D14_onChangeLangLocale(cap, p, name) \
Panel::callback().onChangeLangLocale = [cap](Panel* p, const std::wstring& name)

#define D14_onChangeTheme(cap, p, name) \
Panel::callback().onChangeTheme = [cap](Panel* p, const std::wstring& name)

#define D14_onClose(cap, w) \
Window::callback().onClose = [cap](Window* w)

#define D14_onEndDragging(cap, dgp) \
DraggablePanel::callback().onEndDragging = [cap](DraggablePanel* dgp)

#define D14_onEndResizing(cap, rsp) \
ResizablePanel::callback().onEndResizing = [cap](ResizablePanel* rsp)

#define D14_onEndThumbScrolling(cap, sv) \
ScrollView::callback().onEndThumbScrolling = [cap](ScrollView* sv)

#define D14_onGetFocus(cap, p) \
Panel::callback().onGetFocus = [cap](Panel* p)

#define D14_onKeyboard(cap, p, e) \
Panel::callback().onKeyboard = [cap](Panel* p, KeyboardEvent* e)

#define D14_onLoseFocus(cap, p) \
Panel::callback().onLoseFocus = [cap](Panel* p)

#define D14_onMaximize(cap, w) \
Window::callback().onMaximize = [cap](Window* w)

#define D14_onMinimize(cap, w) \
Window::callback().onMinimize = [cap](Window* w)

#define D14_onMouseButton(cap, p, e) \
Panel::callback().onMouseButton = [cap](Panel* p, MouseButtonEvent* e)

#define D14_onMouseButtonPress(cap, clkp, e) \
ClickablePanel::callback().onMouseButtonPress = [cap](ClickablePanel* clkp, MouseButtonClickEvent* e)

#define D14_onMouseButtonRelease(cap, clkp, e) \
ClickablePanel::callback().onMouseButtonRelease = [cap](ClickablePanel* clkp, MouseButtonClickEvent* e)

#define D14_onMouseEnter(cap, p, e) \
Panel::callback().onMouseEnter = [cap](Panel* p, MouseMoveEvent* e)

#define D14_onMouseLeave(cap, p, e) \
Panel::callback().onMouseLeave = [cap](Panel* p, MouseMoveEvent* e)

#define D14_onMouseMove(cap, p, e) \
Panel::callback().onMouseMove = [cap](Panel* p, MouseMoveEvent* e)

#define D14_onMouseWheel(cap, p, e) \
Panel::callback().onMouseWheel = [cap](Panel* p, MouseWheelEvent* e)

#define D14_onMove(cap, p, e) \
Panel::callback().onMove = [cap](Panel* p, MoveEvent* e)

#define D14_onRestore(cap, w) \
Window::callback().onRestore = [cap](Window* w)

#define D14_onSelectedChange(cap, cb, text) \
ComboBox::callback().onSelectedChange = [cap](ComboBox* cb, const std::wstring& text)

#define D14_onSize(cap, p, e) \
Panel::callback().onSize = [cap](Panel* p, SizeEvent* e)

#define D14_onStartDragging(cap, dgp) \
DraggablePanel::callback().onStartDragging = [cap](DraggablePanel* dgp)

#define D14_onStartResizing(cap, rsp) \
ResizablePanel::callback().onStartResizing = [cap](ResizablePanel* rsp)

#define D14_onStartThumbScrolling(cap, sv) \
ScrollView::callback().onStartThumbScrolling = [cap](ScrollView* sv)

#define D14_onStateChange(type, cap, obj, state) \
type::callback().onStateChange = [cap](type* obj, type::State state)

#define D14_onTriggerMenuItem(cap, menu, text) \
PopupMenu::callback().onTriggerMenuItem = [cap](PopupMenu* menu, const std::wstring& text)

#define D14_onViewportOffsetChange(cap, sv, offset) \
ScrollView::callback().onViewportOffsetChange = [cap](ScrollView* sv, const Point& offset)
