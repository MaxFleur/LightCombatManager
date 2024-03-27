find_package(Catch2 REQUIRED)

add_executable(tests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/CharacterHandlerTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/handler/TableFileHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/CharFileHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/settings/SettingsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/widget/CombatTableWidgetTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/GeneralUtilsTest.cpp
)

target_link_libraries(tests
    PRIVATE Qt::Widgets Catch2::Catch2 additional handler settings utils
)

include(Catch)
catch_discover_tests(tests)
