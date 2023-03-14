#include "Common/Precompile.h"

#include "ClickablePanel.h"
#include "Event.h"
#include "Panel.h"

#include "UIKit/ClickablePanel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    ClickablePanel::ClickablePanel() : ClickablePanel(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        uikit::makeUIObject<uikit::ClickablePanel>(
            D2D1_RECT_F{},
            uikit::resource_utils::g_solidColorBrush);

        Panel::initialize();
        ClickablePanel::initialize();
    }

    ClickablePanel::ClickablePanel(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void ClickablePanel::initialize()
    {
        pimpl->uiobj->f_onMouseButtonPress = [this]
        (uikit::ClickablePanel* clkp, uikit::ClickablePanel::Event& e)
        {
            MouseButtonClickEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseButtonPress(event);
            if (pcallback->onMouseButtonPress)
            {
                pcallback->onMouseButtonPress(this, event);
            }
        };
        pimpl->uiobj->f_onMouseButtonRelease = [this]
        (uikit::ClickablePanel* clkp, uikit::ClickablePanel::Event& e)
        {
            MouseButtonClickEvent event = {};
            *event.getImpl()->uiobj = e;
            onMouseButtonRelease(event);
            if (pcallback->onMouseButtonRelease)
            {
                pcallback->onMouseButtonRelease(this, event);
            }
        };
    }

    bool ClickablePanel::doubleClick() const
    {
        return pimpl->uiobj->enableDoubleClick;
    }

    void ClickablePanel::setDoubleClick(bool value)
    {
        pimpl->uiobj->enableDoubleClick = value;
    }

    ClickablePanel::Callback& ClickablePanel::callback() const { return *pcallback; }

    void ClickablePanel::onMouseButtonPress(MouseButtonClickEvent& event) { }

    void ClickablePanel::onMouseButtonRelease(MouseButtonClickEvent& event) { }
}
