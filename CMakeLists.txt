cmake_minimum_required(VERSION 3.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LightCombatManager LANGUAGES CXX)

find_package(QT NAMES Qt6 COMPONENTS SvgWidgets Widgets REQUIRED)
find_package(Qt6 COMPONENTS SvgWidgets Widgets REQUIRED)

include(CTest)
enable_testing()
add_custom_target(checks COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
add_dependencies(checks tests)

add_subdirectory(src)
add_subdirectory(test)

if(MSVC)
  target_compile_options(LightCombatManager PRIVATE /W4)
else()
  target_compile_options(LightCombatManager PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install and uninstall options
install(TARGETS LightCombatManager)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
