#include "Common/Precompile.h"

#include "Window.h"

#include "Common.h"
#include "IconLabel.h"
#include "Label.h"
#include "Panel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/IconLabel.h"
#include "UIKit/Window.h"

using namespace d14engine;

namespace d14uikit
{
    Window::Window(const std::wstring& title)
        :
        Window(uikit::makeUIObject<uikit::Window>(title)) { }

    _D14_UIKIT_CTOR(Window)
        :
        Panel(uiobj),
        DraggablePanel(uiobj),
        ResizablePanel(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

        pimpl->uiobj->f_onMinimize = [this]
        (uikit::Window* w)
        {
            onMinimize();
            if (pcallback->onMinimize)
            {
                pcallback->onMinimize(this);
            }
        };
        pimpl->uiobj->f_onMaximize = [this]
        (uikit::Window* w)
        {
            onMaximize();
            if (pcallback->onMaximize)
            {
                pcallback->onMaximize(this);
            }
        };
        pimpl->uiobj->f_onRestore = [this]
        (uikit::Window* w)
        {
            onRestore();
            if (pcallback->onRestore)
            {
                pcallback->onRestore(this);
            }
        };
        pimpl->uiobj->f_onClose = [this]
        (uikit::Window* w)
        {
            onClose();
            if (pcallback->onClose)
            {
                pcallback->onClose(this);
            }
        };
        _D14_UIKIT_BIND(IconLabel, caption);
    }

    int Window::captionHeight() const
    {
        return math_utils::round(pimpl->uiobj->captionPanelHeight());
    }

    void Window::setCaptionHeight(int value)
    {
        pimpl->uiobj->setCaptionPanelHeight((float)value);
    }

    int Window::decoBarHeight() const
    {
        return math_utils::round(pimpl->uiobj->decorativeBarHeight());
    }

    void Window::setDecoBarHeight(int value)
    {
        pimpl->uiobj->setDecorativeBarHeight((float)value);
    }

    IconLabel* Window::caption() const
    {
        return pimpl->caption.get();
    }

    const std::wstring& Window::title() const
    {
        return pimpl->caption->label()->text();
    }

    void Window::setTitle(const std::wstring& title)
    {
        pimpl->caption->label()->setText(title);
    }

    int Window::nonContentHeight() const
    {
        return math_utils::round(pimpl->uiobj->nonClientAreaHeight());
    }

    int Window::contentHeight() const
    {
        return math_utils::round(pimpl->uiobj->clientAreaHeight());
    }

    Panel* Window::content() const
    {
        return pimpl->content;
    }

    void Window::setContent(Panel* uiobj)
    {
        pimpl->content = uiobj;

        if (uiobj != nullptr)
        {
            pimpl->uiobj->setCenterUIObject(uiobj->getImpl()->uiobj);
        }
        else pimpl->uiobj->setCenterUIObject(nullptr);
    }

    Window::DisplayState Window::displayState() const
    {
        return (DisplayState)pimpl->uiobj->displayState();
    }

    void Window::setDisplayState(DisplayState state)
    {
        pimpl->uiobj->setDisplayState((uikit::Window::DisplayState)state);
    }

    bool Window::minimizeButton() const
    {
        return pimpl->uiobj->isMinimizeEnabled;
    }

    void Window::setMinimizeButton(bool value)
    {
        pimpl->uiobj->isMinimizeEnabled = value;
    }

    bool Window::maximizeButton() const
    {
        return pimpl->uiobj->isMaximizeEnabled;
    }

    void Window::setMaximizeButton(bool value)
    {
        pimpl->uiobj->isMaximizeEnabled = value;
    }

    bool Window::closeButton() const
    {
        return pimpl->uiobj->isCloseEnabled;
    }

    void Window::setCloseButton(bool value)
    {
        pimpl->uiobj->isCloseEnabled = value;
    }

    Window::Callback& Window::callback() const
    {
        return *pcallback;
    }

    void Window::onMinimize() { }

    void Window::onMaximize() { }

    void Window::onRestore() { }

    void Window::onClose() { }
}
