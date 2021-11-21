add_library (dialog INTERFACE)

target_include_directories (dialog 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(dialog INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/AddCharacterDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AddCharacterDialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/StatusEffectDialog.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StatusEffectDialog.cpp
)


target_link_libraries(dialog 
    INTERFACE Qt5::Widgets
)
