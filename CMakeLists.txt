cmake_minimum_required(VERSION 3.5)

project(LightCombatManager LANGUAGES CXX)

set(SRC_LIST 
    include/GUI/MainWindow.hpp
    include/GUI/WelcomeWidget.hpp
    include/GUI/CharacterCreationWidget.hpp
    include/GUI/Table/TableWidget.hpp
    include/GUI/Table/CustomTable.hpp
    include/GUI/Table/Delegate.hpp
    include/FileHandler.hpp

    main.cpp 
    src/GUI/MainWindow.cpp 
    src/GUI/WelcomeWidget.cpp 
    src/GUI/CharacterCreationWidget.cpp
    src/GUI/Table/TableWidget.cpp
    src/GUI/Table/CustomTable.cpp
    src/GUI/Table/Delegate.cpp
    src/CharacterHandler.cpp
    src/FileHandler.cpp
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(
    include
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(LightCombatManager 
    PRIVATE Qt5::Widgets
)

