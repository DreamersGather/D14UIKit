﻿from sys import argv

from D14UIKit import *

DEMO_NAME = 'DemoTemplate'

def main():
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)

    #------------------------------------------- Set UI event callbacks.

    exit(app.run())

if __name__ == '__main__':
    main()
