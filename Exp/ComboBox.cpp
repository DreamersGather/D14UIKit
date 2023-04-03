#include "Common/Precompile.h"

#include "ComboBox.h"

#include "Button.h"
#include "ClickablePanel.h"
#include "FlatButton.h"
#include "ListView.h"
#include "Panel.h"
#include "PopupMenu.h"
#include "ResizablePanel.h"
#include "ScrollView.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/ComboBox.h"
#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/MenuItem.h"
#include "UIKit/PopupMenu.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    ComboBox::ComboBox() : ComboBox(Passkey{})
    {
        Panel::pimpl->uiobj =
        ClickablePanel::pimpl->uiobj =
        Button::pimpl->uiobj =
        FlatButton::pimpl->uiobj =
        ComboBox::pimpl->uiobj =
        uikit::makeUIObject<uikit::ComboBox>();

        Panel::initialize();
        ClickablePanel::initialize();
        Button::initialize();
        FlatButton::initialize();
        ComboBox::initialize();
    }

    ComboBox::ComboBox(Passkey)
        :
        Panel(Panel::Passkey{}),
        FlatButton(FlatButton::Passkey{}),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>()) { }

    void ComboBox::initialize()
    {
        setHeight(40);

        pimpl->uiobj->f_onSelectedChange = [this]
        (uikit::ComboBox* cb, uikit::IconLabel* content)
        {
            auto& text = content->label()->text();
            onSelectedChange(text);
            if (pcallback->onSelectedChange)
            {
                pcallback->onSelectedChange(this, text);
            }
        };
        // Bind the existing menu implementation to the interface.
        {
            auto& ddm = pimpl->dropDownMenu;

            ddm = std::shared_ptr<PopupMenu>(
                new PopupMenu(PopupMenu::Passkey{}));

#define SET_PIMPL_UIOBJ(Type_Name) \
    ddm->Type_Name::pimpl->uiobj = \
        pimpl->uiobj->dropDownMenu()

            SET_PIMPL_UIOBJ(Panel);
            SET_PIMPL_UIOBJ(ResizablePanel);
            SET_PIMPL_UIOBJ(ScrollView);
            SET_PIMPL_UIOBJ(PopupMenu);

#undef SET_PIMPL_UIOBJ

            ddm->Panel::initialize();
            ddm->ResizablePanel::initialize();
            ddm->ScrollView::initialize();
            // f_onTriggerMenuItem of the drop-down menu was used.
        }
    }

    PopupMenu* ComboBox::dropDownMenu() const
    {
        return pimpl->dropDownMenu.get();
    }

    void ComboBox::setCurrSelected(int index)
    {
        pimpl->uiobj->setCurrSelected(index);
    }

    const std::wstring& ComboBox::currSelectedText() const
    {
        auto& currItem = pimpl->uiobj->currSelected();
        if (!currItem.expired())
        {
            auto content = currItem.lock()->getContent<uikit::IconLabel>();
            return content.lock()->label()->text();
        }
        else return uikit::resource_utils::emptyWstrRef();
    }

    ComboBox::Callback& ComboBox::callback() const { return *pcallback; }

    void ComboBox::onSelectedChange(const std::wstring& text) { }
}
