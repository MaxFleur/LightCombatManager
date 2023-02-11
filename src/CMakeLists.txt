add_subdirectory(ui)
add_subdirectory(handler)
add_subdirectory(utils)

IF (WIN32)
    add_executable(LightCombatManager WIN32
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    )
ELSE()
    add_executable(LightCombatManager
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    )
ENDIF()

target_include_directories (LightCombatManager
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LightCombatManager
    PRIVATE Qt5::Widgets ui handler utils
)
