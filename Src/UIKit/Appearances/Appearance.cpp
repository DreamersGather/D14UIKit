#include "Common/Precompile.h"

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
        g_colorGroup.generateTonedColors();

#define INIT_THEME_STYLES(Class_Name) \
        Class_Name::Appearance::initialize()

        INIT_THEME_STYLES(Button);
        INIT_THEME_STYLES(CheckBox);
        INIT_THEME_STYLES(ComboBox);
        INIT_THEME_STYLES(ElevatedButton);
        INIT_THEME_STYLES(FilledButton);
        INIT_THEME_STYLES(FlatButton);
        INIT_THEME_STYLES(Label);
        INIT_THEME_STYLES(LabelArea);
        INIT_THEME_STYLES(Layout);
        INIT_THEME_STYLES(MenuItem);
        INIT_THEME_STYLES(MenuSeparator);
        INIT_THEME_STYLES(OnOffSwitch);
        INIT_THEME_STYLES(OutlinedButton);
        INIT_THEME_STYLES(PopupMenu);
        INIT_THEME_STYLES(RawTextInput);
        INIT_THEME_STYLES(ResizablePanel);
        INIT_THEME_STYLES(ScrollView);
        INIT_THEME_STYLES(Slider);
        INIT_THEME_STYLES(TabCaption);
        INIT_THEME_STYLES(TabGroup);
        INIT_THEME_STYLES(TextInput);
        INIT_THEME_STYLES(ToggleButton);
        INIT_THEME_STYLES(TreeViewItem);
        INIT_THEME_STYLES(ViewItem);
        INIT_THEME_STYLES(Window);

#undef INIT_THEME_STYLES
    }

    void ColorGroup::generateTonedColors()
    {
        Application::ThemeStyle themeStyle = {};
        if (Application::g_app->customThemeStyle.has_value())
        {
            themeStyle = Application::g_app->customThemeStyle.value();
        }
        else themeStyle = Application::g_app->systemThemeStyle();

        iHSB hsb = rgb2hsb(convert(themeStyle.color));
        iHSB primary = hsb, secondary = hsb, tertiary = hsb;

        // Transform HSB color according to the selected theme mode.
        // Also see D14Engine/Src/UIKit/Appearances/ColorScheme.txt.
        switch (themeStyle.mode)
        {
        case Application::ThemeStyle::Mode::Light:
        {
            primary.S = 100;
            secondary.S = 85;
            tertiary.S = 70;
            primary.B = 70;
            secondary.B = 75;
            tertiary.B = 80;
            break;
        }
        case Application::ThemeStyle::Mode::Dark:
        {
            primary.S = 50;
            secondary.S = 50;
            tertiary.S = 50;
            primary.B = 90;
            secondary.B = 85;
            tertiary.B = 80;
            break;
        }
        default: break;
        }
        g_colorGroup.primary = convert(hsb2rgb(primary));
        g_colorGroup.secondary = convert(hsb2rgb(secondary));
        g_colorGroup.tertiary = convert(hsb2rgb(tertiary));
    }

    ColorGroup g_colorGroup = {};
}
