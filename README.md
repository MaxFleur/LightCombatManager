<div align="center">

   ![License badge](https://img.shields.io/badge/License-GPLv3-blue.svg)
   ![C++ badge](https://img.shields.io/badge/C++-20-blue.svg)
   ![CI MacOS badge](https://github.com/MaxFleur/LightCombatManager/actions/workflows/run-mac.yml/badge.svg?event=push)
   ![CI Ubuntu badge](https://github.com/MaxFleur/LightCombatManager/actions/workflows/run-ubuntu.yml/badge.svg?event=push)
   ![CI Windows badge](https://github.com/MaxFleur/LightCombatManager/actions/workflows/run-windows.yml/badge.svg?event=push)
   ![Tag badge](https://img.shields.io/badge/Release-v2.2.1-blue.svg)

</div>

<p align="center">
  <img width="480" height="360" src="https://github.com/MaxFleur/LightCombatManager/assets/15110943/9e2a420e-c890-4868-b6cd-9512f2aae2ec">
</p>

# LightCombatManager

### A small, lightweight cross-platform combat manager for d20-based role-playing games, based on Qt.

![bossfight](https://github.com/user-attachments/assets/66adc3ed-2568-4625-9e54-a44e5e56d65d)

With LightCombatManager (or just **LCM**), you can easily manage a combat for a d20-based RPG. The table supports all sorts of combat-based operations, such as reordering rows when a character moves their initiative, removing or adding ruleset-defined status effects to one or multiple characters or subsequent addition of characters who just joined the combat. Undoing and logging changes are also supported!

![new_fight](https://github.com/user-attachments/assets/4992d9fc-5b0f-436f-a690-fb8d3c502c96)

LCM provides an intuitive character editor, where characters with initiative value and modifier, a health point counter and additional information can be easily created.\
If the game ends, but the current combat is not finished yet, you can save it on the PC. Characters can also be stored as templates for later usage.

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
LCM is written in C++20. The following frameworks are used for development:
* [Qt6 or Qt5](https://www.qt.io/) for the user interface and the storing and loading of tables.
   * If no Qt6 installation is found on the system, the application searches for a Qt5 installation instead.
   * Note that for the correct displaying of svg files, the Qt SVG plugin is needed.
* [Catch2 v3](https://github.com/catchorg/Catch2) for Unit testing ([Catch2 license](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
* [Uncrustify](https://github.com/uncrustify/uncrustify) for code formatting.
* CMake as build system.

The following commands will install all necessary requirements at once:
### Ubuntu:
`sudo apt install qtbase5-dev libqt5svg5 libqt5svg5-dev qt6-base-dev libqt6svg6 libqt6svg6-dev catch2 uncrustify cmake`
### Arch Linux:
`sudo pacman -S qt5-base qt5-svg qt6-base qt6-svg catch2 uncrustify cmake`
### MacOS:
`brew install qt@5 qt@6 catch2 uncrustify cmake`

For Windows, installers for Qt, CMake and Catch2 are available. Make sure to install the Qt SVG plugin as well!\
Alternatively, if you want to run the application without any additional installing, just download the binaries provided with the latest release.

## Build on Linux/MacOS

1. Clone this repository and `cd` into it.
2. `mkdir build && cd build`
3. On MacOS: Hit `cmake -DCMAKE_PREFIX_PATH="$(brew --prefix qt@5)"` (Change to `qt@6` for Qt6), on Linux: Hit `cmake ..`
4. `make`
5. Start the application with `./src/LightCombatManager`.

## Build on Windows

The following description is focused on building the application with `cmake`, MSVC 2019 and Visual Studio 2022.

1. Clone this repository and `cd` into it using `git cmd`.
2. `mkdir build && cd build`
3. Set the CMake Prefix Path for Qt inside the `CMakeLists.txt`. Example: `set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64\\")` for Qt 5.15 and msvc compiler.
4. Hit `cmake.exe -G “Visual Studio 17 2022” -A x64 ..` This will create an sln-file.
5. Open the sln file using Visual Studio. Build the application in Debug- or Release-Mode.
6. Inside the build-file, a file named `Debug` or `Release` should have been created. Navigate to this folder and then to the `src` subdirectory.
7. The exe-file for the application should be found here. To successfully run the exe, make sure that the directory contains the corresponding qt dll files as well. The final built application dir for Qt6 should look as follows:
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

# License
Until Version 2.1.0, LCM used the [MIT license](https://www.mit.edu/~amini/LICENSE.md). Since Version 2.2.0, LCM uses [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.txt).

# Feedback and Contribution

LCM is still in active development and constantly gets more features and improvements. If there is a ruleset you wish to be supported or another feature you want to be included, feel free to open a new issue for discussion!\
The `master` branch provides the most recent stable version. If you want to get the bleeding edge version with all recent updates and features, checkout the `dev_staging` branch.

## Developer information

If you want to contribute code, please format it using the provided `uncrustify.cfg` before opening a pull request. On Linux or MacOS for example, you can format all modified or new code with one single line:
```
find . \( -name "*.cpp" -o -name "*.hpp" \) -exec uncrustify -c uncrustify.cfg --replace --no-backup {} +
```

The repository always contains branches for the current and last major release for better bug traceability. As `dev_staging` is the branch constantly being developed on, make sure to select it as target branch for Pull Requests.
