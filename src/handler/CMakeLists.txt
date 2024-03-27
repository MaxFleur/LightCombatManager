add_library (handler INTERFACE)

target_include_directories (handler
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(handler INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/TableFileHandler.hpp
        ${CMAKE_CURRENT_LIST_DIR}/TableFileHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/BaseFileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharFileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharFileHandler.cpp
)

target_link_libraries(handler 
    INTERFACE Qt::Widgets
)
