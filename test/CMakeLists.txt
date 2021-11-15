include(CTest)
enable_testing()

find_package(Catch2 REQUIRED)

add_executable(tests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/UtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/CharacterHandlerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/handler/FileHandlerTest.cpp
)

target_link_libraries(tests PRIVATE 
    Qt5::Widgets 
    Catch2::Catch2 
    utils 
    handler
)
