from sys import argv

from D14UIKit import *

DEMO_NAME = 'DemoTemplate'

def main():
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    app.minSize = app.size
    app.resizable = True

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)
    centerLayout = GridLayout()
    mwnd.content = centerLayout
    centerLayout.horzCellCount = 2

    layout1 = ConstraintLayout()
    geoInfo = GridLayout.GeoInfo()
    geoInfo.x = Range(0, 1)
    geoInfo.y = Range(0, 1)
    centerLayout.addElement(layout1, geoInfo)

    lbl_c1 = Label('C1')
    geoInfo1 = ConstraintLayout.GeoInfo()
    geoInfo1.keepWidth = False
    geoInfo1.Left.ToLeft = 20
    geoInfo1.Right.ToRight = 20
    geoInfo1.keepHeight = False
    geoInfo1.Top.ToTop = 20
    geoInfo1.Bottom.ToTop = 120
    layout1.addElement(lbl_c1, geoInfo1)
    lbl_c1.bkgnColor = Color(0, 255, 0)
    lbl_c1.bkgnOpacity = 0.5
    lbl_c1.horzAlign = Label.HCenter

    lbl_c2 = Label('C2')
    lbl_c2.height = 400
    geoInfo1 = ConstraintLayout.GeoInfo()
    geoInfo1.keepWidth = False
    geoInfo1.Left.ToLeft = 20
    geoInfo1.Right.ToRight = 20
    geoInfo1.keepHeight = True
    geoInfo1.Bottom.ToBottom = 20
    layout1.addElement(lbl_c2, geoInfo1)
    lbl_c2.bkgnColor = Color(0, 255, 255)
    lbl_c2.bkgnOpacity = 0.5
    lbl_c2.horzAlign = Label.HCenter

    layout2 = GridLayout()
    geoInfo = GridLayout.GeoInfo()
    geoInfo.x = Range(1, 1)
    geoInfo.y = Range(0, 1)
    centerLayout.addElement(layout2, geoInfo)
    layout2.horzCellCount = 4
    layout2.vertCellCount = 4
    layout2.horzMargin = 20
    layout2.vertMargin = 20

    lbl_g1 = Label('G1')
    geoInfo2 = GridLayout.GeoInfo()
    geoInfo2.x = Range(0, 1)
    geoInfo2.y = Range(0, 1)
    layout2.addElement(lbl_g1, geoInfo2)
    lbl_g1.bkgnColor = Color(255, 0, 0)
    lbl_g1.bkgnOpacity = 0.5
    lbl_g1.horzAlign = Label.HCenter

    lbl_g2 = Label('G2')
    geoInfo2 = GridLayout.GeoInfo()
    geoInfo2.x = Range(1, 3)
    geoInfo2.y = Range(0, 2)
    layout2.addElement(lbl_g2, geoInfo2)
    lbl_g2.bkgnColor = Color(0, 255, 0)
    lbl_g2.bkgnOpacity = 0.5
    lbl_g2.horzAlign = Label.HCenter

    lbl_g3 = Label('G3')
    geoInfo2 = GridLayout.GeoInfo()
    geoInfo2.x = Range(0, 1)
    geoInfo2.y = Range(1, 3)
    geoInfo2.spacing.top = 10
    geoInfo2.spacing.right = 10
    layout2.addElement(lbl_g3, geoInfo2)
    lbl_g3.bkgnColor = Color(255, 0, 255)
    lbl_g3.bkgnOpacity = 0.5
    lbl_g3.horzAlign = Label.HCenter

    lbl_g4 = Label('G4')
    lbl_g4.size = Size(200, 200)
    geoInfo2 = GridLayout.GeoInfo()
    geoInfo2.fixedSize = True
    geoInfo2.x = Range(1, 3)
    geoInfo2.y = Range(2, 2)
    layout2.addElement(lbl_g4, geoInfo2)
    lbl_g4.bkgnColor = Color(0, 255, 255)
    lbl_g4.bkgnOpacity = 0.5
    lbl_g4.horzAlign = Label.HCenter

    #------------------------------------------- Set UI event callbacks.

    exit(app.run())

if __name__ == '__main__':
    main()
