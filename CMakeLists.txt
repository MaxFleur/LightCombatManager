cmake_minimum_required(VERSION 3.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(LightCombatManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 QUIET COMPONENTS Widgets)

if(${Qt6_FOUND})
    find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
    find_package(Qt6 COMPONENTS Widgets REQUIRED)
else()
    find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
endif()

include(CTest)
enable_testing()
add_custom_target(checks COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
add_dependencies(checks tests)

add_subdirectory(src)
add_subdirectory(test)
