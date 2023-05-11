from sys import argv

from D14UIKit import \
    Application, \
    MainWindow

DEMO_NAME = 'DemoTemplate'

if __name__ == '__main__':
    dpi = 96.0
    if len(argv) >= 2:
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    mwnd = MainWindow()
    exit(app.run())
