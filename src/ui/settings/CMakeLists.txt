add_library (mainSettings INTERFACE)

target_include_directories (mainSettings 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(mainSettings INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/DirSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DirSettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MainSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MainSettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SettingsDialog.cpp
)


target_link_libraries(mainSettings 
    INTERFACE Qt5::Widgets
)
