add_library (additional INTERFACE)

target_include_directories (additional
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
) 

target_sources(additional INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/AdditionalInfoData.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AdditionalInfoWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AdditionalInfoWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FocusOutLineEdit.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StatusEffectButton.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StatusEffectButton.cpp
)

target_link_libraries(additional
    INTERFACE Qt::Widgets utils
)
