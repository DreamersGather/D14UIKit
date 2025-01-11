# D14UIKit

[![Documentation Status](https://readthedocs.org/projects/d14uikit/badge/?version=latest)](https://d14std.io/projects/uikit/en/latest/?badge=latest)

<img src="https://raw.githubusercontent.com/yiyaowen/D14Engine.Docs.Img/main/d14uikit/logo.png" height="82"/>

D14UIKit is a GUI development library for modern Windows platform.

## Getting Started

First, download the latest development package for Python3:

* d14uikit_python.zip
  * demo
  * D14UIKit.pyd
  * D14UIKit.pyi
  * HelloWindow.py (You need to create this yourself)

Then, write your Python3 code (HelloWindow.py) as follows:

```python3
# Preferably Python 3.10.x and later versions.
from D14UIKit import *
app = Application('HelloWindow')
mwnd = MainWindow('HelloWindow')
exit(app.run())
```

Finally, run `python3 HelloWindow.py`, and you're all set!

<img src="https://media.githubusercontent.com/media/DreamersGather/D14Docs.Res/main/d14uikit/tutorials/hello_window.png">

If you are familiar with C++/Python3 and want to build your own GUI application, D14UIKit is a great choice: it is based on DirectX 12 and Direct2D for high-performance rendering, provides a classic OOP-style UI framework, and more. If you want to learn details, feel free to visit the project website at https://d14std.io/projects/uikit 😎👍

## Roadmap

- [x] Migrate UIKit @ D14Engine to build a basic GUI development framework.
- [x] Implement PyBind wrapping to provide Python3 development interface.
- [x] Set up the basic project website and write the introduction and getting started documentation.
- [ ] Support asynchronous UI event handling.
- [ ] Complete the development documentation (feeling too lazy to do it 😂).
- [ ] Provide high-performance geometric drawing interfaces to help render complex dynamic charts.
- [ ] Support better 2D/3D real-time rendering, introducing programmable rendering pipeline interfaces.
- [ ] Introduce more modern UI appearances, such as advanced animations, blur, and shadows.
- [ ] What's next? 🤔

## More

D14Project series:

* [D14Engine](https://github.com/DreamersGather/D14Engine): Game Engine based on DirectX 12
* [D14UIKit](https://github.com/DreamersGather/D14UIKit): GUI library from UIKit @ D14Engine

## About

The "UIKit" in the logo is in Segoe UI font, which is the default font for displaying Western languages on Vista and subsequent versions of Windows.
