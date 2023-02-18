<div align="center">

   ![License badge](https://img.shields.io/badge/License-MIT-blue.svg)
   ![C++ badge](https://img.shields.io/badge/C++-17-blue.svg)
   ![Test badge](https://img.shields.io/badge/tests-passing-green.svg)
   ![Tag badge](https://img.shields.io/badge/Release-v1.9.0-blue.svg)

</div>

# LightCombatManager

### A small, lightweight, cross-platform combat manager for d20 based role-playing games, based on Qt.

Using LightCombatManager (or  just **LCM**), you can easily create characters with initiative values, a HP counter and additional information. You can also add status effects to each character.\
![image](https://user-images.githubusercontent.com/15110943/219160410-3287af7b-9518-4cef-80e2-bab211262d9e.png)

After creation, the program creates a full combat table. Using this table, you can easily manage the combat. LCM supports operations such as dragging and dropping rows, undoing changes or the addition and deletion of characters. \
![image](https://user-images.githubusercontent.com/15110943/219162574-db3497e4-a802-44f3-a1fa-ddbdc765854e.png)

If the game ends, but the current combat is not finished yet, you can save the table as a csv file, then reopen it later and continue as if you were never interrupted.

### Supported rulesets

LCM currently supports the following rulesets:
* D&D 3.0E
* D&D 3.5E
* D&D 5E
* Pathfinder 1E
* Pathfinder 2E
* Starfinder

More d20-based rulesets might be supported in the future.

# Tools & Installation
LCM is written in C++17. The following tools are used for development:
* [Qt5](https://www.qt.io/) for the user interface
* [Catch2](https://github.com/catchorg/Catch2) for Unit testing ([Catch2 license](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
* [Uncrustify](https://github.com/uncrustify/uncrustify) for code formatting.
* CMake as build system.

For most Linux Distros, all these libraries should be contained using the official package managers. For Windows, installers for Qt5, CMake and Catch2 are available.

## Linux

1. Download or clone this repository.
2. Open a terminal and `cd` into this repository.
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Hit `cmake ..` and then `make`. Start the application with `./src/LightCombatManager`.

## Windows

The following description is focused on building the application with `cmake` and Visual Studio 2022.

1. Download or clone this repository.
2. Open up `cmd` or `git cmd` and navigate to this repository. 
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Set the CMake Prefix Path for Qt inside the `CMakeLists.txt`. Example: `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.1\\msvc2019_64\\")` for Qt 5.15 and msvc compiler.
5. Hit `cmake.exe -G “Visual Studio 17 2022” -A x64 ..` This will create an sln-file. 
6. Open the sln file using Visual Studio. Build the application in Debug- or Release-Mode.
7. Inside the build-file, a file named `Debug` or `Release` should have been created. Navigate to this folder and then to the `src` subdirectory.
8. The exe-file for the application should be found here. To successfully run the exe, `QT5Core.dll`, `QT5Widgets.dll` and `QT5Gui.dll` 
   have to be in the same file path as the exe (`QT5Cored.dll`, `QT5Widgetsd.dll` and `QT5Guid.dll` for Debug). You also have to create a directory named
   `platforms` in the same dir as the exe-file and copy the `qwindows.dll` into it. 
   
# Contribution

If there is a ruleset you wish to be supported or another feature you might want to be included, feel free to open a new issue.\
If you want to contribute code, please format it using the provided `uncrustify.cfg`.
