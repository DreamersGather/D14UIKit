from D14UIKit import *

DEMO_NAME = 'HelloWindow'

def main():

    app = Application(DEMO_NAME)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)

    #------------------------------------------- Set UI event callbacks.

    exit(app.run())

if __name__ == '__main__':
    main()
