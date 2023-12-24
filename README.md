<div align="center">

   ![License badge](https://img.shields.io/badge/License-MIT-blue.svg)
   ![C++ badge](https://img.shields.io/badge/C++-17-blue.svg)
   ![CI badge](https://github.com/MaxFleur/LightCombatManager/actions/workflows/run.yml/badge.svg)
   ![Tag badge](https://img.shields.io/badge/Release-v1.11.1-blue.svg)

</div>

<p align="center">
  <img width="640" height="480" src="https://github.com/MaxFleur/LightCombatManager/assets/15110943/9e2a420e-c890-4868-b6cd-9512f2aae2ec">
</p>

# LightCombatManager

### A small, lightweight cross-platform combat manager for d20-based role-playing games, based on Qt.

With LightCombatManager (or just **LCM**), characters with initiative values, a health point counter and additional information (including ruleset-defined status effects) can be easily created.\
![Bossfight](https://github.com/MaxFleur/LightCombatManager/assets/15110943/a9164ab9-8a56-4226-bcb4-e5f5a9ba2684)

After character generation, LCM creates a full combat table, which can be used to easily manage the combat. LCM supports operations such as dragging and dropping rows, undoing changes or the deletion and subsequent addition of characters.\
![Start](https://github.com/MaxFleur/LightCombatManager/assets/15110943/d4a209a6-7abb-4149-be9c-314590923fb9)

If the game ends, but the current combat is not finished yet, the combat table can be saved and reopened later to continue the combat.

### Supported rulesets

LCM currently supports the following rulesets:
* D&D 3.0E
* D&D 3.5E
* D&D 5E
* Pathfinder 1E
* Pathfinder 2E
* Starfinder

Support for more d20-based rulesets might be added in the future.

# Tools & Installation
LCM is written in C++17. The following tools are used for development:
* [Qt6 or Qt5](https://www.qt.io/) for the user interface and the storing and loading of tables
* [Catch2](https://github.com/catchorg/Catch2) for Unit testing ([Catch2 license](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
* [Uncrustify](https://github.com/uncrustify/uncrustify) for code formatting.
* CMake as build system.

For most Linux distributions, all these libraries can be installed using their respective official package manager. For Windows, installers for Qt, CMake and Catch2 are available. If no Qt6 installation is found on the system, the application searches for a Qt5 installation instead.\
Alternatively, if you want to run the application without any additional installing, just download the binaries provided with the latest release.

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
8. The exe-file for the application should be found here. To successfully run the exe, `QT6Core.dll`, `QT6Widgets.dll` and `QT6Gui.dll` 
   have to be in the same file path as the exe (`QT5Core.dll`, `QT5Widgets.dll` and `QT5Gui.dll` for Qt5). You also have to create a directory named
   `platforms` in the same dir as the exe-file and copy the `qwindows.dll` into it. 

# Contribution

## More features

LCM constantly gets more features and improvements. If there is a ruleset you wish to be supported or another feature you want to be included, feel free to open a new issue for discussion!

## Developer information

If you want to contribute code, please format it using the provided `uncrustify.cfg` before opening a pull request. On Linux for example, you can format the entire repository's code with one single line:
```
find . \( -name "*.cpp" -o -name "*.hpp" \) -exec uncrustify -c uncrustify.cfg --replace --no-backup {} +
```

The repository always contains branches for the current and last major release. As `dev_staging` is the branch constantly being developed on, make sure to select it as target branch in case you want to open a new Pull Request.
