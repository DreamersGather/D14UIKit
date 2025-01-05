from sys import argv

from D14UIKit import *

DEMO_NAME = 'FrameAnimation'

if __name__ == '__main__':
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    app.size = Size(300, 336)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)
    mwnd.minimizeButton = False
    mwnd.maximizeButton = False

    clntArea = Panel()
    mwnd.content = clntArea

    animArea = FrameAnimPanel()
    animArea.parent = clntArea
    animArea.position = Point(22, 22)
    animArea.size = Size(256, 256)

    animArea.frames = [Image(f'images/stick_boy/{i}.png') for i in range(12)]
    # Applies point-sampler to make the pixel art clear.
    animArea.frameInterpMode = BitmapInterpMode.NearestNeighbor
    animArea.frameTimeSpan = 0.06

    # Notifies the renderer to draw frames continuously.
    animArea.animState = True

    fpsLbl = Label('FPS: None')
    fpsLbl.parent = clntArea
    fpsLbl.position = Point(22, 22)

    #------------------------------------------- Set UI event callbacks.

    # This will be called before the renderer drawing each frame.
    def displayFPS(p):
        if app.fps != displayFPS.fps:
            displayFPS.fps = app.fps
            p.text = f'FPS: {displayFPS.fps}'

    displayFPS.fps = 0
    fpsLbl.f_onUpdate = displayFPS

    exit(app.run())
