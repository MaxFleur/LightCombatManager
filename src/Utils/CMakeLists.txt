add_library (Utils INTERFACE)

target_include_directories (Utils
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(Utils INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils.hpp
    
)

target_link_libraries(Utils 
    INTERFACE Qt5::Widgets
)
