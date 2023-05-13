from D14UIKit import *

DEMO_NAME = 'HelloWindow'

if __name__ == '__main__':

    app = Application(DEMO_NAME)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)

    #------------------------------------------- Set UI event callacks.

    exit(app.run())
