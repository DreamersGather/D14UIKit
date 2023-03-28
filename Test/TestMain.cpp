#include "Application.h"
#include "Callback.h"
#include "Cursor.h"
#include "ElevatedButton.h"
#include "FilledButton.h"
#include "FlatButton.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "OutlinedButton.h"
#include "ToggleButton.h"

using namespace d14uikit;

int main()
{
    Application app;
    app.setResizable(true);
    app.setClearType(true);
    app.setDrawTextNatrualSymmetric(true);

    Image img(L"test.png");

    constexpr auto row = 64, col = 64;
    auto pixel = std::vector<Pixel>(row * col);

    for (int r = 0; r < 64; ++r)
    {
        for (int c = 0; c < 64; ++c)
        {
#pragma warning(push)
#pragma warning(disable : 6011)

            pixel[r * 64 + c] =
            {
                (uint8_t)(r + c + 128),
                (uint8_t)(255 - r),
                (uint8_t)(255 - c),
                (uint8_t)255 // b,g,r,a
            };
#pragma warning(pop)
        }
    }
    img.copy({ 0, 0, col, row }, pixel.data());

    Panel p;

    Panel pp;
    pp.setParent(&p);
    pp.setWidth(256);
    pp.setHeight(256);
    pp.setImage(&img);

    pp.D14_onMouseMove(&, p, e)
    {
        app.cursor()->setIcon(Cursor::Busy);
    };

    Label note;
    note.setParent(&p);
    note.setWidth(256);
    note.setHeight(60);
    note.setX(0);
    note.setY(256);
    note.setText(L"dragon");
    note.setFont(Font(L"Default/Bold/18"));
    note.setHorzAlign(Label::HCenter);

    FlatButton btn1;
    btn1.setParent(&p);
    btn1.setX(300);
    btn1.setY(40);
    btn1.setWidth(240);
    btn1.setHeight(60);
    btn1.setRoundRadius(5);
    btn1.setIcon(&img);
    btn1.setIconSize({ 32, 32 });
    btn1.setText(L"Flat Button");

    btn1.D14_onMouseEnter(, p, e)
    {
        auto btn = dynamic_cast<Button*>(p);
        btn->setText(L"Gotcha!");
    };
    btn1.D14_onMouseLeave(, p, e)
    {
        auto btn = dynamic_cast<Button*>(p);
        btn->setText(L"No idea.");
    };

    FilledButton btn2;
    btn2.setParent(&p);
    btn2.setX(300);
    btn2.setY(140);
    btn2.setWidth(240);
    btn2.setHeight(60);
    btn2.setRoundRadius(5);
    btn2.setIcon(&img);
    btn2.setIconSize({ 32, 32 });
    btn2.setText(L"Filled Button");

    btn2.D14_onMouseButton(, p, e)
    {
        auto btn = dynamic_cast<Button*>(p);
        if (e->leftDblclk())
        {
            btn->setText(L"Left");
        }
        else if (e->rightDblclk())
        {
            btn->setText(L"Right");
        }
        else if (e->middleDblclk())
        {
            btn->setText(L"Middle");
        }
    };

    OutlinedButton btn3;
    btn3.setParent(&p);
    btn3.setX(300);
    btn3.setY(240);
    btn3.setWidth(240);
    btn3.setHeight(60);
    btn3.setRoundRadius(5);
    btn3.setIcon(&img);
    btn3.setIconSize({ 32, 32 });
    btn3.setText(L"Outlined Button");

    btn3.D14_onKeyboard(, p, e)
    {
        if (e->pressed() && isalpha(e->vkey()))
        {
            auto btn = dynamic_cast<Button*>(p);
            btn->setText({ 1, (wchar_t)e->vkey() });
        }
    };

    ElevatedButton btn4;
    btn4.setParent(&p);
    btn4.setX(300);
    btn4.setY(340);
    btn4.setWidth(240);
    btn4.setHeight(60);
    btn4.setRoundRadius(5);
    btn4.setIcon(&img);
    btn4.setIconSize({ 32, 32 });
    btn4.setText(L"Elevated Button");

    btn4.D14_onMouseButtonRelease(, clkp, e)
    {
        auto btn = (Button*)clkp;
        if (e->left())
        {
            btn->setText(L"Left");
        }
        else if (e->right())
        {
            btn->setText(L"Right");
        }
        else if (e->middle())
        {
            btn->setText(L"Middle");
        }
    };

    ToggleButton btn5;
    btn5.setParent(&p);
    btn5.setX(300);
    btn5.setY(440);
    btn5.setWidth(240);
    btn5.setHeight(60);
    btn5.setRoundRadius(5);
    btn5.setIcon(&img);
    btn5.setIconSize({ 32, 32 });
    btn5.setText(L"Toggle Button");

    btn5.D14_onStateChange(ToggleButton, , btn, state)
    {
        switch (state)
        {
        case ToggleButton::State::Activated:
        {
            btn->setText(L"Activated");
            break;
        }
        case ToggleButton::State::Deactivated:
        {
            btn->setText(L"Deactivated");
            break;
        }
        default: break;
        }
    };

    MainWindow w;
    w.setContent(&p);

    w.D14_onClose(&, w)
    {
        app.exit();
    };
    return app.run();
}
