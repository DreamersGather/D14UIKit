from sys import argv

from D14UIKit import *

DEMO_NAME = 'ObjectOriented'

def main():
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)

    #------------------------------------------- Initialize UI objects.

    # The main window is also a Panel, whose rectangle area
    # is divided into 2 parts: non-client and client area.
    # The non-client area consists of the caption panel and
    # the decorative bar; the center part is client area.
    #
    # It is worth noting that there is also a sizing frame
    # around the window (inherited from ResizablePanel),
    # which should also be a part of the non-client area,
    # but that is not the theme of this demostration app.
    mwnd = MainWindow(DEMO_NAME)

    # Create a Panel object, which is the most derived UI object
    # in D14UIKit.  Basically, a Panel is a graphic rectangle
    # area on the screen, where we can draw an image as background
    # or place other UI objects as children.
    clntArea = Panel()

    # Make the panel fills the client area of the main window.
    mwnd.content = clntArea

    # Image is also an object indeed, although not a UI object.
    img = Image('images/test.png')

    # To display the image on the screen, we need to create
    # an associated panel (graphic rectangle area) for it.
    imgArea = Panel()

    # After binding clntArea as the parent, the geometry of
    # imgArea is all related to clntArea, which means that now
    # (0,0) of imgArea is at the left-top corner to clntArea
    # instead of the left-top corner to the root main window.
    imgArea.parent = clntArea

    # If the rectangle area size does not match the image size,
    # an automatical bitmap resampling will be performed.
    imgArea.size = img.size
    imgArea.position = Point(20, 0)
    imgArea.image = img

    # Label, i.e. so-called TextPanel, is used to display
    # a series of static strings on the screen.
    textArea = Label()

    textArea.parent = clntArea
    textArea.size = Size(200, 100)
    textArea.position = Point(400, 100)
    textArea.outlineWidth = 5
    textArea.outlineColor = Color(255, 0, 0)
    textArea.outlineOpacity = 0.5
    textArea.text = 'This is a label'
    textArea.horzAlign = Label.HCenter

    # Font is also an object, of course.  For performance reasons,
    # the library only loads some limited default fonts when
    # initialized, so we must load the specific fonts manually.
    font1 = Font.load( \
        # alias for query
        "MyFont", \
        # font properties
        "Times New Roman", \
        20, \
        "en-us", \
        Font.ExtraBold, \
        Font.Italic, \
        Font.Expanded)

    # To create a font object, we actually query and load it from
    # the global font map maintained by the library.
    font2 = Font('MyFont')

    # Both methods are acceptable, but it is recommended to
    # reference the font directly through Font(name) after loading,
    # because Font.load will fetch and cache the actual font data,
    # while Font(name) only references the cache, which is faster.
    textArea.font = font1
    #textArea.font = font2

    busyArea = Label()
    busyArea.parent = clntArea
    busyArea.size = Size(760, 240)
    busyArea.position = Point(20, 300)
    busyArea.bkgnColor = Color(128, 128, 128)
    busyArea.bkgnOpacity = 0.5
    busyArea.text = 'Try moving cursor in this area'
    busyArea.horzAlign = Label.HCenter

    # We can simply update the existing font of the label instead of
    # loading a complete one if only few properties are changed.
    busyArea.fontSize = 20

    #------------------------------------------- Set UI event callbacks.

    def setBusyCursor(p, e):
        cursor = Application.app.cursor
        cursor.setIcon(Cursor.Busy)

    # The event callback of Panel is also an object (i.e. functor).
    # Thus, we can assign a lambda or a function as the callback.
    busyArea.f_onMouseMove = setBusyCursor

    exit(app.run())

if __name__ == '__main__':
    main()
