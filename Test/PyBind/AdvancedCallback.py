from sys import argv

from D14UIKit import *

DEMO_NAME = 'AdvancedCallback'

if __name__ == '__main__':
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)
    clntArea = Panel()
    mwnd.content = clntArea

    board = TabGroup()
    board.parent = clntArea
    board.size = Size(800, 436)
    board.position = Point(0, 48)
    board.cardSize = Size(280, 32)

    caption = TabCaption('Press left button to draw')

    caption.closable = False
    caption.draggable = False

    # We will simulate drawing by copying pixels
    # to the background image of the canvas panel.
    bkgn = Image(board.width, board.height)

    canvas = Panel()
    canvas.image = bkgn

    tab = TabGroup.Tab()
    tab.caption = caption
    tab.content = canvas
    board.appendTab(tab)

    board.setCurrSelected(0)

    modeSelector = ToggleButton('Pencil')
    modeSelector.parent = clntArea
    modeSelector.size = Size(160, 56)
    modeSelector.position = Point(12, 496)
    modeSelector.roundRadius = 5

    clearButton = FilledButton('Clear')
    clearButton.parent = clntArea
    clearButton.size = Size(160, 56)
    clearButton.position = Point(184, 496)
    clearButton.roundRadius = 5

    strokeLabel = Label('Stroke width')
    strokeLabel.parent = clntArea
    strokeLabel.size = Size(160, 56)
    strokeLabel.position = Point(356, 496)
    strokeLabel.horzAlign = Label.HCenter

    strokeSlider = HorzSlider()
    strokeSlider.parent = clntArea
    strokeSlider.size = Size(240, 50)
    strokeSlider.position = Point(528, 520)
    strokeSlider.minValue = 1.0
    strokeSlider.maxValue = 32.0
    strokeSlider.stepMode = HorzSlider.Discrete
    strokeSlider.stepInterval = 1.0
    strokeSlider.vlabelResident = True
    strokeSlider.vlabelRectSize = Size(48, 32)

    # It won't work by calling setFontSize here
    # since setText will discard the temporary layout.
    # The solution is replacing the font totally
    # instead of only changing the size property.
    # (setText is called when slider changes it value).
    vlabel = strokeSlider.valueLabel
    vlabel.font = Font('Default/Normal/14')

    # Call this after other methods to make sure
    # the geometry of the value label is updated.
    strokeSlider.value = 24.0

    #------------------------------------------- Set UI event callbacks.

    # Prepares the copy source to improve performance.
    pencil = list()
    eraser = list()

    color = app.themeColor
    for i in range(32*32):
        pencil.append(Pixel(color.r, color.g, color.b, 255))
        eraser.append(Pixel(0, 0, 0, 0))

    # Define a lambda to reuse the common logic.
    def drawPixels(cursorPoint):

        # Gets the destination point to copy pixels.
        dstPos = Point \
        ( \
            cursorPoint.x - canvas.absX, \
            cursorPoint.y - canvas.absY  \
        )
        # Decides whether to use Pencil or Eraser.
        source = pencil

        state = modeSelector.state
        if state == ToggleButton.Activated:
            source = eraser

        # Calculates the actual area to apply tools.
        value = int(strokeSlider.value)
        halfLess = value // 2
        halfMore = (value + 1) // 2

        # left-closed and right-oepn
        dstRect = Rect \
        ( \
            dstPos.x - halfLess, dstPos.y - halfLess, \
            dstPos.x + halfMore, dstPos.y + halfMore  \
        )
        # Make sure the destination rect is valid.
        w = bkgn.width + 1
        h = bkgn.height + 1

        dstRect.left = max(dstRect.left, 0)
        dstRect.right = min(dstRect.right, w)
        dstRect.top = max(dstRect.top, 0)
        dstRect.bottom = min(dstRect.bottom, h)

        bkgn.copy(dstRect, source)

    def holdMoveToDrawPixels(p, e):
        app.cursor.setIcon(Cursor.Pen)
        # lbutton is a method, not a property!
        # It is actually static Event.lbutton.
        if e.lbutton():
            drawPixels(e.cursorPoint)

    canvas.f_onMouseMove = holdMoveToDrawPixels

    def pressToDrawPixels(p, e):
        # leftDown is a property, not a method!
        if e.leftDown:
            drawPixels(e.cursorPoint)

    canvas.f_onMouseButton = pressToDrawPixels

    def changeText(obj, state):
        if state == ToggleButton.Activated:
            obj.text = 'Eraser'
        elif state == ToggleButton.Deactivated:
            obj.text = 'Pencil'

    modeSelector.f_onStateChange = changeText

    def clearCanvas(clkp, e):
        # There are two feasible ways to clear an image:
        # 1. Create a blank buffer on the CPU sider.
        # 2. Create a blank bitmap on the GPU sider.
        # and finally call the corresponding copy method.
        # The demo uses the second way, and you can
        # try the first way to get another experience.

        w = bkgn.width
        h = bkgn.height

        blank = Image(w, h)
        bkgn.copy(Point(0, 0), blank, Rect(0, 0, w, h ))

    clearButton.f_onMouseButtonRelease = clearCanvas

    exit(app.run())
