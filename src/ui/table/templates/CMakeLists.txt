add_library (templates INTERFACE)

target_include_directories (templates
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(templates INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/TemplatesListWidget.hpp
        ${CMAKE_CURRENT_LIST_DIR}/TemplatesListWidget.cpp
)

#target_link_libraries(dialog
#       INTERFACE Qt::Widgets utils
#)
