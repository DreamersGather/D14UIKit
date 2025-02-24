from sys import argv

from D14UIKit import *

DEMO_NAME = 'ModernTheme'

def main():
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    app.height = 300
    app.minSize = app.size
    app.resizable = True

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)
    centerLayout = ConstraintLayout()
    mwnd.content = centerLayout

    themeLbl = Label('Theme mode')
    themeLbl.height = 40
    themeLbl.horzAlign = Label.HCenter
    geoInfo = ConstraintLayout.GeoInfo()
    geoInfo.keepWidth = False
    geoInfo.Left.ToLeft = 50
    geoInfo.Right.ToLeft = 350
    geoInfo.keepHeight = True
    geoInfo.Top.ToTop = 100
    centerLayout.addElement(themeLbl, geoInfo)

    themeSelector = ComboBox()
    themeSelector.height = 40
    geoInfo = ConstraintLayout.GeoInfo()
    geoInfo.keepWidth = False
    geoInfo.Left.ToRight = 350
    geoInfo.Right.ToRight = 50
    geoInfo.keepHeight = True
    geoInfo.Top.ToTop = 100
    centerLayout.addElement(themeSelector, geoInfo)
    themeSelector.roundRadius = 5

    menu = themeSelector.dropDownMenu

    items = [ComboBoxItem() for i in range(3)]
    items[0].text = 'Light'
    items[1].text = 'Dark'
    items[2].text = 'Use system setting'

    menu.appendItem(items)

    menu.width = themeSelector.width
    menu.height = len(items) * 40
    menu.roundExtension = 5

    themeSelector.setCurrSelected(2)

    #------------------------------------------- Set UI event callbacks.

    def changeThemeMode(obj, text):
        app = Application.app
        if text == 'Light' or text == 'Dark':
            style = app.themeStyle
            style.name = text
            app.themeMode = style
        elif text == 'Use system setting':
            app.useSystemTheme = True

    themeSelector.f_onSelectedChange = changeThemeMode

    exit(app.run())

if __name__ == '__main__':
    main()
