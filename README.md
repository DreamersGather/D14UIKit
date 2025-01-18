# D14UIKit

[![Static Badge](https://img.shields.io/badge/C++-20-white)](https://en.cppreference.com/w/cpp/20)
[![Static Badge](https://img.shields.io/badge/Direct-3D12_|_2D1-purple)](https://github.com/microsoft/DirectX-Graphics-Samples)
[![Static Badge](https://img.shields.io/badge/Python-_3.10_|_3.11_|_3.12_|_3.13_-FFD343?labelColor=3776AB)](https://www.python.org/)
[![Static Badge](https://img.shields.io/badge/pybind-11-E7E0CD?labelColor=765B2F)](https://github.com/pybind/pybind11)

![Build Status](https://img.shields.io/github/actions/workflow/status/DreamersGather/D14UIKit/msbuild.yml)
[![GitHub Release](https://img.shields.io/github/v/release/DreamersGather/D14UIKit)](https://github.com/DreamersGather/D14UIKit/releases)
[![Documentation Status](https://readthedocs.org/projects/d14uikit/badge/?version=latest)](https://d14std.io/projects/uikit/en/latest/?badge=latest)
![GitHub Repo stars](https://img.shields.io/github/stars/DreamersGather/D14UIKit)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/DreamersGather/D14UIKit/total)

<img src="https://media.githubusercontent.com/media/DreamersGather/D14UIKit.Res/main/logo.png" height="41"/>

D14UIKit is a GUI development library for modern Windows platform.

## Getting Started

First, download the latest development package for Python3:

* d14uikit_python.zip
  * demo
  * D14UIKit.pyd
  * D14UIKit.pyi
  * HelloWindow.py (You need to create this yourself)

Then, write your Python3 code (HelloWindow.py) as follows:

```python
# Preferably Python 3.10.x and later versions.
from D14UIKit import *
app = Application('HelloWindow')
mwnd = MainWindow('HelloWindow')
exit(app.run())
```

Finally, run `python3 HelloWindow.py`, and you're all set!

<img src="https://media.githubusercontent.com/media/DreamersGather/D14Docs.Res/main/d14uikit/tutorials/hello_window.png"/>

If you are familiar with C++/Python3 development and want to build your own GUI application on modern Windows platform, D14UIKit is a great choice: it is based on DirectX 12 and Direct2D for high-performance rendering, provides a classic OOP-style UI framework, and more. If you want to learn details, feel free to visit the project website at https://d14std.io/projects/uikit 😎👍

## How It Works

We know that:

* The [Qt](https://www.qt.io/)-like framework constructs application through UI objects and handles events.

* The [imgui](https://github.com/ocornut/imgui)-like framework implements a render-loop where all draws and events are processed in a single flow.

So, which one is D14UIKit? The answer is that **D14UIKit supports both**. It is a **hybrid** architecture UI framework: when dealing with complex business logic, you can create and manage UI objects based on the OOP style, and control the application switch between ① **Asynchronous Waiting** and ② **Immediate Processing** through the `animState` variable.

## Features

* C++20 and Python 3 make it more elegant:

  ```cpp
  // C++20
  titleInput.callback().onTextChange =
  [&](auto obj, auto text)
  {
      window.setTitle(text);
  };
  ```
  ```python
  # Python 3
  def changeWindowTitle(obj, text):
      window.title = text

  titleInput.f_onTextChange = changeWindowTitle
  ```

* OOP-style makes it more convenient:

  ```python
  # Setup a frame animation
  animArea.frames = [Image(f'{i}.png') for i in range(12)]
  animArea.frameTimeSpan = 0.06
  ```
  Quickly import images using list comprehension.

* Hybrid-Architecture: Switch between run-modes:

  ```python
  # Asynchronous Waiting
  app.animState = False
  # Immediate Processing
  app.animState = True
  ```

* High-Performance: Render based on DirectX 12:

  ```python
  app.fps # for real-time app
  ```

## Roadmap

- [x] Migrate UIKit @ D14Engine to build a basic GUI development framework.
- [x] Implement PyBind wrapping to provide Python3 development interface.
- [x] Set up the basic project website and write the introduction and getting started documentation.
- [ ] Support asynchronous UI event handling.
- [ ] Support interactive development and building of UI (e.g. dynamic creation via Python interpreter).
- [ ] Complete the development documentation (feeling too lazy to do it 😂).
- [ ] Provide high-performance geometric drawing interfaces to help render complex dynamic charts.
- [ ] Support better 2D/3D real-time rendering, introducing programmable rendering pipeline interfaces.
- [ ] Introduce more modern UI appearances, such as advanced animations, blur, and shadows.
- [ ] What's next? 🤔

## About

The "UIKit" in the logo is in Segoe UI font, which is the default font for displaying Western languages on Vista and subsequent versions of Windows.
