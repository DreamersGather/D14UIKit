#include "Application.h"
#include "Cursor.h"
#include "Image.h"
#include "MainWindow.h"

using namespace d14uikit;

int wmain(int argc, wchar_t* argv[])
{
    Application app(argc, argv);

    app.setResizable(true);

    Panel p, pp;

    pp.setParent(&p);

    pp.setX(272);
    pp.setY(172);

    pp.setWidth(256);
    pp.setHeight(256);

    Image bkgn(L"test.png");
    pp.setImage(&bkgn);

    pp.callback().onMouseMove = [&]
    (Panel* p, MouseMoveEvent& e)
    {
        app.cursor()->setIcon(Cursor::Working);
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
