add_library (ui INTERFACE)

add_subdirectory(settings)
add_subdirectory(table)

target_include_directories (ui 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(ui INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CheckBoxStyle.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/WelcomeWidget.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/WelcomeWidget.hpp 
)

if(${Qt6_FOUND})
    target_link_libraries(ui
        INTERFACE Qt::SvgWidgets Qt::Widgets fileHandler settings table
    )
else()
    target_link_libraries(ui
        INTERFACE Qt::Svg Qt::Widgets fileHandler settings table
    )
endif()
