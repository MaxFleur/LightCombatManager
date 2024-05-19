add_library (template INTERFACE)

target_include_directories (template
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(template INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/TemplatesListWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TemplatesListWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TemplatesWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TemplatesWidget.hpp
)

target_link_libraries(template
    INTERFACE Qt::Widgets utils
)
