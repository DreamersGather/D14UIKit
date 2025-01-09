#include <array>

#include "Application.h"
#include "Callback.h"
#include "Cursor.h"
#include "FilledButton.h"
#include "HorzSlider.h"
#include "Image.h"
#include "Label.h"
#include "MainWindow.h"
#include "TabCaption.h"
#include "TabGroup.h"
#include "ToggleButton.h"

using namespace d14uikit;

#define DEMO_NAME L"AdvancedCallback"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);
    Panel clntArea;
    mwnd.setContent(&clntArea);
    
    TabGroup board;
    board.setParent(&clntArea);
    board.setSize({ 800, 436 });
    board.setPosition({ 0, 48 });
    board.setCardSize({ 280, 32 });

    TabCaption caption(L"Press left button to draw");

    caption.setClosable(false);
    caption.setDraggable(false);

    // We will simulate drawing by copying pixels
    // to the background image of the canvas panel.
    Image bkgn(board.width(), board.height());

    Panel canvas;
    canvas.setImage(&bkgn);

    TabGroup::Tab tab;
    tab.caption = &caption;
    tab.content = &canvas;
    board.appendTab(tab);

    board.setCurrSelected(0);

    ToggleButton modeSelector(L"Pencil");
    modeSelector.setParent(&clntArea);
    modeSelector.setSize({ 160, 56 });
    modeSelector.setPosition({ 12, 496 });
    modeSelector.setRoundRadius(5);

    FilledButton clearButton(L"Clear");
    clearButton.setParent(&clntArea);
    clearButton.setSize({ 160, 56 });
    clearButton.setPosition({ 184, 496 });
    clearButton.setRoundRadius(5);

    Label strokeLabel(L"Stroke width");
    strokeLabel.setParent(&clntArea);
    strokeLabel.setSize({ 160, 56 });
    strokeLabel.setPosition({ 356, 496 });
    strokeLabel.setHorzAlign(Label::HCenter);

    HorzSlider strokeSlider;
    strokeSlider.setParent(&clntArea);
    strokeSlider.setSize({ 240, 50 });
    strokeSlider.setPosition({ 528, 520 });
    strokeSlider.setMinValue(1.0f);
    strokeSlider.setMaxValue(32.0f);
    strokeSlider.setStepMode(HorzSlider::Discrete);
    strokeSlider.setStepInterval(1.0f);
    strokeSlider.setVlabelResident(true);
    strokeSlider.setVlabelRectSize({ 48, 32 });

    // It won't work by calling setFontSize here
    // since setText will discard the temporary layout.
    // The solution is replacing the font totally
    // instead of only changing the size property.
    // (setText is called when slider changes it value).
    auto font = Font(L"Default/Normal/14");
    auto vlabel = strokeSlider.valueLabel();
    vlabel->setFont(&font);

    // Call this after other methods to make sure
    // the geometry of the value label is updated.
    strokeSlider.setValue(24.0f);
    
    //------------------------------------------- Set UI event callbacks.

    // Prepares the copy source to improve performance.
    std::array<Pixel, 32*32> pencil, eraser;

    Color color = app.themeColor();
    for (int i = 0; i < 32*32; ++i)
    {
        pencil[i] = // theme color
        {
            (uint8_t)color.r,
            (uint8_t)color.g,
            (uint8_t)color.b,
            255
        };
        eraser[i] = { 0, 0, 0, 0 };
    }
    // Define a lambda to reuse the common logic.
    auto drawPixels = [&](const Point& cursorPoint)
    {
        // Gets the destination point to copy pixels.
        Point dstPos =
        {
            cursorPoint.x - canvas.absX(),
            cursorPoint.y - canvas.absY()
        };
        // Decides whether to use Pencil or Eraser.
        Pixel* source = pencil.data();

        auto state = modeSelector.state();
        if (state == ToggleButton::Activated)
        {
            source = eraser.data();
        }
        // Calculates the actual area to apply tools.
        auto value = (int)strokeSlider.value();
        auto halfLess = value / 2;
        auto halfMore = (value + 1) / 2;

        Rect dstRect = // left-closed and right-oepn
        {
            dstPos.x - halfLess, dstPos.y - halfLess,
            dstPos.x + halfMore, dstPos.y + halfMore
        };
        // Make sure the destination rect is valid.
        int w = bkgn.width() + 1, h = bkgn.height() + 1;

        dstRect.left = std::max(dstRect.left, 0);
        dstRect.right = std::min(dstRect.right, w);
        dstRect.top = std::max(dstRect.top, 0);
        dstRect.bottom = std::min(dstRect.bottom, h);

        bkgn.copy(dstRect, source);
    };
    canvas.D14_onMouseMove(p, e, &)
    {
        app.cursor()->setIcon(Cursor::Pen);

        if (e->lbutton())
        {
            drawPixels(e->cursorPoint());
        }
    };
    canvas.D14_onMouseButton(p, e, &)
    {
        if (e->leftDown())
        {
            drawPixels(e->cursorPoint());
        }
    };
    modeSelector.D14_onStateChange(ToggleButton, obj, state)
    {
        if (state == ToggleButton::Activated)
        {
            obj->setText(L"Eraser");
        }
        else if (state == ToggleButton::Deactivated)
        {
            obj->setText(L"Pencil");
        }
    };
    clearButton.D14_onMouseButtonRelease(clkp, e, &)
    {
        // There are two feasible ways to clear an image:
        // 1. Create a blank buffer on the CPU sider.
        // 2. Create a blank bitmap on the GPU sider.
        // and finally call the corresponding copy method.
        // The demo uses the second way, and you can
        // try the first way to get another experience.

        int w = bkgn.width(), h = bkgn.height();

        Image blank(w, h);
        bkgn.copy({ 0, 0 }, &blank, { 0, 0, w, h });
    };

    return app.run();
}
