from sys import argv

from D14UIKit import *

DEMO_NAME = 'AdvancedCallback'

if __name__ == '__main__':
    dpi = 96.0
    if len(argv) >= 2:
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)

    #------------------------------------------- Set UI event callacks.

    exit(app.run())
