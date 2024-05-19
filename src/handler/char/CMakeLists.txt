add_library (charHandler INTERFACE)

target_include_directories (charHandler
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(charHandler INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.cpp
)
