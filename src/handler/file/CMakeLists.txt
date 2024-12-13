add_library (fileHandler INTERFACE)

target_include_directories (fileHandler
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(fileHandler INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/BaseFileHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/BaseFileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/CharFileHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/CharFileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/EffectFileHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/EffectFileHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TableFileHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TableFileHandler.hpp
)

target_link_libraries(fileHandler
    INTERFACE Qt::Widgets
)
