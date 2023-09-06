#include "Application.h"
#include "Callback.h"
#include "ComboBox.h"
#include "ComboBoxItem.h"
#include "ConstraintLayout.h"
#include "Label.h"
#include "MainWindow.h"
#include "PopupMenu.h"

using namespace d14uikit;

#define DEMO_NAME L"ModernTheme"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setHeight(300);
    app.setMinSize(app.size());
    app.setResizable(true);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);
    ConstraintLayout centerLayout;
    mwnd.setContent(&centerLayout);

    ConstraintLayout::GeoInfo geoInfo = {};

    Label themeLbl(L"Theme mode");
    themeLbl.setHeight(40);
    themeLbl.setHorzAlign(Label::HCenter);
    geoInfo = {};
    geoInfo.keepWidth = false;
    geoInfo.Left.ToLeft = 50;
    geoInfo.Right.ToLeft = 350;
    geoInfo.keepHeight = true;
    geoInfo.Top.ToTop = 100;
    centerLayout.addElement(&themeLbl, geoInfo);

    ComboBox themeSelector;
    themeSelector.setHeight(40);
    geoInfo = {};
    geoInfo.keepWidth = false;
    geoInfo.Left.ToRight = 350;
    geoInfo.Right.ToRight = 50;
    geoInfo.keepHeight = true;
    geoInfo.Top.ToTop = 100;
    centerLayout.addElement(&themeSelector, geoInfo);
    themeSelector.setRoundRadius(5);

    auto menu = themeSelector.dropDownMenu();

    ComboBoxItem items[3];
    items[0].setText(L"Light");
    items[1].setText(L"Dark");
    items[2].setText(L"Use system setting");

    std::list<MenuItem*> pItems;
    for (auto& item : items)
    {
        pItems.push_back(&item);
    }
    menu->appendItem(pItems);

    menu->setWidth(themeSelector.width());
    menu->setHeight(_countof(items) * 40);
    menu->setRoundExtension(5);

    themeSelector.setCurrSelected(2);
    
    //------------------------------------------- Set UI event callacks.

    themeSelector.D14_onSelectedChange(ComboBox, obj, text)
    {
        auto app = Application::app();
        if (text == L"Light" || text == L"Dark")
        {
            app->setThemeMode(text);
        }
        else if (text == L"Use system setting")
        {
            app->setUseSystemTheme(true);
        }
    };
    return app.run();
}
