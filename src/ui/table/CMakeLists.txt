add_library (table INTERFACE )

add_subdirectory(additional)
add_subdirectory(dialog)
add_subdirectory(templates)

target_include_directories (table 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(table INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CombatWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CombatWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CombatTableWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CombatTableWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DelegateSpinBox.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DelegateSpinBox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Undo.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Undo.cpp
)

target_link_libraries(table 
    INTERFACE Qt::Widgets additional dialog settings templates
)
