#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
    
// This class handles the table widget
class TableWidget : public QWidget {
    Q_OBJECT
    
public:
    TableWidget(CharacterHandlerRef charHandler, QWidget *parent = 0);
    
private:
    CharacterHandlerRef m_char;
    // Create the table
    void createTable();
    
    // These widgets create some free space around the text in the centre
    QTableWidget *tableWidget;
};
 
