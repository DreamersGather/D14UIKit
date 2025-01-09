#include "Common/Precompile.h"

#include "ComboBox.h"

#include "Common.h"
#include "IconLabel.h"
#include "Label.h"
#include "MenuItem.h"
#include "PopupMenu.h"

#include "UIKit/ComboBox.h"
#include "UIKit/IconLabel.h"
#include "UIKit/Label.h"
#include "UIKit/PopupMenu.h"

using namespace d14engine;

namespace d14uikit
{
    ComboBox::ComboBox()
        :
        ComboBox(uikit::makeUIObject<uikit::ComboBox>()) { }

    _D14_UIKIT_CTOR(ComboBox)
        :
        Panel(uiobj),
        FlatButton(uiobj),
        pimpl(std::make_shared<Impl>()),
        pcallback(std::make_unique<Callback>())
    {
        pimpl->uiobj = uiobj;

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
        _D14_UIKIT_BIND(PopupMenu, dropDownMenu);
    }

    MenuItem* ComboBox::currSelected() const
    {
        auto& currItem = pimpl->uiobj->currSelected();
        if (!currItem.expired())
        {
            pimpl->currSelected = std::shared_ptr<MenuItem>(new MenuItem(currItem.lock()));
        }
        else pimpl->currSelected.reset();

        return pimpl->currSelected.get();
    }

    void ComboBox::setCurrSelected(int index)
    {
        pimpl->uiobj->setCurrSelected(index);
    }

    std::optional<std::wstring_view> ComboBox::currSelectedText() const
    {
        std::optional<std::wstring_view> text = {};

        // Must call this to update the variable
        auto selected = currSelected();
        if (selected != nullptr)
        {
            // No need to check because ViewItem::content is guaranteed to be valid
            text = selected->content()->label()->text();
        }
        return text;
    }

    PopupMenu* ComboBox::dropDownMenu() const
    {
        return pimpl->dropDownMenu.get();
    }

    ComboBox::Callback& ComboBox::callback() const { return *pcallback; }

    void ComboBox::onSelectedChange(const std::wstring& text) { }
}
