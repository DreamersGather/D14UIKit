#include "Common/Precompile.h"

#include "Inc/ComboBoxItem.h"

#include "MenuItem.h"
#include "Panel.h"
#include "ViewItem.h"

#include "UIKit/IconLabel.h"
#include "UIKit/MenuItem.h"

using namespace d14engine;

namespace d14uikit
{
    ComboBoxItem::ComboBoxItem(const std::wstring& text)
        :
        ComboBoxItem(Passkey{})
    {
        Panel::pimpl->uiobj =
        ViewItem::pimpl->uiobj =
        MenuItem::pimpl->uiobj =
        uikit::makeUIObject<uikit::MenuItem>(
        uikit::IconLabel::comboBoxLayout(text));

        Panel::initialize();
        ViewItem::initialize();
        MenuItem::initialize();
        ComboBoxItem::initialize();
    }

    ComboBoxItem::ComboBoxItem(Passkey)
        :
        MenuItem(MenuItem::Passkey{}) { }

    void ComboBoxItem::initialize() { setHeight(40); }
}
