# LightCombatManager
LightCombatManager is a small combat manager for DnD-like games, primarily for Pathfinder 1e. 

# Tools
LightCombatManager is written in C++14. The user interface is created with QT5 [1]. The code formatting is done using Uncrustify. [2]

# Installation

## Linux

`cmake` and `QT5` are required to build this application. QT on Ubuntu can be installed with `qt5-default`, for Manjaro or other Arch-based distributions install `qt5-base`. `Uncrustify` is also included in most Linux distro packages. \
For Windows, there are both Pre-Builds for CMake, QT5 and Uncrustify.

1. Download or clone this repository.
2. Open a terminal and `cd` into this repository.
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Hit `cmake ..` and then `make`. Start the application with `./LightCombatManager`.

## Windows

There are multiple ways to build this application on Windows. The following description is focussed on building the application with `cmake` and Visual Studio 2019. QT5 must be installed on the system.

1. Download or clone this repository.
2. Open up `cmd` or `git bash` and navigate to this repository. 
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Set the CMake Prefix Path for `Qt` inside the `CMakeLists.txt`. Example: `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.1\\msvc2019_64\\")` for `Qt 5.15` and `msvc compiler`
5. Hit `cmake.exe -G “Visual Studio 16 2019” -A x64 ..`. This will create a `sln`-file. 
6. Open the sln file using Visual Studio. Build the application in Debug- or Release-Mode.
7. Inside the `build`-file, a file named `Debug` or `Release` should have been created. Navigate to this folder.
8. The `exe`-file for the application should be found here. To successfully run the exe, `qwindows.dll`, `QT5Core.dll`, `QT5Widgets.dll` and `QT5Gui.dll` 
   have to be in the same file path as the exe (`QT5Cored.dll`, `QT5Widgetsd.dll` and `QT5Guid.dll` for Debug).
   
# Contribution

To contribute, please format your code using the provided `uncrustify.cfg`. 

[1] https://www.qt.io/ \
[2] https://github.com/uncrustify/uncrustify
