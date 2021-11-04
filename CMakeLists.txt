cmake_minimum_required(VERSION 3.5)

project(LightCombatManager LANGUAGES CXX)

set(SRC_LIST 
    src/main.cpp 
    
    src/Handler/CharacterHandler.hpp
    src/Handler/CharacterHandler.cpp
    src/Handler/FileHandler.hpp
    src/Handler/FileHandler.cpp
    
    src/Utils/Utils.cpp
    src/Utils/Utils.hpp
    
    src/GUI/CharacterCreationWidget.hpp
    src/GUI/CharacterCreationWidget.cpp
    src/GUI/MainWindow.cpp
    src/GUI/MainWindow.hpp
    src/GUI/StatusEffectDialog.cpp
    src/GUI/StatusEffectDialog.hpp
    src/GUI/WelcomeWidget.cpp 
    src/GUI/WelcomeWidget.hpp 
    
    src/GUI/Table/EditCombatDialog.hpp
    src/GUI/Table/EditCombatDialog.cpp
    src/GUI/Table/TableWidget.hpp
    src/GUI/Table/TableWidget.cpp
    src/GUI/Table/CustomTable.hpp
    src/GUI/Table/CustomTable.cpp
    src/GUI/Table/Delegate.hpp
    src/GUI/Table/Delegate.cpp
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

