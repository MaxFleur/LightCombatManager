add_library (Handler INTERFACE)

target_include_directories (Handler
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(Handler INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharacterHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/FileHandler.cpp
    
)


target_link_libraries(Handler 
    INTERFACE Qt5::Widgets
)
