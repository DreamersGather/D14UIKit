#include "Common/Precompile.h"

#include "Inc/ComboBoxItem.h"

#include "UIKit/IconLabel.h"
#include "UIKit/MenuItem.h"

using namespace d14engine;

namespace d14uikit
{
    ComboBoxItem::ComboBoxItem(const std::wstring& text)
        :
        MenuItem(uikit::makeUIObject<uikit::MenuItem>(
            uikit::IconLabel::comboBoxLayout(text))) { setHeight(40); }
}
