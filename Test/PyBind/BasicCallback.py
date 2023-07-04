from sys import argv

from D14UIKit import *

DEMO_NAME = 'BasicCallback'

if __name__ == '__main__':
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    app.height = 380

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)
    clntArea = Panel()
    mwnd.content = clntArea

    titleInput = TextBox()
    titleInput.parent = clntArea
    titleInput.size = Size(400, 50)
    titleInput.position = Point(200, 100)
    titleInput.roundRadius = 5
    titleInput.textRect = Rect(10, 5, 390, 45)

    placer = titleInput.placeholder
    placer.text = 'Input window title...'

    restoreButton = FlatButton()
    restoreButton.parent = clntArea
    restoreButton.size = Size(200, 50)
    restoreButton.position = Point(300, 200)
    restoreButton.roundRadius = 5
    restoreButton.text = 'Restore default'

    #------------------------------------------- Set UI event callacks.

    # The simplest way to designate an event callback is
    # binding a specific function to the functor property.

    def changeMwndTitle(obj, text):
        mwnd.title = text
    titleInput.f_onTextChange = changeMwndTitle

    def restoreMwndTitle(clkp, e):
        # If we update mwnd before titleInput,
        # onTextChange callback will overwrite
        # the window title as empty at the end.
        titleInput.text = ''
        mwnd.title = DEMO_NAME
    restoreButton.f_onMouseButtonRelease = restoreMwndTitle

    exit(app.run())
