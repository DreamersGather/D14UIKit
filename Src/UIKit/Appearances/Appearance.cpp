#include "Common/Precompile.h"

#include "Common/RuntimeError.h"

#include "UIKit/Application.h"
#include "UIKit/ColorUtils.h"

using namespace d14engine::uikit::color_utils;

#include "UIKit/Appearances/Button.h"
#include "UIKit/Appearances/CheckBox.h"
#include "UIKit/Appearances/ComboBox.h"
#include "UIKit/Appearances/ElevatedButton.h"
#include "UIKit/Appearances/FilledButton.h"
#include "UIKit/Appearances/FlatButton.h"
#include "UIKit/Appearances/Label.h"
#include "UIKit/Appearances/LabelArea.h"
#include "UIKit/Appearances/Layout.h"
#include "UIKit/Appearances/MenuItem.h"
#include "UIKit/Appearances/MenuSeparator.h"
#include "UIKit/Appearances/OnOffSwitch.h"
#include "UIKit/Appearances/OutlinedButton.h"
#include "UIKit/Appearances/PopupMenu.h"
#include "UIKit/Appearances/RawTextInput.h"
#include "UIKit/Appearances/ResizablePanel.h"
#include "UIKit/Appearances/ScrollView.h"
#include "UIKit/Appearances/Slider.h"
#include "UIKit/Appearances/TabCaption.h"
#include "UIKit/Appearances/TabGroup.h"
#include "UIKit/Appearances/TextInput.h"
#include "UIKit/Appearances/ToggleButton.h"
#include "UIKit/Appearances/TreeViewItem.h"
#include "UIKit/Appearances/ViewItem.h"
#include "UIKit/Appearances/Window.h"

namespace d14engine::uikit::appearance
{
    void initialize()
    {
        THROW_IF_NULL(Application::g_app);

        g_colorGroup.generateTonedColors(Application::g_app->themeStyle());

#define INIT_THEME_DATA(Class_Name) \
        Class_Name::Appearance::initialize()

        INIT_THEME_DATA(Button);
        INIT_THEME_DATA(CheckBox);
        INIT_THEME_DATA(ComboBox);
        INIT_THEME_DATA(ElevatedButton);
        INIT_THEME_DATA(FilledButton);
        INIT_THEME_DATA(FlatButton);
        INIT_THEME_DATA(Label);
        INIT_THEME_DATA(LabelArea);
        INIT_THEME_DATA(Layout);
        INIT_THEME_DATA(MenuItem);
        INIT_THEME_DATA(MenuSeparator);
        INIT_THEME_DATA(OnOffSwitch);
        INIT_THEME_DATA(OutlinedButton);
        INIT_THEME_DATA(PopupMenu);
        INIT_THEME_DATA(RawTextInput);
        INIT_THEME_DATA(ResizablePanel);
        INIT_THEME_DATA(ScrollView);
        INIT_THEME_DATA(Slider);
        INIT_THEME_DATA(TabCaption);
        INIT_THEME_DATA(TabGroup);
        INIT_THEME_DATA(TextInput);
        INIT_THEME_DATA(ToggleButton);
        INIT_THEME_DATA(TreeViewItem);
        INIT_THEME_DATA(ViewItem);
        INIT_THEME_DATA(Window);

#undef INIT_THEME_DATA
    }

    void ColorGroup::generateTonedColors(const Appearance::ThemeStyle& style)
    {
        auto hsb = (iHSB)style.color;
        iHSB primary = hsb, secondary = hsb, tertiary = hsb;

        // Transform HSB color according to the selected color mode.
        // Also see D14Engine/Src/UIKit/Appearances/ColorScheme.txt.
        if (style.name == L"Light")
        {
            primary.s = 100;
            secondary.s = 85;
            tertiary.s = 70;
            primary.b = 70;
            secondary.b = 75;
            tertiary.b = 80;
        }
        else if (style.name == L"Dark")
        {
            primary.s = 50;
            secondary.s = 50;
            tertiary.s = 50;
            primary.b = 90;
            secondary.b = 85;
            tertiary.b = 80;
        }
        g_colorGroup.primary = (D2D1_COLOR_F)primary;
        g_colorGroup.secondary = (D2D1_COLOR_F)secondary;
        g_colorGroup.tertiary = (D2D1_COLOR_F)tertiary;
    }

    ColorGroup g_colorGroup = {};
}
