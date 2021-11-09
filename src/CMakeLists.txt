add_subdirectory(GUI)
add_subdirectory(Handler)
add_subdirectory(Utils)

add_executable(LightCombatManager
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories (LightCombatManager
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LightCombatManager
    PRIVATE Qt5::Widgets GUI Handler Utils
)
