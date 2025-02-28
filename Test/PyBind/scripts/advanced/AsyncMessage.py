###############
# D14 WARNING #
###############
# Due to the presence of the GIL (Global Interpreter Lock) in CPython,
# this demo cannot correctly reflect the behavior of thread-event callbacks!!!
# The good news is that CPython plans to optimize the GIL in the future;
# in Python 3.13, this feature has experimentally become optional: https://peps.python.org/pep-0703/.
# You can try using GIL-free Python for better Thread! (though stability issues may exist).

from sys import argv
from threading import Thread, Lock
from time import sleep

from D14UIKit import *

DEMO_NAME = 'AsyncMessage'

# These variables need to be promoted to global definition;
# otherwise, the child thread cannot locate them.
uiLockNum = 0
mxLockNum = 0

def main():
    dpi = 96.0
    if len(argv) >= 2 and argv[1] == 'HighDPI':
        dpi = 192.0

    app = Application(DEMO_NAME, dpi)
    app.size = Size(550, 186)

    #------------------------------------------- Initialize UI objects.

    mwnd = MainWindow(DEMO_NAME)
    
    clntArea = Panel()
    mwnd.content = clntArea

    uiLockBtn = FilledButton()
    uiLockBtn.parent = clntArea
    uiLockBtn.size = Size(200, 50)
    uiLockBtn.position = Point(50, 50)
    uiLockBtn.roundRadius = 5
    uiLockBtn.text = 'Lock # UI: 0'

    mxLockBtn = FilledButton()
    mxLockBtn.parent = clntArea
    mxLockBtn.size = Size(200, 50)
    mxLockBtn.position = Point(300, 50)
    mxLockBtn.roundRadius = 5
    mxLockBtn.text = 'Lock # mutex: 0'

    #------------------------------------------- Set UI event callbacks.

    # To avoid race conditions in multithreading,
    # it is necessary to lock read/write operations.
    # Below are commonly used methods:
    #
    # 1. Lock with UI
    #
    # Locking by managing UI controls.
    # This approach can address risks caused by user operations,
    # but cannot avoid potential race conditions within the program.
    #
    # 2. Lock with mutex
    #
    # Locking using mutex objects.
    # This is the standard locking method for multithreading,
    # and requires careful design by developers.
    #
    # Developers should choose based on the specific situation,
    # or use both methods simultaneously,
    # ensuring that race conditions are avoided.

    ###################
    # 1. Lock with UI #
    ###################

    # This variable needs to be promoted to global definition;
    # otherwise, the child thread cannot locate it.
    # uiLockNum = 0

    # To implement a callback function on the UI main thread
    # triggered by an event in a child thread, it is necessary to
    # bind a uint64_t ID to the thread event and the callback.
    #
    # This value determines the mapping
    # between the thread event and its corresponding callback,
    # In C++, we can choose the address of the variable as the ID,
    # which is guaranteed to be globally unique).
    #
    # However, in Python, integer variables may be cached and
    # point to the same address, so the id(x) function cannot be used;
    # here, we manually define different IDs for thread events.
    uiLockThreadID = 1

    # Bad Idea !!!
    # uiLockThreadID = id(uiLockNum)

    def uiLockThread():
        global uiLockNum

        sleep(3)

        uiLockNum += 1

        # Call setThreadEvent in other threads to send messages,
        # and the corresponding callbacks will be called afterward.
        # Note that even though we cannot know the exact time when
        # the callbacks are called, the order can be guaranteed.
        app.setThreadEvent(uiLockThreadID)

    def uiLockTest(clkp, e):
        # lock
        uiLockBtn.enabled = False

        # start working thread
        Thread(target=uiLockThread, daemon=True).start()

    uiLockBtn.f_onMouseButtonRelease = uiLockTest

    def uiLockCallback():
        # unlock
        uiLockBtn.enabled = True

        uiLockBtn.text = f'Lock # UI: {uiLockNum}'

    app.addThreadCallback(uiLockThreadID, uiLockCallback)

    ######################
    # 2. Lock with mutex #
    ######################

    # This variable needs to be promoted to global definition;
    # otherwise, the child thread cannot locate it.
    # mxLockNum = 0
    mxLockThreadID = 2

    # Bad Idea !!!
    # mxLockThreadID = id(mxLockNum)

    mx = Lock()

    def mxLockThread():
        global mxLockNum

        sleep(3)

        # mutex block
        with mx:
            mxLockNum += 1

        app.setThreadEvent(mxLockThreadID)

    def mxLockTest(clkp, e):
        # start working thread
        Thread(target=mxLockThread, daemon=True).start()

    mxLockBtn.f_onMouseButtonRelease = mxLockTest

    def mxLockCallback():
        mxLockBtn.text = f'Lock # mutex: {mxLockNum}'

    app.addThreadCallback(mxLockThreadID, mxLockCallback)

    exit(app.run())

if __name__ == '__main__':
    main()
