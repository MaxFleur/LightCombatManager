#pragma once

#include <QtWidgets>
#include "../CharacterHandler.h"
    
// This class handles the table widget
class TableWidget : public QWidget {
    Q_OBJECT
    
public:
    TableWidget(CharacterHandlerRef charHandler, QWidget *parent = 0);
    ~TableWidget();
    
private:
    CharacterHandlerRef m_char;
    // Create the table
    void createTable();
    
    // Main table widget and header
    QTableWidget    *tableWidget;
    QStringList     tableHeader;
    // The final layout that will be returned
    QVBoxLayout     *tableLayout;
};
 
