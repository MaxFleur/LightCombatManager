add_library (GUI INTERFACE)

add_subdirectory(Table)

target_include_directories (GUI 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(GUI INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StatusEffectDialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/StatusEffectDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/WelcomeWidget.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/WelcomeWidget.hpp 
)

target_link_libraries(GUI 
    INTERFACE Qt5::Widgets Table
)
