include(CTest)
enable_testing()

find_package(Catch2 REQUIRED)

add_executable(tests
    ${CMAKE_CURRENT_LIST_DIR}/Utils/UtilsTest.cpp
)

target_link_libraries(tests
    PRIVATE Qt5::Widgets Catch2::Catch2 Utils Handler
)
