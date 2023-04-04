#include "Application.h"
#include "ComboBox.h"
#include "ComboBoxItem.h"
#include "Image.h"
#include "MainWindow.h"
#include "PopupMenu.h"

using namespace d14uikit;

int main()
{
    Application app;

    MainWindow mwnd;

    Image img(L"test.png");

    Panel p;
    mwnd.setContent(&p);

    std::list<std::shared_ptr<MenuItem>> items;
    std::list<MenuItem*> pitems;
    for (int i = 0; i < 5; ++i)
    {
        auto item = std::make_shared<MenuItem>(L"Item " + std::to_wstring(i), L"Ctrl+F" + std::to_wstring(i));
        item->setIcon(&img);
        item->setIconSize(Size{ 24, 24 });
        item->setFontSize(12);
        item->syncLabelHotkeyTextFormat();
        items.push_back(item);
        pitems.push_back(item.get());
    }
    ComboBox selector;
    selector.setParent(&p);
    selector.setX(200);
    selector.setY(200);
    selector.setWidth(300);
    selector.setRoundRadius(5);
    selector.dropDownMenu()->setHeight(200);
    selector.dropDownMenu()->setRoundExtension(5);
    selector.dropDownMenu()->appendItem(pitems);
    selector.setCurrSelected(0);

    return app.run();
}
