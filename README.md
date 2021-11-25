# LightCombatManager
LightCombatManager is a small combat manager for DnD-like games, primarily for Pathfinder 1e. \
Using LightCombatManager, you can easily create Characters with initiative values, a HP counter and additional information. You can also add status effects to each Character. After creation, the program automatically sorts all Characters and creates a full Combat Table. Inside this Table, you can easily delete and readd Characters or change the Combat Order by moving Characters around. The program will track the current Character and the round number.\
If the current Combat is not finished yet, you can save the Table as a csv file and reopen it later. All Characters, the current Character and the round number will be stored.

# Tools
LightCombatManager is written in C++17. The following tools are used for development:
* [Qt5](https://www.qt.io/) for the user interface
* [Catch2](https://github.com/catchorg/Catch2) for Unit testing ([Catch2 license](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
* [Uncrustify](https://github.com/uncrustify/uncrustify) for code formatting.
* CMake as build system.

# Installation

Qt5, CMake and Catch2 are required to build this application. For most Linux Distros, all these libraries should be contained using the official package managers. 
For Windows, installers for Qt5, CMake and Catch2 are available.

## Linux

1. Download or clone this repository.
2. Open a terminal and `cd` into this repository.
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Hit `cmake ..` and then `make`. Start the application with `./LightCombatManager`.

## Windows

The following description is focused on building the application with `cmake` and Visual Studio 2019.

1. Download or clone this repository.
2. Open up `cmd` or `git bash` and navigate to this repository. 
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Set the CMake Prefix Path for `Qt` inside the `CMakeLists.txt`. Example: `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.1\\msvc2019_64\\")` for `Qt 5.15` and `msvc compiler`
5. Hit `cmake.exe -G “Visual Studio 16 2019” -A x64 ..`. This will create an `sln`-file. 
6. Open the sln file using Visual Studio. Build the application in Debug- or Release-Mode.
7. Inside the `build`-file, a file named `Debug` or `Release` should have been created. Navigate to this folder.
8. The `exe`-file for the application should be found here. To successfully run the exe, `qwindows.dll`, `QT5Core.dll`, `QT5Widgets.dll` and `QT5Gui.dll` 
   have to be in the same file path as the exe (`QT5Cored.dll`, `QT5Widgetsd.dll` and `QT5Guid.dll` for Debug).
   
# Contribution

To contribute, please format your code using the provided `uncrustify.cfg`. Uncrustify can be found in most Linux Distro package managers. 
For Windows, there are installers available.
