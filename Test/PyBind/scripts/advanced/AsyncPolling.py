from sys import argv
import threading
from threading import Thread
from time import sleep

from D14UIKit import *

DEMO_NAME = 'AsyncPolling'

# These variables need to be promoted to global definition;
# otherwise, the child thread cannot locate them.
curr = 0
done = False

def main():
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    app.size = Size(600, 266)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)

    clntArea = Panel()
    mwnd.content = clntArea

    startBtn = FilledButton()
    startBtn.parent = clntArea
    startBtn.size = Size(200, 50)
    startBtn.position = Point(50, 50)
    startBtn.roundRadius = 5
    startBtn.text ='Start Progress'

    controlBtn = FilledButton()
    controlBtn.parent = clntArea
    controlBtn.size = Size(200, 50)
    controlBtn.position = Point(275, 50)
    controlBtn.roundRadius = 5
    controlBtn.text ='Pause Progress'

    full = 500
    color = app.themeColor

    progressLbl = Label()
    progressLbl.parent = clntArea
    progressLbl.size = Size(50, 50)
    progressLbl.position = Point(500, 50)
    progressLbl.text = f'0/{full}'
    progressLbl.font = Font('Default/Normal/14')
    progressLbl.horzAlign = Label.HCenter

    progressArea = Panel()
    progressArea.parent = clntArea
    progressArea.size = Size(full, 30)
    progressArea.position = Point(50, 150)
    progressArea.outlineWidth = 2
    progressArea.outlineColor = color
    progressArea.outlineOpacity = 0.5
    progressArea.roundRadius = 5

    challengeLbl = Label()
    challengeLbl.parent = progressArea
    challengeLbl.position = Point(10, 0)
    challengeLbl.height = 30
    challengeLbl.text = 'Challenge: Use the Pause/Resume button to get a 499/500!'
    challengeLbl.fontSize = 12

    progressBar = Panel()
    progressBar.parent = clntArea
    progressBar.moveAbove(progressArea)
    progressBar.size = Size(0, 30)
    progressBar.position = Point(50, 150)
    progressBar.color = color
    progressBar.opacity = 0.5
    progressBar.roundRadius = 5

    #------------------------------------------- Set UI event callbacks.

    work = threading.Event()
    work.set()

    def workThread():
        global curr, done

        for i in range(1, full+1):
            work.wait()
            sleep(0.002)
            curr = i

        done = True

    def startProgress(clkp, e):
        global curr, done

        if e.left:
            startBtn.enabled = False
            # render continuously for polling
            progressBar.animState = True

            # new round: reset thread state
            curr = 0
            done = False
            progressBar.width = 0

            Thread(target=workThread, daemon=True).start()

    startBtn.f_onMouseButtonRelease = startProgress

    def controlProgress(clkp, e):
        if e.left:
            if work.is_set():
                work.clear()
                controlBtn.text = 'Resume Progress'
            else:
                work.set()
                controlBtn.text = 'Pause Progress'

    controlBtn.f_onMouseButtonRelease = controlProgress

    #############################
    # Polling in each frame !!! #
    #############################

    def updateLbl(p):
        progressLbl.text = f'{curr}/{full}'

    progressLbl.f_onUpdate = updateLbl

    def updateBar(p):
        if done:
            startBtn.enabled = True
            progressBar.animState = False
        else:
            progressBar.width = curr

    progressBar.f_onUpdate = updateBar

    exit(app.run())

if __name__ == '__main__':
    main()
