#include "Application.h"
#include "Cursor.h"
#include "ElevatedButton.h"
#include "FilledButton.h"
#include "FlatButton.h"
#include "Image.h"
#include "MainWindow.h"
#include "OutlinedButton.h"
#include "ToggleButton.h"

using namespace d14uikit;

int wmain(int argc, wchar_t* argv[])
{
    Application app(argc, argv);
    app.setResizable(true);

    Image ico(L"test.png");

    Panel p;

    Panel pp;
    pp.setParent(&p);
    pp.setWidth(256);
    pp.setHeight(256);
    pp.setImage(&ico);

    FlatButton btn1;
    btn1.setParent(&p);
    btn1.setX(300);
    btn1.setY(40);
    btn1.setWidth(240);
    btn1.setHeight(60);
    btn1.setRoundRadius(5);
    btn1.setIcon(&ico);
    btn1.setIconSize({ 32, 32 });
    btn1.setText(L"Flat Button");

    FilledButton btn2;
    btn2.setParent(&p);
    btn2.setX(300);
    btn2.setY(140);
    btn2.setWidth(240);
    btn2.setHeight(60);
    btn2.setRoundRadius(5);
    btn2.setIcon(&ico);
    btn2.setIconSize({ 32, 32 });
    btn2.setText(L"Filled Button");

    OutlinedButton btn3;
    btn3.setParent(&p);
    btn3.setX(300);
    btn3.setY(240);
    btn3.setWidth(240);
    btn3.setHeight(60);
    btn3.setRoundRadius(5);
    btn3.setIcon(&ico);
    btn3.setIconSize({ 32, 32 });
    btn3.setText(L"Outlined Button");

    ElevatedButton btn4;
    btn4.setParent(&p);
    btn4.setX(300);
    btn4.setY(340);
    btn4.setWidth(240);
    btn4.setHeight(60);
    btn4.setRoundRadius(5);
    btn4.setIcon(&ico);
    btn4.setIconSize({ 32, 32 });
    btn4.setText(L"Elevated Button");

    btn4.ClickablePanel::callback().onMouseButtonRelease = []
    (ClickablePanel* clkp, MouseButtonClickEvent& e)
    {
        auto btn = (Button*)clkp;
        if (e.left())
        {
            btn->setText(L"Left clicked");
        }
        else if (e.right())
        {
            btn->setText(L"Right clicked");
        }
        else if (e.middle())
        {
            btn->setText(L"Middle clicked");
        }
    };

    ToggleButton btn5;
    btn5.setParent(&p);
    btn5.setX(300);
    btn5.setY(440);
    btn5.setWidth(240);
    btn5.setHeight(60);
    btn5.setRoundRadius(5);
    btn5.setIcon(&ico);
    btn5.setIconSize({ 32, 32 });
    btn5.setText(L"Toggle Button");

    btn5.callback().onStateChange = []
    (ToggleButton* btn, ToggleButton::State state)
    {
        if (state == ToggleButton::Activated)
        {
            btn->setText(L"Activated");
        }
        else if (state == ToggleButton::Deactivated)
        {
            btn->setText(L"Deactivated");
        }
    };

    MainWindow w;
    w.setContent(&p);

    w.callback().onClose = [&]
    (Window* w)
    {
        app.exit();
    };
    return app.run();
}
