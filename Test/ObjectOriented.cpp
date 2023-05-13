#include "Application.h"
#include "Cursor.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"ObjectOriented"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI"))
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);

    //------------------------------------------- Initialize UI objects.

    // The main window is also a Panel, whose rectangle area
    // is divided into 2 parts: non-client and client area.
    // The non-client area consists of the caption panel and
    // the decorative bar; the center part is client area.
    //
    // It is worth noting that there is also a sizing frame
    // around the window (inherited from ResizablePanel),
    // which should also be a part of the non-client area,
    // but that is not the theme of this demostration app.
    MainWindow mwnd(DEMO_NAME);

    // Create a Panel object, which is the most derived UI object
    // in D14UIKit.  Basically, a Panel is a graphic rectangle
    // area on the screen, where we can draw an image as background
    // or place other UI objects as children.
    Panel clntArea;

    // Make the panel fills the client area of the main window.
    mwnd.setContent(&clntArea);

    // Image is also an object indeed, although not a UI object.
    Image img(L"test.png");

    // To display the image on the screen, we need to create
    // an associated panel (graphic rectangle area) for it.
    Panel imgArea;

    // After binding clntArea as the parent, the geometry of
    // imgArea is all related to clntArea, which means that now
    // (0,0) of imgArea is at the left-top corner to clntArea
    // instead of the left-top corner to the root main window.
    imgArea.setParent(&clntArea);

    // If the rectangle area size does not match the image size,
    // an automatical bitmap resampling will be performed.
    imgArea.setSize(img.size());
    imgArea.setPosition({ 0, 0 });
    imgArea.setImage(&img);

    // Label, i.e. so-called TextPanel, is used to display
    // a series of static strings on the screen.
    Label textArea;

    textArea.setParent(&clntArea);
    textArea.setSize({ 200, 100 });
    textArea.setPosition({ 400, 100 });
    textArea.setOutlineWidth(5);
    textArea.setOutlineColor({ 0, 0, 0 });
    textArea.setOutlineOpacity(1.0f);
    textArea.setText(L"This is a label");
    textArea.setHorzAlign(Label::HCenter);

    // Font is also an object, of course.  For performance reasons,
    // the library only loads some limited default fonts when
    // initialized, so we must load the specific fonts manually.
    Font::load(
        // alias for query
        L"MyFont",
        // font properties
        L"Times New Roman",
        20,
        L"en-us",
        Font::Regular,
        Font::Italic,
        Font::Medium);

    // To create a font object, we actually query and load it from
    // the global font map maintained by the library.
    textArea.setFont(Font(L"MyFont"));

    Label busyArea;
    busyArea.setParent(&clntArea);
    busyArea.setSize({ 760, 240 });
    busyArea.setPosition({ 20, 300 });
    busyArea.setFrgnOpacity(0.5f);
    busyArea.setBkgnColor({ 0, 0, 0 });
    busyArea.setBkgnOpacity(0.5f);
    busyArea.setText(L"Try moving cursor in this area");
    busyArea.setHorzAlign(Label::HCenter);

    // We can simply update the existing font of the label instead of
    // loading a complete one if only few properties are changed.
    busyArea.setFontSize(20);

    //------------------------------------------- Set UI event callacks.

    // The event callback of Panel is also an object (i.e. functor).
    // Thus, we can assign a lambda or a function as the callback.
    auto setBusyCursor = [](Panel* p, MouseMoveEvent* e)
    {
        auto cursor = Application::app()->cursor();
        cursor->setIcon(Cursor::Busy);
    };
    busyArea.callback().onMouseMove = setBusyCursor;

    return app.run();
}
