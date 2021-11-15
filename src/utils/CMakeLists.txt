add_library (utils INTERFACE)

target_include_directories (utils
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(utils INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils.hpp
    
)

target_link_libraries(utils 
    INTERFACE Qt5::Widgets
)
