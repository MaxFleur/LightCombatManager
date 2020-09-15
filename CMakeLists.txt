cmake_minimum_required(VERSION 3.5)

project(LightCombatManager LANGUAGES CXX)

set(SRC_LIST 
    main.cpp 
    mainwindow.cpp 
    src/GUI/WelcomeLayout.cpp 
    src/GUI/CharacterCreationLayout.cpp
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(
    include
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(LightCombatManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
