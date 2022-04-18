add_library (tableSettings INTERFACE)

target_include_directories (tableSettings 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(tableSettings INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/TableSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TableSettings.cpp
)


target_link_libraries(tableSettings 
    INTERFACE Qt5::Widgets
)
