<div align="center">

   ![License badge](https://img.shields.io/badge/License-MIT-blue.svg)
   ![C++ badge](https://img.shields.io/badge/C++-17-blue.svg)
   ![CI badge](https://github.com/MaxFleur/LightCombatManager/actions/workflows/run.yml/badge.svg)
   ![Tag badge](https://img.shields.io/badge/Release-v2.0.0-blue.svg)

</div>

<p align="center">
  <img width="640" height="480" src="https://github.com/MaxFleur/LightCombatManager/assets/15110943/9e2a420e-c890-4868-b6cd-9512f2aae2ec">
</p>

# LightCombatManager

### A small, lightweight cross-platform combat manager for d20-based role-playing games, based on Qt.

With LightCombatManager (or just **LCM**), characters with initiative values, a health point counter and additional information (including ruleset-defined status effects) can be easily created.\
![Bossfight](https://github.com/MaxFleur/LightCombatManager/assets/15110943/b85a9614-977e-4376-b61f-f1f76e777c82)

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
* [Qt6 or Qt5](https://www.qt.io/) for the user interface and the storing and loading of tables.
   * If no Qt6 installation is found on the system, the application searches for a Qt5 installation instead.
   * Note that for the correct displaying of svg files, the Qt SVG plugin is needed.
* [Catch2](https://github.com/catchorg/Catch2) for Unit testing ([Catch2 license](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
* [Uncrustify](https://github.com/uncrustify/uncrustify) for code formatting.
* CMake as build system.

For most Linux distributions, all these libraries can be installed using their respective official package manager. For example, this command installs all dependencies for Ubuntu 22.04:\
`sudo apt install qtbase5-dev libqt5svg5 qt6-base-dev libqt6svg6 catch2 uncrustify cmake`\
For Arch Linux and derivates:\
`sudo pacman -S qt5-base qt5-svg qt6-base qt6-svg catch2 uncrustify cmake`

For Windows, installers for Qt, CMake and Catch2 are available. Make sure to install the Qt SVG plugin as well!\
Alternatively, if you want to run the application without any additional installing, just download the binaries provided with the latest release.

## Build on Linux

1. Download or clone this repository.
2. Open a terminal and `cd` into this repository.
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Hit `cmake ..` and then `make`. Start the application with `./src/LightCombatManager`.

## Build on Windows

The following description is focused on building the application with `cmake`, MSVC 2019 and Visual Studio 2022.

1. Download or clone this repository.
2. Open up `cmd` or `git cmd` and navigate to this repository. 
3. Create a build folder: `mkdir build`. Navigate into this folder via `cd build`.
4. Set the CMake Prefix Path for Qt inside the `CMakeLists.txt`. Example: `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64\\")` for Qt 5.15 and msvc compiler.
5. Hit `cmake.exe -G “Visual Studio 17 2022” -A x64 ..` This will create an sln-file. 
6. Open the sln file using Visual Studio. Build the application in Debug- or Release-Mode.
7. Inside the build-file, a file named `Debug` or `Release` should have been created. Navigate to this folder and then to the `src` subdirectory.
8. The exe-file for the application should be found here. To successfully run the exe, make sure that the directory contains the corresponding qt dll files as well. The final built application dir for Qt6 should look as follows:
```
├── platforms
|   ├── qwindows.dll
├── imageformats
|   ├── qsvg.dll
├── LightCombatManager.exe
├── Qt6Core.dll
├── Qt6Gui.dll
├── Qt6Svg.dll
├── Qt6Widgets.dll
```
The dll files can be found under the Qt installation path under `/bin` and `/plugins`.

# Contribution

## More features

LCM constantly gets more features and improvements. If there is a ruleset you wish to be supported or another feature you want to be included, feel free to open a new issue for discussion!

## Developer information

If you want to contribute code, please format it using the provided `uncrustify.cfg` before opening a pull request. On Linux for example, you can format the entire repository's code with one single line:
```
find . \( -name "*.cpp" -o -name "*.hpp" \) -exec uncrustify -c uncrustify.cfg --replace --no-backup {} +
```

The repository always contains branches for the current and last major release. As `dev_staging` is the branch constantly being developed on, make sure to select it as target branch in case you want to open a new Pull Request. In addition, the branches for the last corresponding major release and its minor updates are also stored for better bug traceability.
