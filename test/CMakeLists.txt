include(CTest)
enable_testing()

find_package(Catch2 REQUIRED)

add_executable(tests
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UtilsTest/UtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HandlerTest/CharacterHandlerTest.cpp
)

target_link_libraries(tests PRIVATE 
    Qt5::Widgets 
    Catch2::Catch2 
    Utils 
    Handler
)
