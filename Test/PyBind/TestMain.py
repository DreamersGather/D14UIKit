from D14UIKit import *

app = Application()
app.resizable = True
app.clearType = True
app.drawTextNatrualSymmetric = True

ico = Image('test.png')

p = Panel()

pp = Panel()
pp.parent = p;
pp.width = 256;
pp.height = 256;
pp.image = ico;

def pp_onMouseMove(p, e):
    Application.app.cursor.setIcon(Cursor.Busy)
pp.f_onMouseMove = pp_onMouseMove

note = Label()
note.parent = p
note.width = 256
note.height = 60
note.x = 0
note.y = 256
note.text = 'dragon'
note.font = Font('Default/Bold/16')
note.horzAlign = Label.HCenter

btn1 = FlatButton()
btn1.parent = p
btn1.x = 300
btn1.y = 40
btn1.width = 240
btn1.height = 60
btn1.roundRadius = 5
btn1.icon = ico
btn1.iconSize = Size(32, 32)
btn1.text = 'Flat Button'

def btn1_onMouseEnter(btn, e):
    btn.text = 'Gotcha!'
btn1.f_onMouseEnter = btn1_onMouseEnter

def btn1_onMouseLeave(btn, e):
    btn.text = 'No idea.'
btn1.f_onMouseLeave = btn1_onMouseLeave

btn2 = FilledButton()
btn2.parent = p
btn2.x = 300
btn2.y = 140
btn2.width = 240
btn2.height = 60
btn2.roundRadius = 5
btn2.icon = ico
btn2.iconSize = Size(32, 32)
btn2.text = 'Filled Button'

def btn2_onMouseButton(btn, e):
    if e.leftDblclk:
        btn.text = 'Left'
    elif e.rightDblclk:
        btn.text = 'Right'
    elif e.middleDblclk:
        btn.text = 'Middle'
btn2.f_onMouseButton = btn2_onMouseButton

btn3 = OutlinedButton()
btn3.parent = p
btn3.x = 300
btn3.y = 240
btn3.width = 240
btn3.height = 60
btn3.roundRadius = 5
btn3.icon = ico
btn3.iconSize = Size(32, 32)
btn3.text = 'Outlined Button'

def btn3_onKeyboard(btn, e):
    if e.pressed:
        if e.vkey in range(0x41, 0x5a) or \
           e.vkey in range(0x61, 0x7a):
            btn.text = bytes([e.vkey]).decode('ascii')
btn3.f_onKeyboard = btn3_onKeyboard

btn4 = ElevatedButton()
btn4.parent = p
btn4.x = 300
btn4.y = 340
btn4.width = 240
btn4.height = 60
btn4.roundRadius = 5
btn4.icon = ico
btn4.iconSize = Size(32, 32)
btn4.text = 'Elevated Button'

def btn4_onMouseButtonRelease(btn, e):
    if e.left:
        btn.text = 'Left'
    elif e.right:
        btn.text = 'Right'
    elif e.middle:
        btn.text = 'Middle'
btn4.f_onMouseButtonRelease = btn4_onMouseButtonRelease

btn5 = ToggleButton()
btn5.parent = p
btn5.x = 300
btn5.y = 440
btn5.width = 240
btn5.height = 60
btn5.roundRadius = 5
btn5.icon = ico
btn5.iconSize = Size(32, 32)
btn5.text = 'Toggle Button'

def btn5_onStateChange(btn, state):
    if state == ToggleButton.Activated:
        btn.text = 'Activated'
    elif state == ToggleButton.Deactivated:
        btn.text = 'Deactivated'
btn5.f_onStateChange = btn5_onStateChange

w = MainWindow()
w.content = p

def w_onClose(w):
    Application.app.exit()
w.f_onClose = w_onClose

exit(app.run())
