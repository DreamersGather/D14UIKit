#include "Application.h"
#include "Callback.h"
#include "FlatButton.h"
#include "MainWindow.h"
#include "TextBox.h"

using namespace d14uikit;

#define DEMO_NAME L"BasicCallback"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setHeight(380);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);
    Panel clntArea;
    mwnd.setContent(&clntArea);

    TextBox titleInput;
    titleInput.setParent(&clntArea);
    titleInput.setSize({ 400, 50 });
    titleInput.setPosition({ 200, 100 });
    titleInput.setRoundRadius(5);
    titleInput.setTextRect({ 10, 5, 390, 45 });

    auto placer = titleInput.placeholder();
    placer->setText(L"Input window title...");

    FlatButton restoreButton;
    restoreButton.setParent(&clntArea);
    restoreButton.setSize({ 200, 50 });
    restoreButton.setPosition({ 300, 200 });
    restoreButton.setRoundRadius(5);
    restoreButton.setText(L"Restore default");

    //------------------------------------------- Set UI event callbacks.

    // We can use the macros in Callback.h to avoid
    // writting verbose lambda signatures manually.
    // The last variable arguments __VA_ARGS__ should
    // be the capture-list of the actual lambda.

    //titleInput.callback().onTextChange =
    //[&](RawTextInput* obj, const std::wstring& text)
    titleInput.D14_onTextChange(obj, text, &)
    {
        mwnd.setTitle(text);
    };

    //restoreButton.callback().onMouseButtonRelease =
    //[&](ClickablePanel* clkp, MouseButtonClickEvent* e)
    restoreButton.D14_onMouseButtonRelease(clkp, e, &)
    {
        // If we update mwnd before titleInput,
        // onTextChange callback will overwrite
        // the window title as empty at the end.
        titleInput.setText(L"");
        mwnd.setTitle(DEMO_NAME);
    };

    return app.run();
}
