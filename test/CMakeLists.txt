find_package(Catch2 3 QUIET)
if(${Catch2_FOUND})
    find_package(Catch2 3 REQUIRED)
    add_definitions(-DCATCH2_V3)
else()
    find_package(Catch2 REQUIRED)
    add_definitions(-DCATCH2_V2)
endif()

add_executable(tests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp

    ${CMAKE_CURRENT_LIST_DIR}/handler/CharacterHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/CharFileHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/EffectFileHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/TableFileHandlerTest.cpp

    ${CMAKE_CURRENT_LIST_DIR}/ui/settings/SettingsTest.cpp

    ${CMAKE_CURRENT_LIST_DIR}/ui/widget/CombatTableWidgetTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/widget/LogListWidgetTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/widget/TemplatesListWidgetTest.cpp

    ${CMAKE_CURRENT_LIST_DIR}/utils/GeneralUtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/UtilsFilesTest.cpp
)

target_link_libraries(tests
    PRIVATE Qt::Widgets Catch2::Catch2 additional charHandler fileHandler settings template utils
)

include(Catch)
catch_discover_tests(tests)
