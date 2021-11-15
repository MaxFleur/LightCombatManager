add_library (handler INTERFACE)

target_include_directories (handler
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(handler INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/FileHandler.cpp
    
)


target_link_libraries(handler 
    INTERFACE Qt5::Widgets
)
