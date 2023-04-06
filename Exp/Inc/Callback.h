#pragma once

#define D14_onChangeLangLocale(p, name, cap) \
Panel::callback().onChangeLangLocale = [cap](Panel* p, const std::wstring& name)

#define D14_onChangeTheme(p, name, cap) \
Panel::callback().onChangeTheme = [cap](Panel* p, const std::wstring& name)

#define D14_onClose(w, cap) \
Window::callback().onClose = [cap](Window* w)

#define D14_onEndDragging(dgp, cap) \
DraggablePanel::callback().onEndDragging = [cap](DraggablePanel* dgp)

#define D14_onEndResizing(rsp, cap) \
ResizablePanel::callback().onEndResizing = [cap](ResizablePanel* rsp)

#define D14_onEndSliding(type, sldr, value, cap) \
type::callback().onEndSliding = [cap](type* sldr, float value)

#define D14_onEndThumbScrolling(sv, offset, cap) \
ScrollView::callback().onEndThumbScrolling = [cap](ScrollView* sv, const D2D1_POINT_2F& offset)

#define D14_onGetFocus(p, cap) \
Panel::callback().onGetFocus = [cap](Panel* p)

#define D14_onKeyboard(p, e, cap) \
Panel::callback().onKeyboard = [cap](Panel* p, KeyboardEvent* e)

#define D14_onLoseFocus(p, cap) \
Panel::callback().onLoseFocus = [cap](Panel* p)

#define D14_onMaximize(w, cap) \
Window::callback().onMaximize = [cap](Window* w)

#define D14_onMinimize(w, cap) \
Window::callback().onMinimize = [cap](Window* w)

#define D14_onMouseButton(p, e, cap) \
Panel::callback().onMouseButton = [cap](Panel* p, MouseButtonEvent* e)

#define D14_onMouseButtonPress(clkp, e, cap) \
ClickablePanel::callback().onMouseButtonPress = [cap](ClickablePanel* clkp, MouseButtonClickEvent* e)

#define D14_onMouseButtonRelease(clkp, e, cap) \
ClickablePanel::callback().onMouseButtonRelease = [cap](ClickablePanel* clkp, MouseButtonClickEvent* e)

#define D14_onMouseEnter(p, e, cap) \
Panel::callback().onMouseEnter = [cap](Panel* p, MouseMoveEvent* e)

#define D14_onMouseLeave(p, e, cap) \
Panel::callback().onMouseLeave = [cap](Panel* p, MouseMoveEvent* e)

#define D14_onMouseMove(p, e, cap) \
Panel::callback().onMouseMove = [cap](Panel* p, MouseMoveEvent* e)

#define D14_onMouseWheel(p, e, cap) \
Panel::callback().onMouseWheel = [cap](Panel* p, MouseWheelEvent* e)

#define D14_onMove(p, e, cap) \
Panel::callback().onMove = [cap](Panel* p, MoveEvent* e)

#define D14_onRestore(w, cap) \
Window::callback().onRestore = [cap](Window* w)

#define D14_onSelectedChange(cb, text, cap) \
ComboBox::callback().onSelectedChange = [cap](ComboBox* cb, const std::wstring& text)

#define D14_onSize(p, e, cap) \
Panel::callback().onSize = [cap](Panel* p, SizeEvent* e)

#define D14_onStartDragging(dgp, cap) \
DraggablePanel::callback().onStartDragging = [cap](DraggablePanel* dgp)

#define D14_onStartResizing(rsp, cap) \
ResizablePanel::callback().onStartResizing = [cap](ResizablePanel* rsp)

#define D14_onStartSliding(type, sldr, value, cap) \
type::callback().onStartSliding = [cap](type* sldr, float value)

#define D14_onStartThumbScrolling(sv, offset, cap) \
ScrollView::callback().onStartThumbScrolling = [cap](ScrollView* sv, const D2D1_POINT_2F& offset)

#define D14_onStateChange(type, obj, state, cap) \
type::callback().onStateChange = [cap](type* obj, type::State state)

#define D14_onTriggerMenuItem(menu, text, cap) \
PopupMenu::callback().onTriggerMenuItem = [cap](PopupMenu* menu, const std::wstring& text)

#define D14_onValueChange(type, obj, value, cap) \
type::callback().onValueChange = [cap](type* obj, float value)

#define D14_onViewportOffsetChange(sv, offset, cap) \
ScrollView::callback().onViewportOffsetChange = [cap](ScrollView* sv, const Point& offset)
