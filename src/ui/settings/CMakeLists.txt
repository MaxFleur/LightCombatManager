add_library (settings INTERFACE)

target_include_directories (settings 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(settings INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/DirSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/DirSettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MainSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MainSettings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TableSettings.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TableSettings.cpp
)


target_link_libraries(settings 
    INTERFACE Qt5::Widgets
)
