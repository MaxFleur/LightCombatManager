add_library (settings INTERFACE)

target_include_directories (settings 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(settings INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/SettingsData.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.cpp
)


target_link_libraries(settings 
    INTERFACE Qt5::Widgets
)
