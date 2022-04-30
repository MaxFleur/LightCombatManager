add_library (ui INTERFACE)

add_subdirectory(settings)
add_subdirectory(table)

target_include_directories (ui 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(ui INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/WelcomeWidget.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/WelcomeWidget.hpp 
)

target_link_libraries(ui 
    INTERFACE Qt5::Widgets settings table
)
