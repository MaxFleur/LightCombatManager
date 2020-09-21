#include "../../include/GUI/TableWidget.h"

// Create the welcome widget at instantiation
TableWidget::TableWidget(CharacterHandlerRef charHandler, QWidget *parent)
        : m_char(charHandler), QWidget( parent) {
    createTable();
}

// This function creates the table of this widget
void TableWidget::createTable() {
    
}
 
