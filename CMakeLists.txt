cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(LightCombatManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

# Copy testing tables to build directory
configure_file(resources/tables/workingTable.csv test/TestingTables/workingTable.csv COPYONLY)
configure_file(resources/tables/brokenTable.csv test/TestingTables/brokenTable.csv COPYONLY)
