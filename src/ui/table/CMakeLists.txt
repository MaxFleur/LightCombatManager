add_library (table INTERFACE)

add_subdirectory(dialog)

target_include_directories (table 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(table INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CustomTable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CustomTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Delegate.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Delegate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TableWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TableWidget.cpp
)


target_link_libraries(table 
    INTERFACE Qt5::Widgets dialog
)
