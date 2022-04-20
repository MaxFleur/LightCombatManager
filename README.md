# LightCombatManager
LightCombatManager is a small combat manager for D&D-like games. Using LightCombatManager, you can easily create Characters with initiative values, an HP counter and additional information. You can also add status effects to each Character.\
![LCM_Pic2](https://user-images.githubusercontent.com/15110943/164336549-e2a12841-0994-49c5-a53d-ee84ab6657f2.png)


After creation, the program automatically sorts all Characters and creates a full Combat Table. Using this table, you can easily manage the Combat. \
![LCM_Pic1](https://user-images.githubusercontent.com/15110943/164336625-1a716475-dea0-4743-a1bb-66dd9db655aa.png)


If the current Combat is not finished yet, you can save the Table as a csv file and reopen it later. All Characters, the current Character and the round number will be stored.\
LightCombatManager currently supports the following rulesets:
* DND 3.5E
* DND 5E
* Pathfinder 1E
* Pathfinder 2E

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

The following description is focused on building the application with `cmake` and Visual Studio 2022.

1. Download or clone this repository.
2. Open up `cmd` or `git bash` and navigate to this repository. 
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Set the CMake Prefix Path for `Qt` inside the `CMakeLists.txt`. Example: `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.1\\msvc2019_64\\")` for `Qt 5.15` and `msvc compiler`
5. Hit `cmake.exe -G “Visual Studio 17 2022” -A x64 ..`. This will create an `sln`-file. 
6. Open the sln file using Visual Studio. Build the application in Debug- or Release-Mode.
7. Inside the `build`-file, a file named `Debug` or `Release` should have been created. Navigate to this folder.
8. The `exe`-file for the application should be found here. To successfully run the exe, `QT5Core.dll`, `QT5Widgets.dll` and `QT5Gui.dll` 
   have to be in the same file path as the exe (`QT5Cored.dll`, `QT5Widgetsd.dll` and `QT5Guid.dll` for Debug). Also, please create a directory named
   `platforms` in the same dir as the `exe`-file and copy the `qwindows.dll` into it. 
   
# Contribution

To contribute, please format your code using the provided `uncrustify.cfg`. Uncrustify can be found in most Linux Distro package managers. 
For Windows, there are installers available.
