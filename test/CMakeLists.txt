find_package(Catch2 REQUIRED)

add_executable(tests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/CharacterHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/FileHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/GeneralUtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/TableUtilsTest.cpp
)

target_link_libraries(tests
    PRIVATE Qt::Widgets Catch2::Catch2 additional handler settings utils
)

include(Catch)
catch_discover_tests(tests)
