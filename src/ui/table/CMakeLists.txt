add_library (table INTERFACE)

add_subdirectory(dialog)

target_include_directories (table 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(table INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/DisabledArrowKeyTable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DisabledArrowKeyTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/DelegateSpinBox.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DelegateSpinBox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TableWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TableWidget.cpp
)


target_link_libraries(table 
    INTERFACE Qt5::Widgets dialog
)
