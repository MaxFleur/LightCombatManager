cmake_minimum_required(VERSION 3.5)

project(LightCombatManager LANGUAGES CXX)

set(SRC_LIST 
    include/GUI/mainwindow.h
    include/GUI/WelcomeWidget.h
    include/GUI/CharacterCreationWidget.h
    include/GUI/TableWidget.h

    main.cpp 
    src/GUI/mainwindow.cpp 
    src/GUI/WelcomeWidget.cpp 
    src/GUI/CharacterCreationWidget.cpp
    src/GUI/TableWidget.cpp
    src/CharacterHandler.cpp
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(
    include
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(LightCombatManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
