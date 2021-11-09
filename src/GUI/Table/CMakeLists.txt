add_library (Table INTERFACE)

target_include_directories (Table 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(Table INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/AddCharacterDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AddCharacterDialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TableWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TableWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CustomTable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CustomTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Delegate.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Delegate.cpp
)


target_link_libraries(Table 
    INTERFACE Qt5::Widgets
)
